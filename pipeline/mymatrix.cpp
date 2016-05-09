#include "mymatrix.h"

MyMatrix::MyMatrix() {
    this->n_lines   = 3;
    this->n_columns = 3;
    this->matrix    = new double*[this->n_lines];
    
    for(int i = 0; i < this->n_lines; i++)
        this->matrix[i] = new double[this->n_columns];
}

MyMatrix::MyMatrix(unsigned int num_lin, unsigned int num_col) {
    this->n_lines   = num_lin;
    this->n_columns = num_col;
    this->matrix    = new double*[this->n_lines];
    
    for(int i = 0; i < this->n_lines; i++)
        this->matrix[i] = new double[this->n_columns];
}

MyMatrix::~MyMatrix() {
    for(int i = 0; i < this->n_lines; i++)
        delete[] this->matrix[i];

    delete[] this->matrix;
}

void MyMatrix::setValue(unsigned int lin, unsigned int col, double value) {
    if(this->n_lines <= lin && this->n_columns <= col)
        throw std::out_of_range("Out of range");

    this->matrix[lin][col] = value;
}

double MyMatrix::getValue(unsigned int lin, unsigned int col) {
    if(this->n_lines <= lin && this->n_columns <= col)
        throw std::out_of_range("Out of range");

    return this->matrix[lin][col];
}

void MyMatrix::setValues(double values[]) {
    size_t size = this->n_columns * sizeof(double);
    
    for(int l = 0; l < this->n_lines; l++)
        memcpy(this->matrix[l], values + (l * this->n_columns), size);
}

unsigned int MyMatrix::numLines() {
    return this->n_lines;
}

unsigned int MyMatrix::numColumns() {
    return this->n_columns;
}

void MyMatrix::loadIdentity() {
    if(this->n_lines != this->n_columns)
        throw std::overflow_error("invalid matrix dimension.");
    
    size_t size = sizeof(double) * this->n_columns;
    
    for(int l = 0; l < this->n_lines; l++) {
        memset(this->matrix[l], 0, size);
        this->matrix[l][l] = 1;
    }
}

void MyMatrix::mult(MyMatrix& m1, MyMatrix& m2) {
    if(m1.numColumns() != m2.numLines() || this->n_lines != m2.numLines() || this->n_columns != m2.numColumns()) {
        throw std::overflow_error("not compatible dimensions matrices.");
    }
    
    double res[this->n_lines * this->n_columns];
    
    for(int l = 0; l < this->n_lines; l++) {
        for(int c = 0; c < this->n_columns; c++) {
            res[this->n_columns * l + c] = 0.0;
            
            for(int cl = 0; cl < m1.numColumns(); cl++) {
                res[this->n_columns * l + c] += m1.getValue(l, cl) * m2.getValue(cl, c);
            }
        }
    }
    
    this->setValues(res);
}

void MyMatrix::div(MyMatrix& m1, double scalar) {
    if(scalar == 0.0)
        throw std::overflow_error("division by zero not allowed.");

    for(int l = 0; l < this->n_lines; l++)
        for(int c = 0; c < this->n_columns; c++)
            this->setValue(l, c, m1.getValue(l, c) / scalar);
}

void MyMatrix::print() {
    for(int l = 0; l < this->n_lines; l++) {
        for(int c = 0; c < this->n_columns; c++)
            std::cout << " " << this->matrix[l][c];
            
        std::cout << std::endl;
    }

    std::cout << std::endl;
}













