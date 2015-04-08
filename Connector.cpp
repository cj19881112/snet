#include "Connector.h"
#include "NewConnectionListener.h"
#include "InetAddr.h"
#include "IOLoop.h"

#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include <cerrno>

int Connector::connect(const InetAddr &addr, IOLoop *ioLoop)
{
	int fd = socket(AF_INET, SOCK_STREAM|O_NONBLOCK, 0);
	if (fd < 0) {
		return -1;
	}

	int err = ::connect(fd, addr.addr(), addr.addrLen());
	if (err < 0) {
		if (EINPROGRESS != errno) {
			close(fd);
			return -1;
		}
	}

	if (err < 0) {
		if (ioLoop->add(fd, IO_WRITE, this)) {
			close(fd);
			return -1;
		}
	} else {
		_newConnectionListener->onNewConnection(ioLoop, fd);		
	}

	return 0;
}

void Connector::handleEvent(IOLoop *ioLoop, int fd, IOEvent ioEvent)
{
	ioLoop->remove(fd);

	if (IO_WRITE != ioEvent) {
		/* error */
		close(fd);
		return ;
	} 

	_newConnectionListener->onNewConnection(ioLoop, fd);
}




