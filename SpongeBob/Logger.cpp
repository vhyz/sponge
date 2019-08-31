#include "Logger.h"
#include <stdarg.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

namespace SpongeBob {

#ifndef NDEBUG
LOG_LEVEL logLevel = LOG_LEVEL_DEBUG;
#else
LOG_LEVEL logLevel = LOG_LEVEL_INFO;
#endif

FILE* logFile = stdout;

void setLoggerFile(FILE* fp) { logFile = fp; }

void setLogLevel(LOG_LEVEL level) { logLevel = level; }

void log_base(const char* file, int line, const char* level, bool abort,
              const char* fmt, ...) {
    static const size_t MAXLINE = 1024;
    char buf[MAXLINE + 1];

    time_t rawtime;
    tm timeinfo;
    time(&rawtime);
    localtime_r(&rawtime, &timeinfo);
    size_t pos = strftime(buf, MAXLINE, "%Y/%m%d %H:%M:%S", &timeinfo);

    pos += sprintf(buf + pos, " %s ", level);

    va_list vl;
    va_start(vl, fmt);
    pos += vsnprintf(buf + pos, MAXLINE - pos, fmt, vl);
    va_end(vl);

    pos += sprintf(buf + pos, " -%s:%d", file, line);

    fprintf(logFile, "%s\n", buf);

    if (abort) {
        exit(127);
    }
}

}  // namespace SpongeBob