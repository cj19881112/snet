#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "IOEventHandler.h"
#include "IOEvent.h"

#include <string>

class IOLoop;
class DisconnectListener;
class ConnectionHandler;

class Connection : public IOEventHandler {
public:
	enum STATE { ST_CONNECTED, ST_CLOSED, ST_ERROR };
	const static int MAX_READ_BUF = 4096;

	void init(IOLoop *loop, int sockFd);

	void handleEvent(IOLoop *loop, IOEvent ioEvent);
	void handleRead();
	void handleWrite();
	void sendData(const char *buf, int len);

	void setDisconnectListener(DisconnectListener *disconnectListener)
	{
		_disconnectListener = disconnectListener;
	}
	DisconnectListener *getDisconnectListener()
	{
		return _disconnectListener;
	}
	void setConnectionHandler(ConnectionHandler *connectionHandler)
	{
		_connectionHandler = connectionHandler;
	}
	ConnectionHandler *getConnectionHandler()
	{
		return _connectionHandler;
	}
private:
	STATE _state;
	int _sockFd;
	DisconnectListener *_disconnectListener;
	ConnectionHandler *_connectionHandler;
	std::string _outputBuffer;
	IOLoop *_loop;
};

#endif

