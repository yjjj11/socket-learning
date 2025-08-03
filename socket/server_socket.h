#pragma once

#include"socket.h"

namespace yazi
{
namespace socket
{
	class ServerSocket:public Socket
	{
	public:
		ServerSocket()=delete;
		ServerSocket(const string &ip,int port);
		~ServerSocket()=default;
	};
	
}
}
