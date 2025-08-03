#pragma once
#include<iostream>
#include<string>
#include<fcntl.h>
#include<arpa/inet.h>
#include<cstring>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include"Logger.h"
using std::string;
namespace yazi
{
	namespace socket
	{
		class Socket
		{
		public:
			Socket();
			virtual ~Socket();
			Socket(int sockfd);
			bool bind (const string &ip,int port);
			bool listen(int backlog);
			bool connect(const string &ip,int port);
			int accept();
			int send(const char*buf,int len);
			int recv(char* buf,int len);
			void close();

			bool set_non_blocking();
			bool set_send_buffer(int size);
			bool set_recv_buffer(int size);
			bool set_linger(bool active,int seconds);
			bool set_keepalive();
			bool set_reuseaddr();
		protected:
			string m_ip;
			int m_port;
			int m_sockfd;
		
		};
	}
}
