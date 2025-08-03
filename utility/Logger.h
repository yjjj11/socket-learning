#pragma once
#include <string>
#include <fstream>
#include <cstdarg>  // 补充可变参数头文件（log函数需要）

namespace yazi {
namespace utility {

// 1. 先声明Logger类和必要的枚举
class Logger {
public:
    enum Level { DEBUG = 0, INFO, WARN, ERROR, FATAL, LEVEL_COUNT };

    static Logger* instance();
    void open(const std::string& file_name);
    void close();
    void log(Level level, const char* file, int line, const char* format, ...);
    void level(Level level) { m_level = level; }
    void max(int bytes) { m_max = bytes; }
    void rotate();

private:
    std::string file_name;
    std::ofstream m_fout;
    static const char* s_level[LEVEL_COUNT];
    Level m_level;
    Logger();
    ~Logger();
    static Logger* m_instance;
    int m_max;
    int m_len;
};

// 2. 在Logger类声明之后，再定义宏（此时编译器已认识Logger）
#define debug(format, ...) yazi::utility::Logger::instance()->log(yazi::utility::Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define info(format, ...)  yazi::utility::Logger::instance()->log(yazi::utility::Logger::INFO,  __FILE__, __LINE__, format, ##__VA_ARGS__)
#define warn(format, ...)  yazi::utility::Logger::instance()->log(yazi::utility::Logger::WARN,  __FILE__, __LINE__, format, ##__VA_ARGS__)
#define error(format, ...) yazi::utility::Logger::instance()->log(yazi::utility::Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define fatal(format, ...) yazi::utility::Logger::instance()->log(yazi::utility::Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

} // namespace utility
} // namespace yazi

