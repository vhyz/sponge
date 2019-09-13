#pragma once

#include <stdio.h>

namespace sponge {

enum LOG_LEVEL {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
};

extern LOG_LEVEL logLevel;
extern FILE* logFile;

void setLoggerFile(FILE* fp);
void setLogLevel(LOG_LEVEL level);

// private function
void log_base(const char* file, int line, const char* level, bool abort,
              const char* fmt, ...);

#define DEBUG(fmt, ...)              \
    if (logLevel <= LOG_LEVEL_DEBUG) \
    log_base(__FILE__, __LINE__, "DEBUG", false, fmt, ##__VA_ARGS__)
#define INFO(fmt, ...)              \
    if (logLevel <= LOG_LEVEL_INFO) \
    log_base(__FILE__, __LINE__, "INFO", false, fmt, ##__VA_ARGS__)
#define WARN(fmt, ...)              \
    if (logLevel <= LOG_LEVEL_WARN) \
    log_base(__FILE__, __LINE__, "WARN", false, fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...)              \
    if (logLevel <= LOG_LEVEL_ERROR) \
    log_base(__FILE__, __LINE__, "ERROR", false, fmt, ##__VA_ARGS__)
#define FATAL(fmt, ...)              \
    if (logLevel <= LOG_LEVEL_FATAL) \
    log_base(__FILE__, __LINE__, "FATAL", true, fmt, ##__VA_ARGS__)

}  // namespace sponge
