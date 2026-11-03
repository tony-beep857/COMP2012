# Matrix Template Class

## Overview
This project provides a robust, template-based `Matrix` class in C++ for performing common linear algebra and matrix operations. Because it is implemented as a template class, it seamlessly supports various data types, including integers (`int`), single-precision floats (`float`), and double-precision floats (`double`).

## Features
Based on the provided test cases in `main.cpp`, the `Matrix` class supports the following features:

*   **Flexible Construction:** Create matrices with specific dimensions (rows, columns), initialize them with default scalar values, or copy existing matrices using the copy constructor.
*   **Element Access & Modification:** Read and modify matrix elements using both the `(row, col)` operator and the standard array subscript `[][]` operators.
*   **Assignment:** Safely assign one matrix to another, including built-in protection against self-assignment.
*   **Arithmetic Operations:**
    *   **Addition:** Add two matrices of the same dimensions using `+` or `+=`.
    *   **Negation:** Negate a matrix using the unary `-` operator.
    *   **Scalar Multiplication:** Multiply a matrix by a scalar value using `*` or `*=` (supports both `matrix * scalar` and `scalar * matrix`).
*   **Comparison Operators:** Test matrices for equality (`==`) and inequality (`!=`), including safe comparisons for floating-point data types.
*   **Output Stream:** Easily print matrix contents to standard output using the overloaded `<<` operator.

## Testing and Usage
The included `main.cpp` file acts as a comprehensive test suite for the `Matrix` class. It prompts the user for a test case number (1-8) and executes the corresponding demonstration:

1.  **Matrix construction and copy:** Tests size initialization, default value population, and copy construction.
2.  **Element access and modification:** Tests data manipulation using `mat(r, c)` and `mat[r][c]`.
3.  **Assignment operator:** Tests assigning matrices of different sizes and self-assignment (`a = a`).
4.  **Addition and compound assignment:** Tests `a + b` and `a += b`.
5.  **Negation operator:** Tests the unary `-` operator for flipping signs of all elements.
6.  **Scalar multiplication:** Tests multiplying matrices by scalar floating-point values.
7.  **Addition of non-square matrices:** Verifies that arithmetic operations work correctly on rectangular matrices (e.g., 2x3).
8.  **Equality and inequality:** Tests `==` and `!=` operators on `float` and `double` matrices.

## Compilation and Execution
To compile and run the test suite, use a standard C++ compiler. For example, using GCC:

```bash
g++ -std=c++11 main.cpp -o matrix_test
./matrix_test
```
Upon running, simply enter a number from `1` to `8` to execute the corresponding test case.