#ifndef _INET_ADDR_H_
#define _INET_ADDR_H_

#include <arpa/inet.h>
#include <netinet/in.h>

class InetAddr {
public:
	InetAddr(const char *ip, int port)
	{
		_addr.sin_family = AF_INET;
		_addr.sin_addr.s_addr = inet_addr(ip);
		_addr.sin_port = htons(port);
	}
	const struct sockaddr *addr() const
	{
		return (struct sockaddr*)&_addr;
	}
	socklen_t addrLen() const
	{
		return sizeof (struct sockaddr_in);
	}

private:
	struct sockaddr_in _addr;	
};
#endif


