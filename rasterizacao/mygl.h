#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************
#include <vector>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <math.h>

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

void PutPixel(Pixel pixel) {
    if(!(pixel.coord.x < 0 || pixel.coord.y < 0 || pixel.coord.x > IMAGE_WIDTH || pixel.coord.y > IMAGE_HEIGHT)) { // não permitir ultrapassar os limites tirar print sem essa proteção

        unsigned int pos = (pixel.coord.y * IMAGE_WIDTH + pixel.coord.x) * 4;
        
        FBptr[pos]     = pixel.color.r;
        FBptr[pos + 1] = pixel.color.g;
        FBptr[pos + 2] = pixel.color.b;
        FBptr[pos + 3] = pixel.color.a;
    }
}

void DrawLine(Pixel pointA, Pixel pointB) {
    std::vector<Pixel> line;

    Pixel p = {.coord = {.x = pointA.coord.x, .y = pointA.coord.y}, .color = {.r = pointA.color.r, .g = pointA.color.g, .b = pointA.color.b, .a = pointA.color.a}};

    int x = pointA.coord.x, y = pointA.coord.y;
    int * axis1 = &x;
    int * axis2 = &y;

    int last_x, last_y;
    
    if(pointB.coord.x < pointA.coord.x)
        last_x = (pointA.coord.x - pointB.coord.x) + pointA.coord.x;
    else last_x = pointB.coord.x;
    
    if(pointB.coord.y < pointA.coord.y)
        last_y = (pointA.coord.y - pointB.coord.y) + pointA.coord.y;
    else last_y = pointB.coord.y;
    
    int last_d = last_x;
    int dx = last_x - pointA.coord.x;
    int dy = last_y - pointA.coord.y;

    if(dx < dy) {
        int tmp = dx;
        dx = dy;
        dy = tmp;
        
        last_d = last_y;
        
        axis1 = &y;
        axis2 = &x;
    }
    
    int d = 2 * dy - dx;
    int incr_e = 2 * dy;
    int incr_ne = 2 * (dy - dx);

    line.push_back(p);

    while (*axis1 < last_d) {
        if (d <= 0) {
            d += incr_e;
            *axis1 += 1;
        } else {
            d += incr_ne;
            *axis1 += 1;
            *axis2 += 1;
        }
        
        p.coord.x = x;
        p.coord.y = y;
    
        if(last_x != pointB.coord.x)
            p.coord.x = pointA.coord.x - (p.coord.x - pointA.coord.x);
        
        if(last_y != pointB.coord.y)
            p.coord.y = pointA.coord.y - (p.coord.y - pointA.coord.y);

        line.push_back(p);
    }

    double incr_color[] = {
        (double) (pointB.color.r - pointA.color.r) / (line.size() - 1),
        (double) (pointB.color.g - pointA.color.g) / (line.size() - 1),
        (double) (pointB.color.b - pointA.color.b) / (line.size() - 1),
        (double) (pointB.color.a - pointA.color.a) / (line.size() - 1)
    };
    
    double color[] = {pointA.color.r, pointA.color.g, pointA.color.b, pointA.color.a};
    
    for(int i = 0; i < line.size(); i++) {
        line.at(i).color.r = round(color[0]);
        line.at(i).color.g = round(color[1]);
        line.at(i).color.b = round(color[2]);
        line.at(i).color.a = round(color[3]);
        
        PutPixel(line.at(i));

        color[0] += incr_color[0];
        color[1] += incr_color[1];
        color[2] += incr_color[2];
        color[3] += incr_color[3];
    }
}

void DrawTriangle(Pixel pointA, Pixel pointB, Pixel pointC) {
    DrawLine(pointA, pointB);
    DrawLine(pointB, pointC);
    DrawLine(pointC, pointA);
}

#endif // _MYGL_H_
