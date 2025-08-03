#include<socket.h>
using namespace yazi::utility;
using namespace yazi::socket;
using namespace std;
int main()
{
	Logger::instance()->open("./client.log");
	Logger::instance()->max(1024);
	//1.创建socket
        Socket client;
	
	//2.连接服务端
	string ip="127.0.0.1";
	int port=8080;
        client.connect(ip,port);

	//3.向服务端发送数据
	string data="hello world";
        client.send(data.c_str(),data.size());

	//4.接收服务端返还的数据
	char buf[1024]={0};
	client.recv(buf,sizeof(buf));
	printf("收到的内容是：%s\n",buf);

	//5.close the socket
	client.close();
	return 0;
}
