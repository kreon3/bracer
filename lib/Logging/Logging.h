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

#define LOG(LEVEL, MSG)                                                        \
  if (LEVEL >= LOG_LEVEL)                                                      \
    Serial.print(MSG);

#define LOG_SPEC(LEVEL, MSG, SPEC)                                             \
  if (LEVEL >= LOG_LEVEL)                                                      \
    Serial.print(MSG, SPEC);

#define LOGLN(LEVEL, MSG)                                                      \
  if (LEVEL >= LOG_LEVEL)                                                      \
    Serial.println(MSG);

#define LOGLN_SPEC(LEVEL, MSG, SPEC)                                           \
  if (LEVEL >= LOG_LEVEL)                                                      \
    Serial.println(MSG, SPEC);

#endif // LIB_LOGGING_H_
