#pragma once
// 范数计算的实现
#include <cmath>
#include <algorithm>
#include <complex>
using namespace std;

template <typename T>
class VectorNorm {
public:
    static T pNorm(const vector<T>& vec, double p) {
        if (p <= 0) throw invalid_argument("p must be positive");
        T sum = 0;
        for (const auto& x : vec)
            sum += pow(abs(x), p);
        return pow(sum, 1.0/p);
    }

    static T infinityNorm(const vector<T>& vec) {
        T max = 0;
        for (const auto& x : vec)
            if (abs(x) > max) max = abs(x);
        return max;
    }
};

template <typename T>
class MatrixNorm {
public:
    static T frobeniusNorm(const Matrix<T>& mat) {
        T sum = 0;
        for (const auto& row : mat.data)
            for (const auto& x : row)
                sum += abs(x) * abs(x);
        return sqrt(sum);
    }
};