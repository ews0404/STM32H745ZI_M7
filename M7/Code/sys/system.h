#pragma once
#include <stdint.h>


namespace sys7
{
	void m7_init(void);
	void m7_update(void);
	
	uint32_t getMillis(void);
	uint32_t getMillisSince(uint32_t oldMillis);
}


// M7 settings
#define M7_SYSCLOCK_HZ	400000000		// M7 core clock rate in Hz
#define M7_LED_MILLIS	250				// M7 led blink rate in milliseconds


// debug macros
#ifdef DEBUG
	#include <stdio.h>
	#define SYS_TRACE(...) 
	#define SYS_LOG(...)
	#define SYS_WARN(...) 
	#define SYS_ERROR(...)
#elif 
	#define SYS_TRACE(...) 
	#define SYS_LOG(...) 
	#define SYS_WARN(...) 
	#define SYS_ERROR(...) 
#endif 