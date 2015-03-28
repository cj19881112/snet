#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#include "IOEvent.h"
#include "IOEventHandler.h"
#include "InetAddr.h"

class NewConnectionListener;

class Acceptor : public IOEventHandler {
public:
	Acceptor(int backlog=128);
	~Acceptor();

	int init(const InetAddr &addr, IOLoop *ioLoop);	
	virtual void handleEvent(IOLoop *ioLoop, IOEvent ioEvent);

	void setNewConnectionListener(NewConnectionListener *ncl)
	{
		_newConnectionListener = ncl;
	}
	NewConnectionListener *getNewConnectionListener()
	{
		return _newConnectionListener;
	}

private:
	int _listenFd;
	int _backlog;
	NewConnectionListener *_newConnectionListener;
};

#endif

