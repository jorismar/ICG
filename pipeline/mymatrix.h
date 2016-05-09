#ifndef MY_MATRIX_H
#define MY_MATRIX_H

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

class MyMatrix {
    private:
        double**      matrix;
        unsigned int n_lines;
        unsigned int n_columns;
        
    public:
        MyMatrix();
        MyMatrix(unsigned int num_lin, unsigned int num_col);
        virtual ~MyMatrix();
        
        void setValue(unsigned int lin, unsigned int col, double value);
        double getValue(unsigned int lin, unsigned int col);
        
        void setValues(double values[]);
        
        unsigned int numLines();
        unsigned int numColumns();
        
        void loadIdentity();
        
        void mult(MyMatrix& m1, MyMatrix& m2);
        void div(MyMatrix& m1, double scalar);

        void print();
};

#endif