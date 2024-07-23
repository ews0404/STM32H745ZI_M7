#include "sys/system.h"

int __attribute__((naked, noreturn)) main(void)
{
	sys7::m7_init();
	
	while (1) {
		sys7::m7_update();
	}
}