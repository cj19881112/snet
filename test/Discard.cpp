#include "TcpServer.h"
#include "ConnectionHandler.h"
#include "ConnectionHandlerFactory.h"

#include <string>
#include <iostream>

class DiscardHandler : public ConnectionHandler {
	void handleData(Connection *c, char *buf, int len)
	{
		std::cout << std::string(buf, len) << std::endl;
	}
};

class DiscardHandlerFactory : public ConnectionHandlerFactory {
public:
	virtual ConnectionHandler *createConnectionHandler()
	{
		return new DiscardHandler;
	}
};


int main()
{
	TcpServer discardServer;
	if (discardServer.init("0.0.0.0", 10000, new DiscardHandlerFactory) != -1) {
		discardServer.start();
	} else {
		std::cout << "init error!" << std::endl;
	}
}

