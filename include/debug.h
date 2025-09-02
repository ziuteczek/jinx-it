#ifndef DEBUG_H
#define DEBUG_H

#if DEBUG
#include <stdio.h>
#define debug(x, ...)                                                                                 \
    do                                                                                                \
    {                                                                                                 \
        fprintf(stderr, "%s:%s(%u):" x "\n", __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#else
#define debug(x, ...)
#endif

#endif