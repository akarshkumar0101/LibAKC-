//
// Created by Akarsh Kumar on 11/20/19.
//
#pragma once

#include "Vector.h"


namespace AKMath {

    Quaternion add(const Quaternion &a, const Quaternion &b) {
        return Quaternion(a.realPart() + b.realPart(), a.vectorPart() + b.vectorPart());
    }
    Quaternion operator+(const Quaternion &a, const Quaternion &b) {
        return add(a, b);
    }
    Quaternion& operator+=(Quaternion &a, const Quaternion &b){
        return a = add(a,b);
    }

    Quaternion sub(const Quaternion &a, const Quaternion &b) {
        return Quaternion(a.realPart() - b.realPart(), a.vectorPart() - b.vectorPart());
    }
    Quaternion operator-(const Quaternion &a, const Quaternion &b) {
        return sub(a, b);
    }
    Quaternion& operator-=(Quaternion &a, const Quaternion &b){
        return a = sub(a,b);
    }

    Quaternion multiply(const Quaternion &a, const Quaternion &b) {
        const double &r1 = a.realPart(), &r2 = b.realPart();
        const Vector3D &v1 = a.vectorPart(), &v2 = b.vectorPart();
        return Quaternion(r1 * r2 - (v1 ^ v2), r1 * v2 + r2 * v1 + v1 * v2);
    }
    Quaternion inverse(const Quaternion &a) {
        double scalar = dot(a.vectorPart(), a.vectorPart());
        scalar += a.realPart() * a.realPart();
        scalar = 1 / scalar;
        return Quaternion(a.realPart() * scalar, a.vectorPart() * -scalar);
    }
    Quaternion operator*(const Quaternion &a, const Quaternion &b) {
        return multiply(a, b);
    }
    Quaternion& operator*=(Quaternion &a, const Quaternion &b){
        return a = multiply(a,b);
    }
    Quaternion operator/(const Quaternion &a, const Quaternion &b) {
        return multiply(a, inverse(b));
    }
    Quaternion& operator/=(Quaternion &a, const Quaternion &b){
        return a = multiply(a, inverse(b));
    }

    double dot(const Quaternion& a, const Quaternion &b) {
        return a.realPart()*b.realPart()+dot(a.vectorPart(),b.vectorPart());
    }
    inline double operator^(const Quaternion &a, const Quaternion &b) {
        return dot(a, b);
    }

    template<class Scalar>
    Quaternion scale(const Quaternion &q, const Scalar scalar) {
        return Quaternion(q.realPart()*scalar, q.vectorPart()*scalar);
    }
    template< class Scalar>
    inline Quaternion operator*(const Quaternion &a, Scalar scalar) {
        return scale(a, scalar);
    }

    template<class Scalar>
    inline Quaternion operator*(Scalar scalar, const Quaternion &a) {
        return scale(a, scalar);
    }
    template<class Scalar>
    Quaternion& operator*=(Quaternion &a, Scalar scalar){
        return a = scale(a,scalar);
    }
    template<class Scalar>
    Quaternion& operator/=(Quaternion &a, Scalar scalar){
        return a = scale(a,1.0/scalar);
    }

    double magnitudeSqr(const Quaternion &q) {
        return dot(q, q);
    }

    double magnitude(const Quaternion &q) {
        return std::sqrt(magnitudeSqr(q));
    }
    Quaternion length(const Quaternion &q, const double length) {
        double mag = magnitude(q);
        return scale(q, length / mag);
    }


    Quaternion normal(const Quaternion &q) {
        return length(q, 1.0);
    }

    double component(const Quaternion &q, const Quaternion &axis) {
        double ans = dot(q, axis) / magnitude(axis);
        return ans;
    }

    Quaternion project(const Quaternion &vector, const Quaternion &axis) {
        double comp = component(vector, axis);
        return length(axis,comp);
    }

    Quaternion rotate(const Quaternion &p, const Quaternion &q) {
        return multiply(q, multiply(p, inverse(q)));
    }

    Vector3D rotate(const Vector3D &p, const Quaternion &q) {
        return multiply(q, multiply(Quaternion(p), inverse(q))).vectorPart();
    }

    std::ostream &operator<<(std::ostream &out, const AKMath::Quaternion &q) {
        out << q.realPart() << '+' << q.vectorPart()[0] << "i+" << q.vectorPart()[1] << "j+" << q.vectorPart()[2]
            << "k";
        return out;
    }

}