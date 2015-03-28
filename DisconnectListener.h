#ifndef _DISCONNECT_LISTENER_
#define _DISCONNECT_LISTENER_

class Connection;

class DisconnectListener {
public:
	virtual void disconnect(Connection *c)= 0;
};

#endif

