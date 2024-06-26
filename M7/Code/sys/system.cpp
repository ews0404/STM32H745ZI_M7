#include <limits.h>
#include "system.h"
#include "../Common/inc/gpio.h"
#include "inc/stm32h7xx.h"

using namespace gpio;

static pinDef m7_led = { .port = GPIOE, .pin = PIN_1, .mode = Output, .type = PushPull, .speed = Low, .pull = None, .alternate = AF0 };
static uint32_t m7_systick_milliseconds;
static uint32_t m7_led_millis;


static void m7_led_init(void);
static void m7_led_update(void);
static void m7_nvic_init(void);		
static void m7_fpu_init(void);
static void m7_systick_init(void);	
static void waitForM4(void);
static void startM4(void);


void sys7::m7_init(void)
{
	m7_led_init();
	
	// wait for the M4 to finish its configuration
	waitForM4();
	
	// let the M7 do its configuration while the M4 waits
	m7_nvic_init();
	m7_fpu_init();
	m7_systick_init();
	
	// start the M4 again, now both cores are running
	startM4();
}

void sys7::m7_update(void)
{
	m7_led_update();
}


void m7_led_init(void)
{
	configurePin(m7_led);
	digitalWrite(m7_led, 1);
	m7_led_millis = sys7::getMillis();
}


void m7_led_update(void)
{
	if (sys7::getMillisSince(m7_led_millis) > M7_LED_MILLIS) { 
		m7_led_millis = sys7::getMillis();
		toggle(m7_led); 
	}
}


void waitForM4(void)
{
	// wait for M4 core to set RCC->CGR:BOOT_C1 to true, indicating that it is done initializing
	// igore option byte BCM7 since using that stops the core clock which interferes with debugging
	while (!READ_BIT(RCC->GCR, RCC_GCR_BOOT_C1)) {}
}

void startM4(void)
{
	// signal the M4 core that the M7 core is done initializing by setting RCC->GCR:BOOT_C2 to true
	// igore option byte BCM4 since using that stops the core clock which interferes with debugging
	SET_BIT(RCC->GCR, RCC_GCR_BOOT_C2);
}


void m7_nvic_init(void)
{
	NVIC_SetPriorityGrouping(3);		// use 4 priority bits and 4 subpriority bits, for 16 interrupt priority levels
	__enable_irq();						// global interrupt enable
}


void m7_fpu_init(void)
{
	// allow full access to the floating point hardware processor (PM0253 4.7.6)
	SCB->CPACR |= (0x3 << 20);
}


void m7_systick_init(void)
{
	// set up systick counter to increment once every millisecond, based on the M4 clock
	// start SysTick timer to fire every 1 millisecond (PM0253 4.4)
	SysTick->LOAD = (M7_SYSCLOCK_HZ / 1000U - 1);						// 24 bit timer will count down from this value (SYST_RVR)
	SysTick->VAL = 0UL;													// set starting value of counter (SYST_CVR)
	NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);	// set SysTick interrupt priority
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;							// SysTick uses main processor clock (SYST_CSR)
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;							// count to 0 changes SysTick interrupt status to pending
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;							// enable SysTick counter
}


extern "C" void SysTick_Handler()
{
	m7_systick_milliseconds++;
}


uint32_t sys7::getMillis(void)
{
	return m7_systick_milliseconds;
}


uint32_t sys7::getMillisSince(uint32_t oldMillis)
{
	// if milliseconds < oldMillis the milliseconds counter overflowed and we can handle that happening one time,
	// each overflow takes ~49.7 days 
	return (m7_systick_milliseconds >= oldMillis) ? (m7_systick_milliseconds - oldMillis) : (UINT_MAX - oldMillis + m7_systick_milliseconds + 1);
}
