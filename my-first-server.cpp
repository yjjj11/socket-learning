#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<sys/socket.h>
#include<netinet/in.h>
#include<cstring>
#include"socket/socket.h"
using namespace yazi::socket;
using namespace yazi::utility;
using namespace std;

int main()
{
	Logger::instance()->open("./server.log");
    //1.创建socket
    Socket server;

    //2.绑定socket
    string ip = "127.0.0.1";
    int port = 8080;
    server.bind(ip,port);
    //3.创建监听函数
    server.listen(1024);
    while(1)
    {
        //4.等待接收客户端连接
        int confd =server.accept();
	if(confd<0)return 1;
	Socket client(confd);
        //5.接收客户端的数据
        char buf[1024] = {0};
        size_t len = client.recv(buf,sizeof(buf)); // 留一个字节给结束符
        
        if(len <= 0)
        {
            if(len < 0)
                printf("接收数据错误: error=%d errmsg=%s\n", errno, strerror(errno));
            else
                printf("客户端已断开连接\n");
            
            close(confd);
            continue;
        }
        
        // 添加字符串结束符
        buf[len] = '\0';
        // 输出时添加换行符，确保缓冲区刷新
        printf("recv: confd=%d msg=%s\n", confd, buf);

        //6.服务端向客户端发送数据
        client.send(buf,len);
        
   }

    //7.关闭socket（实际上while(1)永远不会执行到这里）
    server.close();
    debug("客户端已关闭");
    return 0;
}
    
