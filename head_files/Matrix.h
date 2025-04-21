#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <stdexcept>
#include <cmath>
#include <algorithm>
using namespace std;

template <typename T>
class Matrix {
protected:
    int rows, cols;

public:
    int getRows() const { return rows; }
    int getCols() const { return cols; }
    vector<vector<T>> data;

    // 一般构造函数
    Matrix(int r = 0, int c = 0, T val = T()) : rows(r), cols(c), 
        data(r, vector<T>(c, val)) {}

    // 文件构造函数
    Matrix(const string& filename) {
        ifstream file(filename);
        if (!file) throw runtime_error("File open failed");
        file >> rows >> cols;
        data.resize(rows, vector<T>(cols));
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                file >> data[i][j];
    }

    // 运算符重载
    Matrix<T> operator+(const Matrix<T>& other) const {
        if (rows != other.rows || cols != other.cols)
            throw invalid_argument("Dimension mismatch");
        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                result.data[i][j] = data[i][j] + other.data[i][j];
        return result;
    }

    Matrix<T> operator-(const Matrix<T>& other) const {
        if (rows!= other.rows || cols!= other.cols)
            throw invalid_argument("Dimension mismatch");
        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                 result.data[i][j] = data[i][j] - other.data[i][j];
        return result;
    }

    Matrix<T> operator*(const Matrix<T>& other) const {
        if (cols!= other.rows)
            throw invalid_argument("Dimension mismatch");
        Matrix<T> result(rows, other.cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < other.cols; ++j)
                for (int k = 0; k < cols; ++k)
                    result.data[i][j] += data[i][k] * other.data[k][j];
        return result;
    }

    // 保存矩阵文件
    virtual void saveToFile(const string& filename) const {
        ofstream file(filename);
        file << rows << " " << cols << "\n";
        for (const auto& row : data) {
            for (const auto& val : row)
                file << val << " ";
            file << "\n";
        }
    }

    // 矩阵转置
    Matrix<T> transpose() const {
        Matrix<T> result(cols, rows);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                result.data[j][i] = data[i][j];
        cout<<"转置成功！"<<endl;
        return result;
    }

    // 矩阵的秩
    virtual int rank() const {
        if (data.empty()) return 0;
        
        // 复制矩阵以避免修改原数据
        vector<vector<T>> mat = data;
        int rank = 0;
        const int n = rows, m = cols;
        const T eps = 1e-10;  // 处理浮点误差的阈值
    
        for (int col = 0, row = 0; col < m && row < n; ++col) {
            // 1. 找主元（当前列中绝对值最大的行）
            int pivot = row;
            for (int i = row + 1; i < n; ++i) {
                if (abs(mat[i][col]) > abs(mat[pivot][col])) {
                    pivot = i;
                }
            }
    
            // 如果主元接近0，跳过该列
            if (abs(mat[pivot][col]) < eps) continue;
    
            // 2. 交换当前行与主元行
            if (pivot != row) {
                swap(mat[row], mat[pivot]);
            }
    
            // 3. 消去下方行的当前列元素
            for (int i = row + 1; i < n; ++i) {
                T factor = mat[i][col] / mat[row][col];
                for (int j = col; j < m; ++j) {
                    mat[i][j] -= factor * mat[row][j];
                }
            }
    
            // 4. 增加秩计数
            ++row;
            ++rank;
        }

        return rank;
    }

    // 化为最简型
    void reducedRowEchelonForm() {
        const T eps = 1e-10;  // 浮点误差阈值
        int lead = 0;         // 当前主元列

        for (int r = 0; r < rows && lead < cols; ++r, ++lead) {
            // 1. 找主元行
            int i = r;
            while (i < rows && abs(data[i][lead]) < eps) ++i;

            if (i == rows) {  // 当前列全零
                --lead;        // 保持lead与r同步
                continue;
            }

            // 2. 交换当前行与主元行
            swap(data[r], data[i]);

            // 3. 主元归一化
            T pivot = data[r][lead];
            for (int j = lead; j < cols; ++j)
                data[r][j] /= pivot;

            // 4. 消去其他行（包括上方和下方）
            for (int k = 0; k < rows; ++k) {
                if (k != r && abs(data[k][lead]) > eps) {
                    T factor = data[k][lead];
                    for (int j = lead; j < cols; ++j)
                        data[k][j] -= factor * data[r][j];
                }
            }
        }
    }

    // 求逆矩阵
    Matrix<T> inverse() const {
        if (rows != cols) throw std::logic_error("Not a square matrix");
        const int n = rows;
        const T eps = 1e-10;
        T det = determinant();
        if (std::abs(det) < eps)
            throw std::runtime_error("Matrix is not invertible");

        // 构造增广矩阵 [A|I]
        Matrix<T> aug(n, 2*n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j)
                aug.data[i][j] = data[i][j];
            aug.data[i][n + i] = 1;  // 单位矩阵部分
        }

        // 高斯-约当消元
        for (int col = 0; col < n; ++col) {
            // 选主元（部分主元法）
            int pivot = col;
            for (int i = col + 1; i < n; ++i)
                if (std::abs(aug.data[i][col]) > std::abs(aug.data[pivot][col]))
                    pivot = i;

            if (std::abs(aug.data[pivot][col]) < eps)
                throw std::runtime_error("Matrix is singular");

            // 交换行
            if (pivot != col)
                std::swap(aug.data[col], aug.data[pivot]);

            // 主元归一化
            T pivot_val = aug.data[col][col];
            for (int j = col; j < 2*n; ++j)
                aug.data[col][j] /= pivot_val;

            // 消去其他行
            for (int i = 0; i < n; ++i) {
                if (i != col && std::abs(aug.data[i][col]) > eps) {
                    T factor = aug.data[i][col];
                    for (int j = col; j < 2*n; ++j)
                        aug.data[i][j] -= factor * aug.data[col][j];
                }
            }
        }

        // 提取逆矩阵部分
        Matrix<T> inv(n, n);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                inv.data[i][j] = aug.data[i][n + j];

        return inv;
    }

    // 求伴随矩阵
    Matrix<T> adjugate() const {
        if (rows != cols) throw std::logic_error("Not a square matrix");
        const int n = rows;
        Matrix<T> adj(n, n);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // 计算余子式
                Matrix<T> minor(n-1, n-1);
                for (int r = 0, mi = 0; r < n; ++r) {
                    if (r == i) continue;
                    for (int c = 0, mj = 0; c < n; ++c) {
                        if (c == j) continue;
                        minor.data[mi][mj++] = data[r][c];
                    }
                    ++mi;
                }
                // 代数余子式并转置
                adj.data[j][i] = ((i + j) % 2 ? -1 : 1) * minor.determinant();
            }
        }
        return adj;
    }

    // 行列式计算（递归法，实际应用建议用LU分解优化）
    T determinant() const {
        if (rows != cols) throw std::logic_error("Not a square matrix");
        const int n = rows;

        if (n == 1) return data[0][0];
        if (n == 2) return data[0][0]*data[1][1] - data[0][1]*data[1][0];

        T det = 0;
        for (int col = 0; col < n; ++col) {
            Matrix<T> minor(n-1, n-1);
            for (int r = 1; r < n; ++r) {
                for (int c = 0, mj = 0; c < n; ++c) {
                    if (c == col) continue;
                    minor.data[r-1][mj++] = data[r][c];
                }
            }
            det += (col % 2 ? -1 : 1) * data[0][col] * minor.determinant();
        }
        return det;
    }

    // 打印
    virtual void print() const {
        for (const auto& row : data) {
            for (const auto& val : row)
                cout << val << " ";
            cout << "\n";
        }
    }
};

