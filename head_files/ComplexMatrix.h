#pragma once
// 复矩阵的实现
#include <complex> //复数
#include "Matrix.h"
using namespace std;

template <typename T>
class ComplexMatrix : public Matrix<complex<T>> {
public:
    using Matrix<complex<T>>::Matrix;

    ComplexMatrix(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("File not found");

        int rows, cols;
        file >> rows >> cols;
        this->rows = rows;
        this->cols = cols;
        this->data.resize(rows, vector<complex<T>>(cols));

        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j) {
                string element;
                file >> element;
                
                // 解析 "a+bi" 格式
                size_t plus_pos = element.find('+');
                size_t i_pos = element.find('i');
                T real = stod(element.substr(0, plus_pos));
                T imag = stod(element.substr(plus_pos+1, i_pos - plus_pos -1));
                
                this->data[i][j] = complex<T>(real, imag);
            }
        file.close();
    }

    // 复数矩阵特有运算
    ComplexMatrix<T> conjugate() const {
        ComplexMatrix<T> result(this->rows, this->cols);
        for (int i = 0; i < this->rows; ++i)
            for (int j = 0; j < this->cols; ++j)
                result.data[i][j] = conj(this->data[i][j]);  //共轭复数
        return result;
    }

    // 重写秩
    int rank() const override {
        
    }

    // 重写文件保存方法
    void saveToFile(const string& filename) const override {
        ofstream file(filename);
        file << this->rows << " " << this->cols << "\n";
        for (const auto& row : this->data) {
            for (const auto& val : row)
                file << val.real() << "+" << val.imag() << "i ";
            file << "\n";
        }
    }

    void print() const override {
        for (const auto& row : this->data) {
            for (const auto& val : row)
                cout << val.real() << "+" << val.imag() << "i ";
            cout << "\n";
        }
    }
};