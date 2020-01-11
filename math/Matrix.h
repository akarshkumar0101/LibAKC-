//
// Created by Akarsh Kumar on 2019-11-08.
//

#pragma once


#include "Vector.h"


namespace AKMath {

    template<int M, int N, class D>
    class Matrix {
    private:
        Vector <N, D> mData[M];

    public:
        Matrix() {
        }

        Matrix(const D initData) {
            for (int m = 0; m < M; m++) {
                at(m) = Vector<N, D>(initData);
            }
        }

        inline Vector <N, D> &at(int m) const {
            return (Vector<N, D> &) (mData[m]);
        }

        inline Vector <N, D> &operator[](int m) const {
            return at(m);
        }

        inline D get(const int m, const int n) const {
            return at(m).at(n);
        }

        inline void set(const int m, const int n, const D value) {
            at(m).at(n) = value;
        }


        template<int M_, int N_, class D_>
        friend std::ostream &operator<<(std::ostream &out, const Matrix<M_, N_, D_> &vect);

    };

    template<int M, int N, class D>
    std::ostream &operator<<(std::ostream &out, const Matrix<M, N, D> &matrix) {
        out << '[';
        for (int m = 0; m < M; m++) {
            out << matrix.at(m);
            if (m < M - 1) {
                out << std::endl;
            }
        }
        out << ']';
        return out;
    }


}