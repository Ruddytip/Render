#pragma once
// #include <iostream>
#include <vector>

class Matrix {
    std::vector<std::vector<double>> data;
    int rows, cols;
public:
    Matrix(int r = 4, int c = 4); // 4 - Default alloc
    Matrix(std::vector<std::vector<double>> _data);
    int nrows();
    int ncols();
    static Matrix identity(int dimensions);
    Matrix operator*(const Matrix& a);
    Matrix transpose();
    std::vector<double>& operator[](const int i);
    // friend std::ostream& operator<<(std::ostream& s, Matrix& m);
};