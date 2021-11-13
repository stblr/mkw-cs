#pragma once

typedef int BOOL;

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

struct Patch {
    u8 *from;
    u8 *to;
};

#define REPLACE(func) \
    extern u8 func; \
    extern u8 my_ ## func; \
    __attribute__((section("patches"))) \
    extern const struct Patch func ## _patch = { \
        &func, \
        &my_ ## func, \
    }
