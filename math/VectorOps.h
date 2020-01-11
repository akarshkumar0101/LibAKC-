//
// Created by Akarsh Kumar on 2019-11-07.
//

#pragma once

#include <cmath>

namespace AKMath {

    template<int N, class D>
    Vector<N, D> add(const Vector<N, D> &a, const Vector<N, D> &b) {
        Vector<N,D> ans;
        for (int i = 0; i < N; i++) {
            ans.set(i, a.get(i) + b.get(i));
        }
        return ans;
    }
    template<int N, class D>
    inline Vector<N, D> operator+(const Vector<N, D> &a, const Vector<N, D> &b) {
        return add(a, b);
    }
    template<int N, class D>
    Vector<N,D>& operator+=(Vector<N, D> &a, const Vector<N, D> &b){
        return a = add(a,b);
    }

    template<int N, class D>
    Vector<N, D> sub(const Vector<N, D> &a, const Vector<N, D> &b) {
        Vector<N,D> ans;
        for (int i = 0; i < N; i++) {
            ans.set(i, a.get(i) - b.get(i));
        }
        return ans;
    }
    template<int N, class D>
    inline Vector<N, D> operator-(const Vector<N, D> &a, const Vector<N, D> &b) {
        return sub(a, b);
    }
    template<int N, class D>
    Vector<N,D>& operator-=(Vector<N, D> &a, const Vector<N, D> &b){
        return a = sub(a,b);
    }


    template<int N, class D>
    D dot(const Vector<N, D> &a, const Vector<N, D> &b) {
        D dt = 0;
        for (int i = 0; i < N; i++) {
            dt += a.get(i) * b.get(i);
        }
        return dt;
    }
    template<int N, class D>
    inline D operator^(const Vector<N, D> &a, const Vector<N, D> &b) {
        return dot(a, b);
    }

    template<class D>
    Vector<3, D> cross(const Vector<3, D> &a, const Vector<3, D> &b) {
        Vector<3, D> ans;
        ans[0] = a[1] * b[2] - a[2] * b[1];
        ans[1] = -(a[0] * b[2] - a[2] * b[0]);
        ans[2] = a[0] * b[1] - a[1] * b[0];
        return ans;
    }
    template<class D>
    inline Vector<3, D> operator*(const Vector<3, D> &a, const Vector<3, D> &b) {
        return cross(a, b);
    }
    template<class D>
    Vector<3,D>& operator*=(Vector<3, D> &a, const Vector<3, D> &b){
        return a = cross(a,b);
    }


    template<int N, class D>
    D magnitudeSqr(const Vector<N, D> &vector) {
        return dot(vector, vector);
    }

    template<int N, class D>
    D magnitude(const Vector<N, D> &vector) {
        return std::sqrt(magnitudeSqr(vector));
    }

    template<int N, class D, class Scalar>
    Vector<N, D> scale(const Vector<N, D> &srcVector, const Scalar scalar) {
        Vector<N,D> ans;
        for (int n = 0; n < N; n++) {
            ans.set(n, srcVector.get(n) * scalar);
        }
        return ans;
    }
    template<int N, class D, class Scalar>
    inline Vector<N, D> operator*(const Vector<N, D> &a, Scalar scalar) {
        return scale(a, scalar);
    }

    template<int N, class D, class Scalar>
    inline Vector<N, D> operator*(Scalar scalar, const Vector<N, D> &a) {
        return scale(a, scalar);
    }
    template<int N, class D, class Scalar>
    Vector<N,D>& operator*=(Vector<N, D> &a, Scalar scalar){
        return a = scale(a,scalar);
    }
    template<int N, class D, class Scalar>
    Vector<N,D>& operator/=(Vector<N, D> &a, Scalar scalar){
        return a = scale(a,1.0/scalar);
    }

    template<int N, class D>
    Vector<N, D> length(const Vector<N, D> &vector, const D length) {
        D mag = magnitude(vector);
        return scale(vector, length / mag);
    }

    template<int N, class D>
    Vector<N, D> normal(const Vector<N, D> &vector) {
        return length(vector, 1.0);
    }

    template<int N, class D>
    D component(const Vector<N, D> &vector, const Vector<N, D> &axis) {
        D ans = dot(vector, axis) / magnitude(axis);
        return ans;
    }

    template<int N, class D>
    Vector<N, D> project(const Vector<N, D> &vector, const Vector<N, D> &axis) {
        D comp = component(vector, axis);
        return length(axis,comp);
    }









}




