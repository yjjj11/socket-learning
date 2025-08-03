#include<iostream>
#include"Logger.h"
using namespace std;
using namespace yazi::utility;
int main()
{
	Logger::instance()->open("./test.log");//用日志单例打开文件
	//Logger::instance()->log(Logger::DEBUG,__FILE__,__LINE__,"hello world");
	Logger::instance()->level(Logger::DEBUG); //设置当前文档的日
	Logger::instance()->max(200);//设置最大日志行署
	debug("hello world");//用宏来简化log函数
	debug("name is %s,age is %d","jack",18);
	error("error message");
	info("info message");
	warn("warn message");
	fatal("fatal message");
	return 0;
}
