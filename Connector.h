#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "IOEventHandler.h"
#include "IOEvent.h"

class InetAddr;
class NewConnectionListener;
class IOLoop;

class Connector : public IOEventHandler {
public:
	Connector(NewConnectionListener *ncl=0)
		: _newConnectionListener(ncl)
	{}
	int connect(const InetAddr &addr, IOLoop *ioLoop);
	virtual void handleEvent(IOLoop *ioLoop, int fd, IOEvent ioEvent);

	void setNewConnectionListener(NewConnectionListener *ncl)
	{
		_newConnectionListener = ncl;
	}
	NewConnectionListener *getNewConnectionListener()
	{
		return _newConnectionListener;
	}
private:
	NewConnectionListener *_newConnectionListener;
};

#endif

