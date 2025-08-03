all: libLogger.so main

main: main.cpp Logger.cpp Logger.h
	g++ -o main  main.cpp Logger.cpp

# 修复拼写错误 Loger.h → Logger.h
libLogger.so: Logger.h Logger.cpp
	g++ -fPIC -shared -o libLogger.so Logger.cpp

clean:
	 rm -f libLogger.a libLogger.so main
