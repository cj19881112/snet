#ifndef _CONNECTION_HANDLER_FACTORY_H_
#define _CONNECTION_HANDLER_FACTORY_H_

class ConnectionHandler;

class ConnectionHandlerFactory {
public:
	virtual ConnectionHandler *createConnectionHandler() = 0;
};

#endif


