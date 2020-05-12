#ifndef __WEB_SERVER_H__
#define __WEB_SERVER_H__

#include "TcpListener.h"

#include <string>
#include <vector>

#define HTML_BEGIN "<html>\r\n<body>\r\n"
#define HTML_END "</body>\r\n</html>"

class WebServer : public TcpListener
{
public:

	WebServer(const char* ipAddress, int port) :
		TcpListener(ipAddress, port) { }

protected:

	void onMessageReceived(int clientSocket, const char* msg, int length);

private:

	static size_t calculateSum(const std::string& content);

	static void parseEndpoint(const std::string& endpoint, std::vector<std::string>& files);

};

#endif // __WEB_SERVER_H__