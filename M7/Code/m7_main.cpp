#include "sys/system.h"

int main(void)
{
	sys::m7_init();
	
	while (1) {
		sys::m7_update();
	}
}