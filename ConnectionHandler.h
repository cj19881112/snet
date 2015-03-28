#ifndef _CONNECTION_HANDLER_H_
#define _CONNECTION_HANDLER_H_

class Connection;

class ConnectionHandler {
public:
	virtual void reset() {}
	virtual void handleError(Connection *c) {}
	virtual void handleClose(Connection *c) {}
	virtual void handleData(Connection *c, char *buf, int len){}
	virtual void handleSendComplete(Connection *c) {}
};

#endif
