#pragma once

namespace sys
{
	void m7_init(void);
	void m7_update(void);
}


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