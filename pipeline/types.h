#ifndef STRUCTS_TYPES_H
#define STRUCTS_TYPES_H

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int r;
    int g;
    int b;
    int a;
} Color;

typedef struct {
    Point coord;
    Color color;
} Pixel;

#endif