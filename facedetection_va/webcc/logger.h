#ifndef WEBCC_LOGGER_H_
#define WEBCC_LOGGER_H_

// This file was generated from "config.h.in" by CMake.
#include "webcc/config.h"

#if WEBCC_ENABLE_LOG

#include <cstring>  // for strrchr()
#include <string>

// Log levels.
#define WEBCC_VERB 0  // Similar to DEBUG in other projects.
#define WEBCC_INFO 1
#define WEBCC_WARN 2
#define WEBCC_ERRO 3
#define WEBCC_FATA 4

// Default log level.
#ifndef WEBCC_LOG_LEVEL
#define WEBCC_LOG_LEVEL WEBCC_WARN
#endif

#define WEBCC_LOG_FILE_NAME "webcc.log"

namespace webcc {

enum LogMode {
  LOG_FILE        = 1,  // Log to file.
  LOG_CONSOLE     = 2,  // Log to console.
  LOG_FLUSH       = 4,  // Flush on each log.
  LOG_OVERWRITE   = 8,  // Overwrite any existing log file.
};

// Commonly used modes.
const int LOG_CONSOLE_FILE_APPEND = LOG_CONSOLE | LOG_FILE;
const int LOG_CONSOLE_FILE_OVERWRITE = LOG_CONSOLE | LOG_FILE | LOG_OVERWRITE;
const int LOG_FILE_OVERWRITE = LOG_FILE | LOG_OVERWRITE;

// Initialize logger.
// If |dir| is empty, log file will be generated in current directory.
void LogInit(const std::string& dir, int modes);

void Log(int level, const char* file, int line, const char* format, ...);

}  // namespace webcc

// Initialize the logger with a level.
#define WEBCC_LOG_INIT(dir, modes) webcc::LogInit(dir, modes);

#if (defined(WIN32) || defined(_WIN64))

// See: https://stackoverflow.com/a/8488201
// ISSUE: The last path separator of __FILE__ in a header file becomes "/"
//        instead of "\". The result is that __FILENAME__ will contain a
//        prefix of "webcc/". So don't log from a header file!
#define __FILENAME__ std::strrchr("\\" __FILE__, '\\') + 1

#if WEBCC_LOG_LEVEL <= WEBCC_VERB
#define LOG_VERB(format, ...) \
    webcc::Log(WEBCC_VERB, __FILENAME__, __LINE__, format, ##__VA_ARGS__);
#else
#define LOG_VERB(format, ...)
#endif

#if WEBCC_LOG_LEVEL <= WEBCC_INFO
#define LOG_INFO(format, ...) \
    webcc::Log(WEBCC_INFO, __FILENAME__, __LINE__, format, ##__VA_ARGS__);
#else
#define LOG_INFO(format, ...)
#endif

#if WEBCC_LOG_LEVEL <= WEBCC_WARN
#define LOG_WARN(format, ...) \
    webcc::Log(WEBCC_WARN, __FILENAME__, __LINE__, format, ##__VA_ARGS__);
#else
#define LOG_WARN(format, ...)
#endif

#if WEBCC_LOG_LEVEL <= WEBCC_ERRO
#define LOG_ERRO(format, ...) \
    webcc::Log(WEBCC_ERRO, __FILENAME__, __LINE__, format, ##__VA_ARGS__);
#else
#define LOG_ERRO(format, ...)
#endif

#if WEBCC_LOG_LEVEL <= WEBCC_FATA
#define LOG_FATA(format, ...) \
    webcc::Log(WEBCC_FATA, __FILENAME__, __LINE__, format, ##__VA_ARGS__);
#else
#define LOG_FATA(format, ...)
#endif

#else

// See: https://stackoverflow.com/a/8488201
#define __FILENAME__ std::strrchr("/" __FILE__, '/') + 1

#if WEBCC_LOG_LEVEL <= WEBCC_VERB
#define LOG_VERB(format, args...) \
    webcc::Log(WEBCC_VERB, __FILENAME__, __LINE__, format, ##args);
#else
#define LOG_VERB(format, args...)
#endif

#if WEBCC_LOG_LEVEL <= WEBCC_INFO
#define LOG_INFO(format, args...) \
    webcc::Log(WEBCC_INFO, __FILENAME__, __LINE__, format, ##args);
#else
#define LOG_INFO(format, args...)
#endif

#if WEBCC_LOG_LEVEL <= WEBCC_WARN
#define LOG_WARN(format, args...) \
    webcc::Log(WEBCC_WARN, __FILENAME__, __LINE__, format, ##args);
#else
#define LOG_WARN(format, args...)
#endif

#if WEBCC_LOG_LEVEL <= WEBCC_ERRO
#define LOG_ERRO(format, args...) \
    webcc::Log(WEBCC_ERRO, __FILENAME__, __LINE__, format, ##args);
#else
#define LOG_ERRO(format, args...)
#endif

#if WEBCC_LOG_LEVEL <= WEBCC_FATA
#define LOG_FATA(format, args...) \
    webcc::Log(WEBCC_FATA, __FILENAME__, __LINE__, format, ##args);
#else
#define LOG_FATA(format, args...)
#endif

#endif  // defined(WIN32) || defined(_WIN64)

#else  // WEBCC_ENABLE_LOG == 0

#define WEBCC_LOG_INIT(dir, modes)

#if (defined(WIN32) || defined(_WIN64))
#define LOG_VERB(format, ...)
#define LOG_INFO(format, ...)
#define LOG_WARN(format, ...)
#define LOG_ERRO(format, ...)
#define LOG_FATA(format, ...)
#else
#define LOG_VERB(format, args...)
#define LOG_INFO(format, args...)
#define LOG_WARN(format, args...)
#define LOG_ERRO(format, args...)
#define LOG_FATA(format, args...)
#endif  // defined(WIN32) || defined(_WIN64)

#endif  // WEBCC_ENABLE_LOG

#endif  // WEBCC_LOGGER_H_
