#pragma once
// 线性方程组求解
#include "Matrix.h"
using namespace std;

template <typename T>
class LinearSolver {
public:
    static vector<T> solveAXB(const Matrix<T>& A, const vector<T>& b) {
        if (A.getRows() != A.getCols() || A.getRows() != b.size())

        Matrix<T> augmented(A.getRows(), A.getCols() + 1);
        
        // 原代码：augmented.data[i][j] = A.data[i][j];
        // 修改为：
        augmented(i, j) = A(i, j);
        vector<T> x(b.size());

        // 构造增广矩阵
        for (int i = 0; i < A.rows; ++i) {
            for (int j = 0; j < A.cols; ++j)
                augmented.data[i][j] = A.data[i][j];
            augmented.data[i][A.cols] = b[i];
        }

        // 前向消元
        for (int k = 0; k < A.rows; ++k) {
            // 选取主元...（需添加部分交换逻辑）
            for (int i = k + 1; i < A.rows; ++i) {
                T factor = augmented.data[i][k] / augmented.data[k][k];
                for (int j = k; j <= A.rows; ++j)
                    augmented.data[i][j] -= factor * augmented.data[k][j];
            }
        }

        // 回代求解
        for (int i = A.rows - 1; i >= 0; --i) {
            x[i] = augmented.data[i][A.rows];
            for (int j = i + 1; j < A.rows; ++j)
                x[i] -= augmented.data[i][j] * x[j];
            x[i] /= augmented.data[i][i];
        }

        return x;
    }
};