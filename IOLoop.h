#ifndef _IO_LOOP_H_
#define _IO_LOOP_H_

#include <sys/epoll.h>
#include "IOEvent.h"

class IOEventHandler;

class IOLoop {
public:
	IOLoop(int maxEvent=4096, int timeout=-1);
	~IOLoop();

	int init();
	int run();

	int add(int sockFd, IOEvent e, IOEventHandler *handler);
	int add(int sockFd, IOEvent e);

	int remove(int sockFd);
	int remove(int sockFd, IOEvent e);

private:
	void dispatch(struct epoll_event e);
	int eventRegistered(int sockFd, int event);
	void addEventToTuple(int sockFd, int event);	
	void removeEventFromTuple(int sockFd, int event);
	int ctl(int cmd, int sockFd, int event);

	struct Tuple {
		Tuple(int fd=-1, IOEventHandler *h=0, IOEvent e=0) 
			: sockFd(fd), handler(h), wantEvent(e)
		{}
		int sockFd;
		IOEventHandler *handler;
		IOEvent wantEvent;
	};

private:
	int _epollFd;
	int _maxEvent;
	int _timeout;

	Tuple *_handlerTuples;
	int _tupleCount;
};

#endif

