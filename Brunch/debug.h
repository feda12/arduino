#ifndef DEBUG

// #define __DEBUG

#ifdef __DEBUG

#define DEBUG(a)    Serial.print(a)
#define DEBUGLN(a)  Serial.println(a)

#else

#define DEBUG(a)    (0)
#define DEBUGLN(a)  (0)

#endif

#endif