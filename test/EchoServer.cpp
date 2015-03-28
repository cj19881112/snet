#include "TcpServer.h"
#include "ConnectionHandler.h"
#include "ConnectionHandlerFactory.h"

#include <string>
#include <iostream>

class EchoHandler : public ConnectionHandler {
public:
	void handleData(Connection *c, char *buf, int len)
	{
		c->sendData(buf, len);
	}

	virtual void handleClose(Connection *c)
	{
		std::cout << "client close!" << std::endl;
	}

	virtual void handleError(Connection *c)
	{
		std::cout << "error client!" << std::endl;
	}
};

class EchoHandlerFactory : public ConnectionHandlerFactory {
public:
	virtual ConnectionHandler *createConnectionHandler()
	{
		return new EchoHandler;
	}
};


int main()
{
	TcpServer echoServer;
	if (echoServer.init("0.0.0.0", 10000, new EchoHandlerFactory) != -1) {
		echoServer.start();
	} else {
		std::cout << "init error!" << std::endl;
	}
}

