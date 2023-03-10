#include "logger.h"

#include <iostream>
#include <time.h>

// 获取日志的单例
Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

Logger::Logger()
{
    // 启动写日志线程，把日志队列中的信息写到日志文件中
    std::thread writeLogTask([&](){
        for(;;)
        {
            // 获取当前的日期，然后取日志信息，写入相应的日志文件中
            time_t now = time(nullptr);
            tm* nowtm = localtime(&now);

            char file_name[128];
            sprintf(file_name, "%d-%d-%d-log.txt", 
                    nowtm->tm_year + 1900,
                    nowtm->tm_mon + 1,
                    nowtm->tm_mday);
            FILE* pf = fopen(file_name, "a+"); // a+ ==>追加
            if(nullptr == pf) 
            {
                std::cout << "logger file: " << file_name << " open error!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string msg = m_lckQue.Pop();
            char time_buf[128] = { 0 };
            sprintf(time_buf, "%d:%d:%d =>[%s] ", 
                    nowtm->tm_hour, 
                    nowtm->tm_min, 
                    nowtm->tm_sec,
                    m_loglevel == INFO ? "info" : "error");
            msg.insert(0, time_buf);
            msg.append("\n");
        
            fputs(msg.c_str(), pf);
            fclose(pf);

        }
    });
    // 设置分离线程
    writeLogTask.detach();
}

// 设置日志级别
void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level;
}

// 写日志(写到日志缓冲队列中)
void Logger::Log(std::string msg)
{
    m_lckQue.Push(msg);
}
