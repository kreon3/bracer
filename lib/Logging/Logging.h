#ifndef LIB_LOGGING_H_
#define LIB_LOGGING_H_
////////////////////////////////////////////////////////////////////////////////
// Logging

#define DEBUG 1
#define VERBOSE 2
#define INFO 3
#define WARNING 4
#define ERROR 5
#define NONE 255

#define LOG_LEVEL INFO

#if LOG_LEVEL <= DEBUG
#define LOG_DEBUG_FUNC(FUNC, ...) FUNC(__VA_ARGS__);
#else
#define LOG_DEBUG_FUNC(FUNC, ...)
#endif

#if LOG_LEVEL <= VERBOSE
#define LOG_VERBOSE_FUNC(FUNC, ...) FUNC(__VA_ARGS__);
#else
#define LOG_VERBOSE_FUNC(FUNC, ...)
#endif

#if LOG_LEVEL <= INFO
#define LOG_INFO_FUNC(FUNC, ...) FUNC(__VA_ARGS__);
#else
#define LOG_INFO_FUNC(FUNC, ...)
#endif

#if LOG_LEVEL <= WARNING
#define LOG_WARNING_FUNC(FUNC, ...) FUNC(__VA_ARGS__);
#else
#define LOG_WARNING_FUNC(FUNC, ...)
#endif

#if LOG_LEVEL <= ERROR
#define LOG_ERROR_FUNC(FUNC, ...) FUNC(__VA_ARGS__);
#else
#define LOG_ERROR_FUNC(FUNC, ...)
#endif

#if LOG_LEVEL <= NONE
#define LOG_NONE_FUNC(FUNC, ...) FUNC(__VA_ARGS__);
#else
#define LOG_NONE_FUNC(FUNC, ...)
#endif

#define LOG(LEVEL, MSG) \
  LOG_##LEVEL##_FUNC(Serial.print, MSG)

#define LOG_SPEC(LEVEL, MSG, SPEC) \
  LOG_##LEVEL##_FUNC(Serial.print, MSG, SPEC)

#define LOGLN(LEVEL, MSG) \
  LOG_##LEVEL##_FUNC(Serial.println, MSG)

#define LOGLN_SPEC(LEVEL, MSG, SPEC) \
  LOG_##LEVEL##_FUNC(Serial.println, MSG, SPEC)



#endif // LIB_LOGGING_H_
