#include "inc/gpio.h"

using namespace gpio;

static pinDef m7_led = { .port = GPIOE, .pin = PIN_1, .mode = Output, .type = PushPull, .speed = Low, .pull = None, .alternate = AF0 };


int main(void)
{
	int i = 0;
	
	configurePin(m7_led);
	
	while (1)
	{
		++i;
		if (i > 1000000)
		{
			i = 0;
			toggle(m7_led);
		}
	}
}