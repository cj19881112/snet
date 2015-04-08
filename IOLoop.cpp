#include "IOLoop.h"

#include "IOEvent.h"
#include "IOEventHandler.h"

#include <sys/epoll.h>
#include <errno.h>

#include <cassert>

static int ioEventToEpollEvent(int ioEvent)
{
	int epollEvent = 0;
	if (ioEvent & IO_READ) {
		epollEvent |= EPOLLIN;
	}
	if (ioEvent & IO_WRITE) {
		epollEvent |= EPOLLOUT;
	}
	return epollEvent;
}

static int epollEventToIOEvent(int epollEvent)
{
	IOEvent ioEvent = IO_NONE;
	if (epollEvent & EPOLLIN) {
		ioEvent |= IO_READ;
	}
	if (epollEvent & EPOLLOUT) {
		ioEvent |= IO_WRITE;
	}
	if (epollEvent & EPOLLERR) {
		ioEvent |= IO_ERROR;
	}
	return ioEvent;
}

IOLoop::IOLoop(int maxEvent, int timeout)
	: _epollFd(-1), _maxEvent(maxEvent),
	_timeout(timeout), _tupleCount(4096000)
{
	assert(maxEvent>0 && "maxEvent must > 0");

	_handlerTuples = new Tuple[_tupleCount];
	assert(_handlerTuples && "not enought memory");
}

IOLoop::~IOLoop()
{
	delete [] _handlerTuples;
}

int IOLoop::init()
{
	_epollFd = epoll_create(10);
	if (-1 == _epollFd) {
		return -1;
	}
	return 0;
}

int IOLoop::run()
{
	struct epoll_event events[_maxEvent];
	while (1) {
		int nfds = 0;
		do {
			nfds = epoll_wait(_epollFd, events, _maxEvent, _timeout);
		} while (nfds<0 && EINTR==errno);

		if (nfds < 0) {
			return -1;
		}	

		for (int i = 0; i < nfds; i++) {
			dispatch(events[i]);
		}
	}
}

int IOLoop::add(int sockFd, IOEvent e, IOEventHandler *handler)
{
	if (sockFd > _tupleCount) {
		_tupleCount *= 2;
		Tuple *p = new Tuple[_tupleCount];
		if (!p) {
			return -1;
		}
		delete [] _handlerTuples;
		_handlerTuples = p;
	}

	_handlerTuples[sockFd].sockFd = sockFd;
	_handlerTuples[sockFd].wantEvent = e;
	_handlerTuples[sockFd].handler = handler;

	return ctl(EPOLL_CTL_ADD, sockFd, e);
}

int IOLoop::add(int sockFd, IOEvent e)
{
	assert(sockFd < _tupleCount && "error sockFd > _handlerTuples");
	if (!eventRegistered(sockFd, e)) {
		addEventToTuple(sockFd, e);
		e = _handlerTuples[sockFd].wantEvent;
		return ctl(EPOLL_CTL_MOD, sockFd, e);
	}
	return 0;
}

int IOLoop::remove(int sockFd)
{
	assert(sockFd < _tupleCount && "error sockFd > _handlerTuples");
	_handlerTuples[sockFd] = Tuple();
	return ctl(EPOLL_CTL_DEL, sockFd, IO_NONE);
}

int IOLoop::remove(int sockFd, IOEvent e)
{
	assert(sockFd < _tupleCount && "error sockFd > _handlerTuples");
	if (eventRegistered(sockFd, e)) {
		removeEventFromTuple(sockFd, e);
		e = _handlerTuples[sockFd].wantEvent;
		return ctl(EPOLL_CTL_MOD, sockFd, e);
	}
	return 0;
}


void IOLoop::dispatch(struct epoll_event e)
{
	int fd = e.data.fd;
	int events = e.events; 

	if (fd > _tupleCount) {
		/* something wrong */
		assert(0 && "文件描述符对应的处理器不存在!");
		return ;
	}

	IOEventHandler *handler = _handlerTuples[fd].handler;	

	assert(handler && "找到的handler是空的!");

	if (handler) {
		handler->handleEvent(this, fd, epollEventToIOEvent(events));
	}
}

int IOLoop::eventRegistered(int sockFd, int event)
{
	return (_handlerTuples[sockFd].wantEvent & event);
}

void IOLoop::addEventToTuple(int sockFd, int event)
{
	_handlerTuples[sockFd].wantEvent |= event;
}

void IOLoop::removeEventFromTuple(int sockFd, int event)
{
	_handlerTuples[sockFd].wantEvent &= ~event;
}

int IOLoop::ctl(int cmd, int sockFd, int event)
{
	struct epoll_event e;
	e.data.fd = sockFd;
	e.events = ioEventToEpollEvent(event);
	return epoll_ctl(_epollFd, cmd, sockFd, &e);
}
