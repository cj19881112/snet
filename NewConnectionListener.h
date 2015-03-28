#ifndef _NEW_CONNECTION_LISTENER_H_
#define _NEW_CONNECTION_LISTENER_H_

class IOLoop;
class NewConnectionListener {
public:
	virtual void onNewConnection(IOLoop *ioLoop, int clientFd) = 0;
};

#endif


