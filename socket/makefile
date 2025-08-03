all: libsocket.so libserver_socket.so libclient_socket.so

# 生成依赖文件相关（可选，用于自动处理头文件依赖）
CPPFLAGS += -MMD -MP
-include $(OBJS:.o=.d)

# 编译动态库的关键参数：-fPIC创建位置无关代码，-shared创建共享库
libsocket.so: socket.cpp socket.h
	g++ -fPIC -shared -o libsocket.so socket.cpp -I../utility -L../utility -lLogger

libserver_socket.so: libsocket.so server_socket.cpp server_socket.h
	g++ -fPIC -shared -o libserver_socket.so server_socket.cpp -I./ -I../utility -L./ -L../utility -lsocket -lLogger

libclient_socket.so: libsocket.so client_socket.cpp client_socket.h
	g++ -fPIC -shared -o libclient_socket.so client_socket.cpp -I./ -I../utility -L./ -L../utility -lsocket -lLogger
clean:
	rm -f libsocket.so libserver_socket.so libclient_socket.so
	rm -f *.d  # 如果用了自动依赖生成，清理.d文件
