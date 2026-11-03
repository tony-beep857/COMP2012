#include "matrix.h"

void run_test(int i) {
    switch (i) {
        case 1: { // Matrix construction and copy
            Matrix<int> mat1(2, 3);
            Matrix<int> mat2(2, 3, 5);
            Matrix<int> mat3(mat2);
            std::cout << "Matrix 1:\n" << mat1 << std::endl;
            std::cout << "Matrix 2:\n" << mat2 << std::endl;
            std::cout << "Matrix 3:\n" << mat3 << std::endl;
            break;
        }
        case 2: { // Element access and modification
            Matrix<int> mat(2, 3);
            mat(0, 1) = 2026;
            mat(1, 2) = 10;
            mat[1][0] = 24;
            std::cout << "mat(0,1) = " << mat(0, 1) << std::endl;
            std::cout << "mat[1][0] = " << mat[1][0] << std::endl;
            std::cout << "mat[1][2] = " << mat[1][2] << std::endl;
            std::cout << mat << std::endl;
            break;
        }
        case 3: { // Assignment operator and self-assignment
            Matrix<int> a(2, 2, 1);
            Matrix<int> b(3, 3, 2);
            a = b;
            std::cout << "After assignment:\n" << a << std::endl;
            a = a;
            std::cout << "After self-assignment:\n" << a << std::endl;
            break;
        }
        case 4: { // Addition operator and compound assignment
            Matrix<int> a(2, 2, 3);
            Matrix<int> b(2, 2, 4);
            std::cout << "a + b:\n" << (a + b) << std::endl;
            a += b;
            std::cout << "a after += b:\n" << a << std::endl;
            break;
        }
        case 5: { // Negation operator
            Matrix<int> a(2, 3);
            a(0, 0) = 1;
            a(0, 1) = -2;
            a(0, 2) = 3;
            a(1, 0) = -4;
            a(1, 1) = 5;
            a(1, 2) = -6;
            std::cout << "a:\n" << a << std::endl;
            std::cout << "-a:\n" << (-a) << std::endl;
            break;
        }
        case 6: { // Scalar multiplication and compound assignment
            Matrix<double> a(2, 2, 2.5);
            std::cout << "a * 2:\n" << (a * 2.0) << std::endl;
            std::cout << "3 * a:\n" << (3.0 * a) << std::endl;
            a *= 4.0;
            std::cout << "a after *= 4:\n" << a << std::endl;
            break;
        }
        case 7: { // Addition of non-square matrices
            Matrix<int> a(2, 3);
            Matrix<int> b(2, 3);
            for (int r = 0; r < 2; ++r) {
                for (int c = 0; c < 3; ++c) {
                    a(r, c) = r * 3 + c + 1;
                }
            }
            b(0, 0) = 6;
            b(0, 1) = 0;
            b(0, 2) = -1;
            b(1, 0) = 2;
            b(1, 1) = -3;
            b(1, 2) = 4;
            std::cout << "a:\n" << a << std::endl;
            std::cout << "b:\n" << b << std::endl;
            std::cout << "a + b:\n" << (a + b) << std::endl;
            break;
        }
        case 8: { // Equality and inequality operators for floating-point matrices
            Matrix<float> a(1, 1, 1.0f);
            Matrix<float> b(1, 1, 1.0f);
            std::cout << "float equal: " << (a == b) << std::endl;
            std::cout << "float not equal: " << (a != b) << std::endl;
            Matrix<double> c(1, 1, 1.0);
            Matrix<double> d(1, 1, 2.0);
            std::cout << "double equal: " << (c == d) << std::endl;
            std::cout << "double not equal: " << (c != d) << std::endl;
            break;
        }
        default:
            std::cout << "Invalid test case number." << std::endl;
    }
}

int main() {
    std::cout << "Please input the test case to run: ";
	int i;
	std::cin >> i;

    run_test(i);
}
