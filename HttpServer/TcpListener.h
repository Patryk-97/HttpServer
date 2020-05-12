#ifndef __TCP_LISTENER_H__
#define __TCP_LISTENER___

#include <WS2tcpip.h>

class TcpListener
{

public:

	TcpListener(const char* ipAddress, int port) :
		m_ipAddress(ipAddress), m_port(port) { }

	int init();
	int run();

protected:

	virtual void onMessageReceived(int clientSocket, const char* msg, int length);

	void sendToClient(int clientSocket, const char* msg, int length);

private:

	const char*		m_ipAddress;
	int				m_port;
	int				m_socket;
	fd_set			m_master;
};

#endif // __TCP_LISTENER___