//
// Created by Akarsh Kumar on 2019-11-07.
//

#pragma once


#include <cstring>
#include <array>
#include <iostream>


/*
#define X_INDEX 0
#define Y_INDEX 1
#define Z_INDEX 2
#define W_INDEX 3
 */

namespace AKMath {
    template<int N, class D>
    class Vector {
    private:
        D mData[N];

    public:

        Vector() {
        }

        //not pass by reference because D is assumed to be a small value type
        Vector(const D initData) {
            for (int n = 0; n < N; n++) {
                mData[n] = initData;
            }
        }

        Vector(const D *data) {
            std::memcpy(this->mData, data, N * sizeof(D));
        }

        Vector(const std::array<D, N> &stdArray) : Vector(stdArray.data()) {}

        template<int M>
        Vector(const Vector<M, D> &vect) {
            std::memcpy(mData, vect.data(), std::min(N, M) * sizeof(D));
            std::memset(mData + std::min(N, M), 0, (std::max(N, M) - std::min(N, M)) * sizeof(N));
        }

        inline D *data() const {
            return (D *) mData;
        }

        inline D &at(const int n) const {
            return (D &) mData[n];
        }

        inline D &operator[](const int n) const {
            return at(n);
        }

        inline D get(const int n) const {
            return at(n);
        }

        inline void set(const int n, const D value) {
            at(n) = value;
        }


        static Vector<N, D> unitVector(const int n) {
            Vector<N, D> ans((D) 0);
            ans.set(n, 1);
            return ans;
        }
    };


    template<int N, class D>
    std::ostream &operator<<(std::ostream &out, const Vector<N, D> &vect) {
        out << '<';
        for (int i = 0; i < N; i++) {
            out << vect.get(i);
            if (i < N - 1) {
                out << ", ";
            }
        }
        out << '>';
        return out;
    }


    template<int N>
    using VectorND = Vector<N, double>;
    template<class D>
    using Vector2 = Vector<2, D>;
    template<class D>
    using Vector3 = Vector<3, D>;
    using Vector2D = Vector<2, double>;
    using Vector3D = Vector<3, double>;
    using Vector4D = Vector<4, double>;

    Vector3D i = Vector3D::unitVector(0);
    Vector3D j = Vector3D::unitVector(1);
    Vector3D k = Vector3D::unitVector(2);

}

#include "VectorOps.h"
