#include "WebServer.h"

#include <iostream>

int main()
{
	WebServer webServer("0.0.0.0", 8080);
	if (webServer.init() != 0)
	{
		return 1; 
	}
	else
	{
		std::cout << "Listening...\n";
		webServer.run();
	}

	std::cin.get();

	return 0;
}