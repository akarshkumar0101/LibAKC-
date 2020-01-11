//
//  HVector.h
//  LibAKC++
//
//  Created by Akarsh Kumar on 5/20/19.
//  Copyright © 2019 Akarsh Kumar. All rights reserved.
//

#ifndef HVector_h
#define HVector_h

#include "Vector.h"


template<class T>
class HVector: public AbstractVector<T> {
private:
#define VECTOR_X_INDEX 0
#define VECTOR_Y_INDEX 1
#define VECTOR_Z_INDEX 2
#define VECTOR_W_INDEX 3
    int N;
    T *data;
    
public:
    HVector(): N(0), data(nullptr) {
    }
    HVector(const int N): N(N), data(new T[N]) {
    }
    HVector(const int N, const T* vector): N(N), data(new T[N]){
        std::memcpy(this->data, vector, sizeof(T)*getN());
    }
    template <int N>
    HVector(const std::array<T, N>& vector): N(N), data(new T[N]){
        for(int i=0; i < N; i++){
            set(i, vector[i]);
        }
    }
    HVector(const HVector& vector): N(vector.getN()), data(new T[N]){
        std::memcpy(this->data, vector.data, sizeof(T)*getN());
    }
    void operator =(const HVector& vector) {
        if(vector.getN() != getN()) {
            delete[] this->data;
            this->N = vector.getN();
            this->data = new T[N];
        }
        std::memcpy(this->data, vector.data, sizeof(T)*getN());
    }
    ~HVector(){
        delete[] data;
        data = nullptr;
    }
    
    /*
     template <int... A>
     Vector(A... as) {
     auto values = {as...};
     std::cout<<values.size()<<std::endl;
     }
     */
    
    int getN() const {
        return N;
    }
    void setN(int N){
        this->N = N;
        delete[] data;
        data = new T[N];
    }
    void set(const int& n, const T& t) {
        data[n] = t;
    }
    const T& get(const int& n) const {
        return data[n];
    }
    
    const T& x() const {
        return get(VECTOR_X_INDEX);
    }
    const T& y() const {
        return get(VECTOR_Y_INDEX);
    }
    const T& z() const {
        return get(VECTOR_Z_INDEX);
    }
    const T& w() {
        return get(VECTOR_W_INDEX);
    }
    
    template<class T2, class DotType>
    const DotType dot(const HVector<T2>& another) const {
        DotType ret = 0;
        for(int i=0; i < N;i++){
            ret += get(i) * another.get(i);
        }
        return ret;
    }
    template<class DotType>
    const DotType dot(const HVector<T>& another){
        return dot<T,DotType>(another);
    }
    const T dot(const HVector<T>& another){
        return dot<T>(another);
    }
    
    template<class LengthType>
    const LengthType length() {
        LengthType length = 0;
        length = this->dot<T,LengthType>(*this);
        return sqrt(length);
    }
    const T length() {
        return length<T>();
    }
    
    const Vector<T,3> cross(const Vector<T, 3>& b) {
        return cross(*this, b);
    }
    
    
};

template<class T>
const HVector<T> cross(const HVector<T>& a, const HVector<T>& b) {
    return HVector<T>(a.y()*b.z()-a.z()*b.y(), a.z()*b.x()-a.x()*b.z(), a.x()*b.y()-a.y()*b.x());
}

template<class T>
const void add(const HVector<T>& a, const HVector<T>& b, const HVector<T>& dest) {
    for(int i=0; i <a.getN();i++){
        dest.set(i,a.get(i)+b.get(i));
    }
}
template<class T>
const void sub(const HVector<T>& a, const HVector<T>& b, const HVector<T>& dest) {
    for(int i=0; i <a.getN();i++){
        dest.set(i,a.get(i)-b.get(i));
    }
}
template<class T>
const void scale(const HVector<T>& vect, const HVector<T>& dest) {
    for(int i=0; i <vect.getN();i++){
        //dest.set(i,a.get(i)-b.get(i));
    }
}
template<class T>
const HVector<T> add(const HVector<T>& a, const HVector<T>& b) {
    HVector<T> res(a.getN());
    add(a, b, res);
    return res;
}

template<class T>
const HVector<T> sub(const HVector<T>& a, const HVector<T>& b) {
    HVector<T> res(a.getN());
    sub(a, b, res);
    return res;
}




#endif /* HVector_h */
