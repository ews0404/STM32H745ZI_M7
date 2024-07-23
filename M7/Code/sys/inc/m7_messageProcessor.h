#pragma once
#include "../Common/inc/messageQueue.h"


namespace m7_messageProcessor
{
	void init(void);
	void update(void);
	
	void print_mq(const char* message);
}