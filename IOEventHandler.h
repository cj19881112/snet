#ifndef _IO_EVENT_HANDLER_H_
#define _IO_EVENT_HANDLER_H_

#include "IOEvent.h"

class IOLoop;

class IOEventHandler {
public:
	virtual void handleEvent(IOLoop *ioLoop, IOEvent ioEvent) = 0;
};

#endif

