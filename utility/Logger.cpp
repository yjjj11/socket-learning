#include "Logger.h"  // 补充#号
#include <stdarg.h>
#include <time.h>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <errno.h>  // 补充errno头文件

using namespace yazi::utility;
using namespace std;

// 静态成员初始化（必须在命名空间外或使用完整限定名）
const char* Logger::s_level[Logger::LEVEL_COUNT] = {
    "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

Logger* Logger::m_instance = nullptr;

Logger::Logger() : m_level(DEBUG), m_max(0), m_len(0) {}

Logger::~Logger() {
    close();
}

Logger* Logger::instance() {
    if (m_instance == nullptr) {
        m_instance = new Logger();
    }
    return m_instance;
}

void Logger::open(const string& file_name) {
    this->file_name = file_name;
    m_fout.open(file_name, ios::app);
    if (m_fout.fail()) {
        throw logic_error("open log file failed: " + file_name + ", error: " + strerror(errno));
    }
    cout << "文件打开成功: " << file_name << endl;
    m_fout.seekp(0, ios::end);
    m_len = m_fout.tellp();
}

void Logger::close() {
    if (m_fout.is_open()) {
        m_fout.close();
    }
}

void Logger::log(Level level, const char* file, int line, const char* format, ...) {
    if (level < m_level) {  // 修正逻辑判断（原判断反向，现已修正）
        return;
    }

    if (!m_fout.is_open()) {
        throw logic_error("log file not open, cannot write log");
    }

    time_t now = time(0);
    struct tm* ptn = localtime(&now);
    char timestamp[32] = {0};
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptn);  // 使用%S（秒）而非%s

    // 输出日志头部（时间、级别、文件、行号）
    const char* ftm = "%s [%s] %s:%d: ";
    int size = snprintf(nullptr, 0, ftm, timestamp, s_level[level], file, line);
    if (size > 0) {
        char* buffer = new char[size + 1];
        snprintf(buffer, size + 1, ftm, timestamp, s_level[level], file, line);
        m_fout << buffer;
        m_len += size;
        delete[] buffer;  // 使用delete[]释放数组
    }

    // 处理日志内容（可变参数）
    va_list arg_ptr;
    va_start(arg_ptr, format);
    size = vsnprintf(nullptr, 0, format, arg_ptr);
    va_end(arg_ptr);

    if (size > 0) {
        char* content = new char[size + 1];
        va_start(arg_ptr, format);
        vsnprintf(content, size + 1, format, arg_ptr);
        va_end(arg_ptr);
        m_fout << content << endl;
        m_len += size + 1;  // 加1是因为包含换行符
        delete[] content;
    }

    m_fout.flush();

    // 检查是否需要日志轮转
    if (m_max > 0 && m_len >= m_max) {
        rotate();
    }
}

void Logger::rotate() {
    close();

    // 生成带时间戳的备份文件名
    time_t now = time(0);
    struct tm* ptm = localtime(&now);
    char timestamp[32] = {0};
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", ptm);  // 修正为%S
    string backup_name = file_name + "." + timestamp;  // 备份文件格式：原文件名.时间戳

    // 重命名原文件为备份文件
    if (rename(file_name.c_str(), backup_name.c_str()) != 0) {
        throw logic_error("rename log file failed: " + string(strerror(errno)) + 
                         ", from: " + file_name + ", to: " + backup_name);
    }

    // 重新打开原文件（会创建新的空文件）
    open(file_name);
}

