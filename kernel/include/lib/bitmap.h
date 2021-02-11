#pragma once

#include <stdint.h>

struct Bitmap1;
struct Bitmap2;
struct Bitmap3;

typedef struct Bitmap1 Bitmap1;
typedef struct Bitmap2 Bitmap2;
typedef struct Bitmap3 Bitmap3;

struct Bitmap1
{
    uint32_t b1[32];
    uint32_t a;
};
struct Bitmap2
{
    Bitmap1 b2[32];
    uint32_t a;
};
struct Bitmap3
{
    Bitmap2 b2[32];
    uint32_t a;
};
