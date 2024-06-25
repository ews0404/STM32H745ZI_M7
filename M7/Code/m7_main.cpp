#include "sys/system.h"

int main(void)
{
	sys7::m7_init();
	
	while (1) {
		sys7::m7_update();
	}
}