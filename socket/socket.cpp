#include "socket.h"
#include <errno.h>
#include <arpa/inet.h>  // 确保包含inet_addr等函数的头文件

using namespace yazi::socket;

// 构造函数：创建socket
Socket::Socket() : m_ip(""), m_port(0), m_sockfd(0) {
    // 调用系统socket函数（加::前缀）
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0) {
        error("create socket error: errno=%d errmsg=%s", errno, strerror(errno));
    } else {
        debug("create socket success! sockfd=%d", m_sockfd);
    }
}

// 带参数构造函数：使用已有的sockfd
Socket::Socket(int sockfd) : m_ip(""), m_port(0), m_sockfd(sockfd) {}

// 析构函数：关闭socket
Socket::~Socket() {
    close();  // 调用类的close成员函数
}

// 绑定IP和端口（成员函数）
bool Socket::bind(const std::string &ip, int port) {
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;  // 修正协议族拼写（原AF_INETi错误）

    // 处理IP地址（空IP表示绑定所有网卡）
    if (ip.empty()) {
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    }
    sockaddr.sin_port = htons(port);  // 端口转换为网络字节序

    // 调用系统bind函数（加::前缀）
    if (::bind(m_sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        error("socket bind error: errno=%d, errmsg=%s", errno, strerror(errno));
        return false;
    } else {
        debug("socket bind success! ip=%s, port=%d", ip.c_str(), port);
    }

    m_ip = ip;
    m_port = port;
    return true;
}

// 监听连接（成员函数）
bool Socket::listen(int backlog) {
    // 调用系统listen函数（加::前缀）
    if (::listen(m_sockfd, backlog) < 0) {
        error("socket listen error: errno=%d, errmsg=%s", errno, strerror(errno));
        return false;
    } else {
        debug("socket listen success! backlog=%d", backlog);
    }
    return true;
}

// 发起连接（成员函数）
bool Socket::connect(const std::string &ip, int port) {
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;  // 协议族（原AF_INETi错误）

    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());  // 目标IP
    sockaddr.sin_port = htons(port);                  // 目标端口

    // 调用系统connect函数（加::前缀）
    if (::connect(m_sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        error("socket connect error: errno=%d, errmsg=%s", errno, strerror(errno));
        return false;
    } else {
        debug("socket connect success! ip=%s, port=%d", ip.c_str(), port);
    }

    m_ip = ip;
    m_port = port;
    return true;
}

// 接受连接（成员函数）
int Socket::accept() {
    // 调用系统accept函数（加::前缀）
    int confd = ::accept(m_sockfd, nullptr, nullptr);
    if (confd < 0) {
        error("socket accept error: errno=%d, errmsg=%s", errno, strerror(errno));
        return -1;  // 修正返回值：用-1表示错误（原返回false错误，因返回值为int）
    } else {
        debug("socket accept success! confd=%d", confd);
    }
    return confd;
}

// 发送数据（成员函数）
int Socket::send(const char* buf, int len) {
    // 调用系统send函数（加::前缀）
    return ::send(m_sockfd, buf, len, 0);
}

// 接收数据（成员函数）
int Socket::recv(char* buf, int len) {
    // 调用系统recv函数（加::前缀）
    return ::recv(m_sockfd, buf, len, 0);
}

// 关闭socket（成员函数）
void Socket::close() {
    if (m_sockfd > 0) {
        ::close(m_sockfd);  // 调用系统close函数（加::前缀）
        m_sockfd = 0;
        debug("socket closed");
    }
}
bool Socket::set_non_blocking()
{
	int flags=fcntl(m_sockfd,F_GETFL,0);
	if(flags<0)
	{
		error("socket set_non_blocking failed   error=%d,errmsg=%s",errno,strerror(errno));
		return false;
	}
	flags|=O_NONBLOCK;
	if(fcntl(m_sockfd,F_SETFL,flags)<0)
	{

		error("socket set_non_blocking failed   error=%d,errmsg=%s",errno,strerror(errno));
		return false;
	}
	return true;
}
bool Socket::set_send_buffer(int size)
{
	int buff_size=size;
	if(setsockopt(m_sockfd,SOL_SOCKET,SO_SNDBUF,&buff_size,sizeof(buff_size))<0)
	{
		error("socket set_send_buffer failed   error=%d,errmsg=%s",errno,strerror(errno));
		return false;
	}
	return true;
}
bool Socket::set_recv_buffer(int size)
{
	int buff_size=size;
	if(setsockopt(m_sockfd,SOL_SOCKET,SO_RCVBUF,&buff_size,sizeof(buff_size))<0)
	{
		error("socket set_recv_buffer failed   error=%d,errmsg=%s",errno,strerror(errno));
		
		return false;
	}

}
bool Socket::set_linger(bool active,int seconds)
{
	struct linger l;
	memset(&l,0,sizeof(l));

	l.l_onoff=active?1:0;
	l.l_linger=seconds;
	if(setsockopt(m_sockfd,SOL_SOCK,SO_LINGER,&l,sizeof(l))<0)
	{

		error("socket set_linger failed   error=%d,errmsg=%s",errno,strerror(errno));
		return false;
	}
	return 1;
}
bool Socket::set_keepalive()
{
	int flag=1;
	if(setsockopt(m_sockfd,SOL_SOCKET,SO_KEEPALIVE,&flag,sizeof(flag))<0)
	{
	
		error("socket set_keepalive failed   error=%d,errmsg=%s",errno,strerror(errno));
		return false;
	}
	return 1;
}
bool Socket::set_reuseaddr()
{
	
	int flag=1;
	if(setsockopt(m_sockfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag))<0)
	{
	
		error("socket set_reuseaddr failed   error=%d,errmsg=%s",errno,strerror(errno));
		return false;
	}
	return 1;
}
