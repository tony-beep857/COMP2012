#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>

const double epsilon = 1e-9; // For floating-point comparison

template <typename T>
class Matrix {
    private:
        int rows;
        int cols;
        T* data;

    public:

        Matrix(int rows, int cols);
        Matrix(int rows, int cols, const T& fill);
        Matrix(const Matrix<T>& other);
        ~Matrix();

        Matrix<T>& operator=(const Matrix<T>& other);

        int size() const { return rows * cols; }
        int get_num_row() const { return rows; }
        int get_num_col() const { return cols; }

        T& operator()(int r, int c);
        const T& operator()(int r, int c) const;

        T* operator[](int r);
        const T* operator[](int r) const;

        Matrix<T>& operator+=(const Matrix<T>& other);
        Matrix<T>& operator*=(const T& scalar);
        Matrix<T> operator-() const;
};

template <typename T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs);

template <typename T>
Matrix<T> operator*(const Matrix<T>& matrix, const T& scalar);

template <typename T>
Matrix<T> operator*(const T& scalar, const Matrix<T>& matrix);

template <typename T>
bool operator==(const Matrix<T>& lhs, const Matrix<T>& rhs);

template <typename T>
bool operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs);

template <>
bool operator==(const Matrix<float>& lhs, const Matrix<float>& rhs);

template <>
bool operator==(const Matrix<double>& lhs, const Matrix<double>& rhs);

template <>
bool operator!=(const Matrix<float>& lhs, const Matrix<float>& rhs);

template <>
bool operator!=(const Matrix<double>& lhs, const Matrix<double>& rhs);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix);

#include "matrix.tpp"

#endif 
