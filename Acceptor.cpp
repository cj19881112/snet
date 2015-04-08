#include "Acceptor.h"
#include "IOLoop.h"
#include "NewConnectionListener.h"

#include <sys/socket.h>
#include <fcntl.h>

#include <cassert>

Acceptor::Acceptor(int backlog)
	: _listenFd(-1), _backlog(backlog)
{
	assert(backlog > 0 && "backlog必须是正的");
}

Acceptor::~Acceptor()
{
}

int Acceptor::init(const InetAddr &addr, IOLoop *ioLoop)
{
	_listenFd = ::socket(AF_INET, SOCK_STREAM, 0);
	if (_listenFd == -1) {
		return -1;
	}

	if (-1==::bind(_listenFd, addr.addr(), addr.addrLen())) {
		return -1;
	}

	if (-1 == ::listen(_listenFd, _backlog)) {
		return -1;
	}

	int yes = 1;
	if (-1 == setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (yes))) {
		return -1;
	}

	if (-1 == ioLoop->add(_listenFd, IO_READ, this)) {
		return -1;
	}

	return 0;
}

void Acceptor::handleEvent(IOLoop *ioLoop, int fd, IOEvent ioEvent)
{
	if (ioEvent & IO_ERROR) {
		/* 严重错误 */
		ioLoop->remove(_listenFd);
		return ;
	}

	int clientFd = ::accept4(_listenFd, 0, 0, O_NONBLOCK);
	if (clientFd < 0) {
		/* 接受客户端请求连接失败 */
		return ;
	}

	_newConnectionListener->onNewConnection(ioLoop, clientFd);
}

