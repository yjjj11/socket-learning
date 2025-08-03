INCLUDEDIR=-I./socket -I./utility
LIBDIR=-L./socket -L./utility

# 告诉链接器在运行时也能找到动态库
LDFLAGS=-Wl,-rpath=./socket:./utility

all: my-first-server my-first-client

# 先编译子模块的动态库
submodules:
	make -C utility
	make -C socket
# 编译服务器
my-first-server: my-first-server.cpp submodules
	g++ -o my-first-server my-first-server.cpp $(INCLUDEDIR) $(LIBDIR) $(LDFLAGS) -lsocket -lLogger -lserver_socket

# 编译客户端
my-first-client: my-first-client.cpp submodules
	g++ -o my-first-client my-first-client.cpp $(INCLUDEDIR) $(LIBDIR) $(LDFLAGS) -lsocket -lLogger -lclient_socket

clean:
	rm -f my-first-client my-first-server
	make -C socket clean
	make -C utility clean

