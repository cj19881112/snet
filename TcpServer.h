#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "NewConnectionListener.h"
#include "DisconnectListener.h"
#include "IOLoop.h"
#include "Connection.h"
#include "Acceptor.h"
#include "InetAddr.h"
#include "ConnectionHandlerFactory.h"

#include <deque>

#include <cstdlib>


class TcpServer : public NewConnectionListener, DisconnectListener {
public:
	int init(const char *ip, int port, ConnectionHandlerFactory *factory);
	void start()
	{
		_ioLoop.run();
	}
	void onNewConnection(IOLoop *ioLoop, int clientFd);
	void disconnect(Connection *c)
	{
		_connectionPool.push_back(c);
	}
	void setConnectionHandlerFactory(ConnectionHandlerFactory *factory) 
	{
		_factory = factory;
	}
	ConnectionHandlerFactory *getConnectionHandlerFactory()
	{
		return _factory;
	}

private:
	ConnectionHandlerFactory *_factory;
	std::deque<Connection*> _connectionPool;
	IOLoop _ioLoop;
	Acceptor _acceptor;
};

#endif

