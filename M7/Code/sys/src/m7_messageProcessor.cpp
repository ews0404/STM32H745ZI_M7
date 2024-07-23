#include "../inc/m7_messageProcessor.h"
#include "../Common/inc/messageQueue.h"
#include "../system.h"
#include "../Common/inc/gpio.h"
#include "../Common/inc/messageID.h"
#include <string.h>
#include <stdio.h>


using namespace gpio;
namespace mq = messageQueue;


static mq::MessageQueueBufferType mbuf;
static pinDef redLED = { .port = GPIOB, .pin = PIN_14, .mode = Output, .type = PushPull, .speed = Low, .pull = None, .alternate = AF0 };


static void processMessage(mq::MessageQueueBufferType* msg);


void m7_messageProcessor::init(void)
{
	messageQueue::init(messageQueue::M7toM4);
	configurePin(redLED);
}


void m7_messageProcessor::update(void)
{
	// if a message exists in the queue, copy it to the local buffer for processsing
	if (mq::hasMessages(mq::M4toM7)) {
		mq::readMessage(mq::M4toM7, &mbuf);
		processMessage(&mbuf);
	}
}

void processMessage(mq::MessageQueueBufferType* msg)
{
	switch (msg->messageID) {
		case(NoOp):
			break;
		
		case(SetLED):
			digitalWrite(redLED, (msg->data[0] % 2) == 1);
			break;
		
		case(PrintString):
			SYS_WARN("M7 does not support PrintString MessageID");
			break;
		
		default:
			SYS_WARN("unrecognized messageID: %d", msg->messageID);
	}
}

#include <stdarg.h>

void m7_messageProcessor::print_mq(const char* message)
{
	// send a string to M4 to get passed out of the debug port

	#ifdef DEBUG
		// add 1 to strlen() so that terminating \0 character gets copied 
		messageQueue::sendMessage(messageQueue::M7toM4, MessageID::PrintString, strlen((char*)message) + 1, (uint8_t*)message);	
	#endif // DEBUG
}
