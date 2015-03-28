#include "TcpServer.h"
#include <cassert>
#include <signal.h>

static class Sigign {
public:
	Sigign()
	{
		signal(SIGPIPE, SIG_IGN);
	}
} _sigign;

int TcpServer::init(const char *ip, int port, ConnectionHandlerFactory *factory)
{
	if (_ioLoop.init() < 0) {
		return -1;
	}
	if (_acceptor.init(InetAddr(ip, port), &_ioLoop) < 0) {
		return -1;
	}
	_acceptor.setNewConnectionListener(this);
	_factory = factory;
	return 0;
}

void TcpServer::onNewConnection(IOLoop *ioLoop, int clientFd)
{
	Connection *c = 0;
	if (!_connectionPool.empty()) {
		c = _connectionPool.front();	
		_connectionPool.pop_front();
	} else {
		c = new Connection;
		if (!c) {
			/* not enought memory */
			return ;
		}
		assert(_factory && "factory not set!");
		c->setConnectionHandler(_factory->createConnectionHandler());
		c->setDisconnectListener(this);
	}
	c->init(&_ioLoop, clientFd);
	c->handleEvent(0,0);
}

