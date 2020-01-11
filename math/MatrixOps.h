//
// Created by Akarsh Kumar on 2019-11-08.
//

#pragma once

#include "Matrix.h"
#include "Vector.h"
#include "VectorOps.h"

namespace AKMath {

    template<int N, class D>
    Matrix <N, N, D> identityMatrix() {
        Matrix<N, N, D> ans(0);
        for (int n = 0; n < N; n++) {
            ans.at(n).at(n) = 1;
        }
        return ans;
    }

    template<int M, int N, class D>
    Vector <M, D> matrixMultiply(const Matrix <M, N, D> &matrix, const Vector <N, D> &vector) {
        Vector<M, D> ans;
        for (int m = 0; m < M; m++) {
            ans.set(m, dot(matrix[m], vector));
        }
        return ans;
    }

    template<int M, int N, int P, class D>
    Matrix <M, P, D> matrixMultiply(const Matrix <M, N, D> &matrix1, const Matrix <N, P, D> &matrix2) {
        Matrix<M, P, D> ans;
        for (int m = 0; m < M; m++) {
            for (int p = 0; p < M; p++) {
                D val = 0;
                for (int n = 0; n < N; n++) {
                    val += matrix1.get(m, n) * matrix2.get(n, p);
                }
                ans.set(m, p, val);
            }
        }
        return ans;
    }


}