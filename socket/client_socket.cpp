#include"client_socket.h"
using namespace yazi::socket;

ClientSocket::ClientSocket(const string& ip,int port):Socket()
{
	connect(ip,port);
}
