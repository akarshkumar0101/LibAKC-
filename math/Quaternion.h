//
// Created by Akarsh Kumar on 2019-11-15.
//

#pragma once

#include <iostream>
#include "Vector.h"

namespace AKMath {
    class Quaternion {
    private:
        double mRealPart;
        Vector3D mVectorPart;

    public:
        Quaternion():mRealPart(0.0), mVectorPart(){}
        Quaternion(double realPart, double i, double j, double k): mRealPart(realPart), mVectorPart({i, j, k}) {}
        Quaternion(double realPart, Vector3D vectorPart) : mRealPart(realPart), mVectorPart(vectorPart) {}
        Quaternion(const Vector3D& vector) : mRealPart(0.0), mVectorPart(vector){}
        Quaternion(Vector3D axis, double angle) : mRealPart(cos(angle / 2)), mVectorPart(length(axis, sin(angle / 2))) {}
        Quaternion(const Quaternion& quaternion) : mRealPart(quaternion.mRealPart), mVectorPart(quaternion.mVectorPart){}

        inline double& realPart() const {
            return (double&) mRealPart;
        }
        inline Vector3D& vectorPart() const {
            return (Vector3D&) mVectorPart;
        }

        Quaternion& operator=(const Quaternion& b){
            mRealPart = b.mRealPart;
            mVectorPart = b.mVectorPart;
            return *this;
        }
    };

}



#include "QuaternionOps.h"
