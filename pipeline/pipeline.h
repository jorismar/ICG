#ifndef GRAPHIC_PIPELINE_H
#define GRAPHIC_PIPELINE_H

#include <cmath>
#include "mymatrix.h"
#include "types.h"

typedef MyMatrix MyVector;

double norm(MyMatrix * mtx);
void cross(MyMatrix * v1, MyMatrix * v2, MyMatrix * res);
void myglInit();
void myglScale(double x, double y, double z);
void myglRotate(double angle, double x, double y, double z);
void myglTranslate(double dx, double dy, double dz);
void myglLookAt(double pos_x, double pos_y, double pos_z, double lookat_x, double lookat_y, double lookat_z, double up_x, double up_y, double up_z);
void myglViewport(int x, int y, size_t width, size_t height);
void myglViewPlaneDistance(double dist);
Point myglPipeline(double x, double y, double z);

//**************************************************************************

MyMatrix g_model(4, 4);
MyMatrix g_view(4, 4);
MyMatrix g_projection(4, 4);
MyMatrix g_viewport(4, 4);

double d;

double norm(MyVector * vec) {
    return sqrt(
                vec->getValue(0, 0) * vec->getValue(0, 0) +
                vec->getValue(1, 0) * vec->getValue(1, 0) +
                vec->getValue(2, 0) * vec->getValue(2, 0)
    );
}

void cross(MyVector * v1, MyVector * v2, MyVector * res) {
    res->setValue(0, 0, v1->getValue(1, 0) * v2->getValue(2, 0) - v2->getValue(1, 0) * v1->getValue(2, 0));
    res->setValue(1, 0, v1->getValue(2, 0) * v2->getValue(0, 0) - v2->getValue(2, 0) * v1->getValue(0, 0));
    res->setValue(2, 0, v1->getValue(0, 0) * v2->getValue(1, 0) - v2->getValue(0, 0) * v1->getValue(1, 0));
}

//**************************************************************************

void myglInit() {
    g_model.loadIdentity();
    g_view.loadIdentity();
    g_projection.loadIdentity();
    g_viewport.loadIdentity();
    
    myglViewPlaneDistance(1);    
}

//**************************************************************************

void myglScale(double x, double y, double z) {
    MyMatrix scale(4, 4);
    
    scale.loadIdentity();
    
    scale.setValue(0, 0, x);
    scale.setValue(1, 1, y);
    scale.setValue(2, 2, z);
    
    g_model.mult(g_model, scale);
}

//**************************************************************************

void myglRotate(double angle, double x, double y, double z) {
    MyMatrix rotate(4, 4);
    
    rotate.loadIdentity();
    
    if(x > 0.0) {
        rotate.setValue(1, 1, cos(angle));
        rotate.setValue(1, 2, -sin(angle));
        rotate.setValue(2, 1, sin(angle));
        rotate.setValue(2, 2, cos(angle));
    }
    
    if(y > 0.0) {
        rotate.setValue(0, 0, cos(angle));
        rotate.setValue(0, 2, sin(angle));
        rotate.setValue(2, 0, -sin(angle));
        rotate.setValue(2, 2, cos(angle));
    }
    
    if(z > 0.0) {
        rotate.setValue(0, 0, cos(angle));
        rotate.setValue(0, 1, -sin(angle));
        rotate.setValue(1, 0, sin(angle));
        rotate.setValue(1, 1, cos(angle));
    }
    
    g_model.mult(g_model, rotate);
}

//**************************************************************************

void myglTranslate(double dx, double dy, double dz) {
    MyMatrix translate(4, 4);
    
    translate.loadIdentity();
    
    translate.setValue(0, 3, dx);
    translate.setValue(1, 3, dy);
    translate.setValue(2, 3, dz);
    
    g_model.mult(g_model, translate);
}

//**************************************************************************

void myglLookAt(double pos_x, double pos_y, double pos_z, double lookat_x, double lookat_y, double lookat_z, double up_x, double up_y, double up_z) {
    MyVector x_cam(3, 1);
    MyVector y_cam(3, 1);
    MyVector z_cam(3, 1);

    MyMatrix Bt(4, 4);
    MyMatrix T(4, 4);
    
    MyVector aux(3, 1);
    MyVector up(3, 1);

    double vec_pl[] = {pos_x - lookat_x, pos_y - lookat_y, pos_z - lookat_z};
    double vec_up[]  = {up_x, up_y, up_z};
    
    aux.setValues(vec_pl);
    up.setValues(vec_up);
    
    z_cam.div(aux, norm(&aux));

    cross(&up, &z_cam, &aux);
    
    x_cam.div(aux, norm(&aux));
    
    cross(&z_cam, &x_cam, &aux);
    y_cam.div(aux, norm(&aux));
    
    double bt_mtx[] = {
        x_cam.getValue(0, 0), x_cam.getValue(1, 0), x_cam.getValue(2, 0), 0,
        y_cam.getValue(0, 0), y_cam.getValue(1, 0), y_cam.getValue(2, 0), 0,
        z_cam.getValue(0, 0), z_cam.getValue(1, 0), z_cam.getValue(2, 0), 0,
                0,                    0,                    0,            1
    };
    
    Bt.setValues(bt_mtx);
    
    T.loadIdentity();
    T.setValue(0, 3, -pos_x);
    T.setValue(1, 3, -pos_y);
    T.setValue(2, 3, -pos_z);
    
    g_view.loadIdentity();

    g_view.mult(Bt, T);
}

//**************************************************************************

void myglViewport(int x, int y, size_t width, size_t height) {
    MyMatrix S1(4, 4);
    MyMatrix S2(4, 4);
    MyMatrix T(4, 4);

    S1.loadIdentity();
    S2.loadIdentity();
    T.loadIdentity();
    
    S1.setValue(1, 1, -1);
    
    S2.setValue(0, 0, (width - 1) / 2.0f);
    S2.setValue(1, 1, (height - 1) / 2.0f);
    
    T.setValue(0, 3, 1);
    T.setValue(1, 3, 1);
    
    g_viewport.loadIdentity();
    
    g_viewport.mult(T, S1);
    g_viewport.mult(S2, g_viewport);
}

//**************************************************************************

void myglViewPlaneDistance(double dist) {
    d = dist;
    
    g_projection.setValue(2, 3, d);
    g_projection.setValue(3, 2, (-1) / d);
    g_projection.setValue(3, 3, 0);
}

//**************************************************************************

Point myglPipeline(double x, double y, double z) {
    double vert[] = {x, y, z, 1};
    MyMatrix vertice(4, 1);

    vertice.setValues(vert);

    vertice.mult(g_model, vertice);

    vertice.mult(g_view, vertice);

    vertice.mult(g_projection, vertice);

    vertice.div(vertice, vertice.getValue(3, 0));

    vertice.mult(g_viewport, vertice);

    Point p = {.x = round(vertice.getValue(0, 0)), .y = round(vertice.getValue(1, 0))};
    
    return p;
}

#endif








