#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include <iterator>
#include "WebServer.h"

void WebServer::onMessageReceived(int clientSocket, const char* msg, int length)
{
	// Client's request string e.g. GET /index.html HTTP/1.1
	std::istringstream iss(msg);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	bool isValidEndpoint = true;
	std::vector<std::string> files;
	int errorCode = 404;
	std::string content;

	if (parsed.size() >= 3 && parsed[0] == "GET")
	{
		WebServer::parseEndpoint(parsed[1].substr(1, parsed[1].size() - 1), files);
	}

	content = HTML_BEGIN + std::string("<table border = \"1\">\r\n");

	for (size_t i = 0; i < files.size() && true == isValidEndpoint; i++)
	{
		std::ifstream f(".\\files\\" + files[i] + ".txt");

		if (true == f.good())
		{
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			content += str;
			errorCode = 200;
		}
		else
		{
			isValidEndpoint = false;
		}

		f.close();
	}

	if (true == isValidEndpoint)
	{
		content += "</table>\r\n<p>" + std::to_string(WebServer::calculateSum(content)) + "</p>\r\n";
		content += HTML_END;
	}
	else
	{
		std::ifstream f(".\\files\\index.html");

		if (f.good())
		{
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			content = str;
			errorCode = 200;
		}

		f.close();
	}

	std::ostringstream oss;
	oss << "HTTP/1.1 " << errorCode << " OK\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: text/html\r\n";
	oss << "Content-Length: " << content.size() << "\r\n";
	oss << "\r\n";
	oss << content;

	std::string output = oss.str();
	int size = output.size() + 1;

	sendToClient(clientSocket, output.c_str(), size);
}

size_t WebServer::calculateSum(const std::string& content)
{
	// locals
	size_t sum{};
	int currBeginTdTagPos{};
	int currEndTdTagPos{};
	int tdIter{};
	std::string pricePos;

	do
	{
		currBeginTdTagPos = content.find("<td>", currBeginTdTagPos);
		currEndTdTagPos = content.find("</td>", currBeginTdTagPos);

		if (currBeginTdTagPos != std::string::npos)
		{
			if (currEndTdTagPos != std::string::npos && tdIter % 2 == 1)
			{
				pricePos = content.substr(currBeginTdTagPos + 4,
					currEndTdTagPos - currBeginTdTagPos - 4);
				sum += static_cast<size_t>(strtoul(pricePos.c_str(), 0, 0));
			}
		}
		else
		{
			break;
		}
		
		tdIter++;
		currBeginTdTagPos++;
	} while(true);

	return sum;
}

void WebServer::parseEndpoint(const std::string& endpoint, std::vector<std::string>& files)
{
	// locals
	int prevEndpointPos{};
	int currEndpointPos{};
	files.clear();

	while (currEndpointPos != std::string::npos)
	{
		prevEndpointPos = currEndpointPos;
		currEndpointPos = endpoint.find("_", currEndpointPos);

		if (currEndpointPos != std::string::npos)
		{
			files.push_back(std::move(endpoint.substr(prevEndpointPos, currEndpointPos - prevEndpointPos)));
			currEndpointPos++;
		}
		else
		{
			files.push_back(std::move(endpoint.substr(prevEndpointPos, endpoint.size() - prevEndpointPos)));
		}
	}
}