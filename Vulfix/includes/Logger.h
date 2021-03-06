//defines a Logger for vULFIX

#ifndef Logger_h
#define Logger_h

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define Vulfix_Log_NULLWarning(whatWasNULL, methodThatCalled, description) printf("Vulfix Warning: NULL %s || Method: \"%s\" || %s\n", whatWasNULL, methodThatCalled, description);
#define Vulfix_Log_NULLError(whatWasNULL, methodThatCalled, description) printf("Vulfix Error: NULL %s || Method: \"%s\" || %s\n", whatWasNULL, methodThatCalled, description);
#define Vulfix_Log_FailedObjectCreation(object, methodThatCalled, description) printf("Vulfix Error: Failed to create %s || Method: \"%s\" || %s\n", object, methodThatCalled, description);
#define Vulfix_Log_FailedObjectDestruction(object, methodThatCalled, description) printf("Vulfix Error: Failed to destory %s || Method: \"%s\" || %s\n", object, methodThatCalled, description);

#define LogMessage(message) printf("%s\n", message);

#ifdef VULFIX_RELEASE

#define LogWarning(code, message)
#define LogError(code, message)
#define LogFatalError(code, message) printf("Vulfix Fatal Error: %s || %s\n", code, message); getchar(); exit(1);

#define LogData(formate, ...)
#define LogFatalData(formate, ...) printf(formate, __VA_ARGS__); getchar(); exit(1);

#endif

#ifdef VULFIX_DIST

#define LogWarning(code, message)
#define LogError(code, message)
#define LogFatalError(code, message) exit(1);

#define LogData(formate, ...)
#define LogFatalData(formate, ...) printf(formate, __VA_ARGS__); exit(1);

#endif

#ifdef VULFIX_DEBUG

#define LogWarning(code, message) printf("Vulfix Warning : %s || %s\n", code, message);
#define LogError(code, message) printf("Vulfix Error : %s || %s\n", code, message);
#define LogFatalError(code, message) printf("Vulfix Fatal Error: %s || %s\n", code, message); getchar(); exit(1);

#define LogData(formate, ...) printf(formate, __VA_ARGS__);
#define LogFatalData(formate, ...) printf(formate, __VA_ARGS__); getchar(); exit(1);

#endif

#ifndef LogWarning(code, message)
#define LogWarning(code, message)
#endif
#ifndef LogError(code, message)
#define LogError(code, message)
#endif
#ifndef LogFatalError(code, message)
#define LogFatalError(code, message)
#endif
#ifndef LogData(formate, ...)
#define LogData(formate, ...)
#endif
#ifndef LogFatalData(formate, ...)
#define LogFatalData(formate, ...)
#endif

#endif