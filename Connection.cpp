#include "Connection.h"
#include "IOLoop.h"
#include "DisconnectListener.h"
#include "ConnectionHandler.h"

#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>

#include <iostream>

void Connection::init(IOLoop *loop, int sockFd)
{
	_loop = loop;
	_sockFd = sockFd;
	_state = ST_CONNECTED;
	_loop->add(_sockFd, IO_READ, this);
	if (_connectionHandler) {
		_connectionHandler->reset();
	}
}

void Connection::handleEvent(IOLoop *loop, IOEvent ioEvent)
{
	if (ioEvent & IO_READ) {
		handleRead();
	}
	if (ioEvent & IO_WRITE) {
		handleWrite();
	}
	if (ioEvent & IO_ERROR) {
		_state = ST_ERROR;
	}

	if (_state == ST_CLOSED || _state == ST_ERROR) {
		loop->remove(_sockFd);
		::close(_sockFd);
		_disconnectListener->disconnect(this);
	}
}

void Connection::handleRead()
{
	char buf[MAX_READ_BUF] = {0, };

	int n = ::recv(_sockFd, buf, MAX_READ_BUF, 0);
	if (n < 0) {
		if (EINTR == errno || EAGAIN == errno) {
			return ;
		}
		_connectionHandler->handleError(this);
		_state = ST_ERROR;
	} else if (n == 0) {
		_connectionHandler->handleClose(this);
		_state = ST_CLOSED;
	} else {
		_connectionHandler->handleData(this, buf, n);
	}
}

void Connection::handleWrite()
{
	int n = ::send(_sockFd, _outputBuffer.data(), _outputBuffer.size(), 0);
	if (n < 0) {
		if (EINTR == errno || EAGAIN == errno) {
			_loop->add(_sockFd, IO_WRITE);
		} else {
			_state = ST_ERROR;
		}
	} else if (n == _outputBuffer.size()){
		_outputBuffer.clear();
		_loop->remove(_sockFd, IO_WRITE);
		_connectionHandler->handleSendComplete(this);
	} else {
		_outputBuffer = _outputBuffer.substr(n);
		_loop->add(_sockFd, IO_WRITE);
	}
}

void Connection::sendData(const char *buf, int len)
{
	_outputBuffer.append(buf, len);
	_loop->add(_sockFd, IO_WRITE);
}

