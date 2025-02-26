#pragma once
#include "../utils/conversions.h"
#include "../drivers/vga_text.h"

#define PUSH_BS(val) \
    __asm__ volatile (        \
        "sub $4, %%esp\n\t"   \
        "movl %0, (%%esp)"     \
        :                      \
        : "r" (val)            \
        : "esp"               \
    )


#define POP_BS_AND_IGNORE()     \
    __asm__ volatile (          \
        "add $4, %%esp"         \
        :                       \
        :                       \
        : "esp"                 \
    )

#define PEEK_BS()       \
    do {                         \
        int val;                 \
        __asm__ volatile (        \
            "movl (%%esp), %0\n\t" \
            "add $4, %%esp"       \
            : "=r" (val)          \
            :                     \
            : "esp"               \
        );                        \
        kprint(tostring_inplace(val, 16));      \
        PUSH_BS(val); \
    } while (0)
