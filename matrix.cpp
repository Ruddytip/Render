#include "matrix.hpp"

Matrix::Matrix(int r, int c) : data(std::vector<std::vector<double>>(r, std::vector<double>(c, 0.f))), rows(r), cols(c) {}

Matrix::Matrix(std::vector<std::vector<double>> _data) : data(_data), rows(data.size()), cols(data[0].size()) {}

int Matrix::nrows() { return rows; }

int Matrix::ncols() { return cols; }

Matrix Matrix::identity(int dimensions) {
    Matrix E(dimensions, dimensions);
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {
            E[i][j] = (i == j ? 1.f : 0.f);
        }
    }
    return E;
}

std::vector<double>& Matrix::operator[](const int i) {
    return data[i];
}

Matrix Matrix::operator*(const Matrix& a) {
    Matrix result(rows, a.cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < a.cols; j++) {
            result.data[i][j] = 0.f;
            for (int k = 0; k < cols; k++) {
                result.data[i][j] += data[i][k] * a.data[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::transpose() {
    Matrix result(cols, rows);
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            result[j][i] = data[i][j];
    return result;
}

// std::ostream& operator<<(std::ostream& s, Matrix& m) {
//     for (int i = 0; i < m.nrows(); i++)  {
//         for (int j = 0; j < m.ncols(); j++) {
//             s << m[i][j];
//             if (j < m.ncols() - 1) s << "\t";
//         }
//         s << "\n";
//     }
//     return s;
// }