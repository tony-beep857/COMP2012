#include <stdexcept>
#include <algorithm>

template <typename T>
Matrix<T>::Matrix(int rows, int cols) : rows(rows), cols(cols), data(nullptr) {
    if (rows < 0 || cols < 0) {
        throw std::invalid_argument("Matrix dimensions must be non-negative");
    }
    data = new T[static_cast<size_t>(rows) * cols]();
}

template <typename T>
Matrix<T>::Matrix(int rows, int cols, const T& fill) : rows(rows), cols(cols), data(nullptr) {
    if (rows < 0 || cols < 0) {
        throw std::invalid_argument("Matrix dimensions must be non-negative");
    }
    const int n = rows * cols;
    data = new T[n];
    std::fill(data, data + n, fill);
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& other) : rows(other.rows), cols(other.cols), data(nullptr) {
    const int n = rows * cols;
    data = new T[n];
    std::copy(other.data, other.data + n, data);
}

template <typename T>
Matrix<T>::~Matrix() {
    delete[] data;
}

template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
    if (this == &other) {
        return *this;
    }
    const int n = other.rows * other.cols;
    T* new_data = new T[n]; // allocate first for exception safety
    std::copy(other.data, other.data + n, new_data);
    delete[] data;
    data = new_data;
    rows = other.rows;
    cols = other.cols;
    return *this;
}

template <typename T>
T& Matrix<T>::operator()(int r, int c) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        throw std::out_of_range("Matrix index out of range");
    }
    return data[r * cols + c];
}

template <typename T>
const T& Matrix<T>::operator()(int r, int c) const {
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        throw std::out_of_range("Matrix index out of range");
    }
    return data[r * cols + c];
}

template <typename T>
T* Matrix<T>::operator[](int r) {
    if (r < 0 || r >= rows) {
        throw std::out_of_range("Matrix row index out of range");
    }
    return data + r * cols;
}

template <typename T>
const T* Matrix<T>::operator[](int r) const {
    if (r < 0 || r >= rows) {
        throw std::out_of_range("Matrix row index out of range");
    }
    return data + r * cols;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other) {
    if (rows != other.rows || cols != other.cols) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    const int n = rows * cols;
    for (int i = 0; i < n; ++i) {
        data[i] += other.data[i];
    }
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const T& scalar) {
    const int n = rows * cols;
    for (int i = 0; i < n; ++i) {
        data[i] *= scalar;
    }
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-() const {
    Matrix<T> result(rows, cols);
    const int n = rows * cols;
    for (int i = 0; i < n; ++i) {
        result.data[i] = -data[i];
    }
    return result;
}

template <typename T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    Matrix<T> result(lhs);
    result += rhs;
    return result;
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& matrix, const T& scalar) {
    Matrix<T> result(matrix);
    result *= scalar;
    return result;
}

template <typename T>
Matrix<T> operator*(const T& scalar, const Matrix<T>& matrix) {
    return matrix * scalar;
}

template <typename T>
bool operator==(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    if (lhs.get_num_row() != rhs.get_num_row() || lhs.get_num_col() != rhs.get_num_col()) {
        return false;
    }
    for (int r = 0; r < lhs.get_num_row(); ++r) {
        for (int c = 0; c < lhs.get_num_col(); ++c) {
            if (!(lhs(r, c) == rhs(r, c))) {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
bool operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs) {
    return !(lhs == rhs);
}

// Floating-point specializations: compare within epsilon.
// (Marked inline since this file is included from a header.)
template <>
inline bool operator==(const Matrix<float>& lhs, const Matrix<float>& rhs) {
    if (lhs.get_num_row() != rhs.get_num_row() || lhs.get_num_col() != rhs.get_num_col()) {
        return false;
    }
    for (int r = 0; r < lhs.get_num_row(); ++r) {
        for (int c = 0; c < lhs.get_num_col(); ++c) {
            if (std::fabs(static_cast<double>(lhs(r, c)) - static_cast<double>(rhs(r, c))) >= epsilon) {
                return false;
            }
        }
    }
    return true;
}

template <>
inline bool operator==(const Matrix<double>& lhs, const Matrix<double>& rhs) {
    if (lhs.get_num_row() != rhs.get_num_row() || lhs.get_num_col() != rhs.get_num_col()) {
        return false;
    }
    for (int r = 0; r < lhs.get_num_row(); ++r) {
        for (int c = 0; c < lhs.get_num_col(); ++c) {
            if (std::fabs(lhs(r, c) - rhs(r, c)) >= epsilon) {
                return false;
            }
        }
    }
    return true;
}

template <>
inline bool operator!=(const Matrix<float>& lhs, const Matrix<float>& rhs) {
    return !(lhs == rhs);
}

template <>
inline bool operator!=(const Matrix<double>& lhs, const Matrix<double>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix) {
    for (int r = 0; r < matrix.get_num_row(); ++r) {
        for (int c = 0; c < matrix.get_num_col(); ++c) {
            if (c > 0) os << ' ';
            os << matrix(r, c);
        }
        if (r + 1 < matrix.get_num_row()) os << '\n';
    }
    return os;
}
