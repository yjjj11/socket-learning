all: libsocket.so

# 编译动态库的关键参数：-fPIC创建位置无关代码，-shared创建共享库
libsocket.so: socket.cpp
	g++ -fPIC -shared -o libsocket.so socket.cpp -I../utility -L../utility -lLogger

clean:
	rm -f libsocket.so

