#include "system.h"
#include "inc/gpio.h"
#include "inc/stm32h7xx.h"

using namespace gpio;

static pinDef m7_led = { .port = GPIOE, .pin = PIN_1, .mode = Output, .type = PushPull, .speed = Low, .pull = None, .alternate = AF0 };
static uint32_t m7_ledCounter = 0;

static void waitForM4(void);
static void startM4(void);

void sys::m7_init(void)
{
	// wait for the M4 to finish its configuration
	waitForM4();
	
	// let the M7 do its configuration while the M4 waits
	configurePin(m7_led);
	
	// start the M4 again, now both cores are running
	startM4();
}

void sys::m7_update(void)
{
	m7_ledCounter++;
	if (m7_ledCounter > 100000) { m7_ledCounter = 0; toggle(m7_led); }
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