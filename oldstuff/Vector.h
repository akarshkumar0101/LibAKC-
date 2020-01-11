//
//  Vector.h
//  LibAKC++
//
//  Created by Akarsh Kumar on 5/20/19.
//  Copyright © 2019 Akarsh Kumar. All rights reserved.
//

#ifndef Vector_h
#define Vector_h


template<class T>
class AbstractVector {
public:
    virtual int getN() const = 0;
    virtual void set(const int& n, const T& t) = 0;
    virtual const T& get(const int& n) const = 0;
};

template<class T, int N>
class Vector: public AbstractVector<T> {
private:
#define VECTOR_X_INDEX 0
#define VECTOR_Y_INDEX 1
#define VECTOR_Z_INDEX 2
#define VECTOR_W_INDEX 3
    T vector[N];
    
public:
    Vector(){
        for(int i=0; i < N; i++){
            set(i, 0);
        }
    }
    Vector(T* const & vector){
        for(int i=0; i < N; i++){
            set(i, vector[i]);
        }
    }
    Vector(const std::array<T, N>& vector){
        for(int i=0; i < N; i++){
            set(i, vector[i]);
        }
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
    void set(const int& n, const T& t) {
        vector[n] = t;
    }
    const T& get(const int& n) const {
        return vector[n];
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
    const DotType dot(const Vector<T2, N>& another) const {
        DotType ret = 0;
        for(int i=0; i < N;i++){
            ret += get(i) * another.get(i);
        }
        return ret;
    }
    template<class DotType>
    const DotType dot(const Vector<T, N>& another){
        return dot<T,DotType>(another);
    }
    const T dot(const Vector<T, N>& another){
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
const Vector<T,3> cross(const Vector<T,3>& a, const Vector<T, 3>& b) {
    return Vector<T, 3>(a.y()*b.z()-a.z()*b.y(), a.z()*b.x()-a.x()*b.z(), a.x()*b.y()-a.y()*b.x());
}


template<class T>
std::ostream& operator<<(std::ostream& os, const AbstractVector<T>& vect)
{
    os << "Vector" << vect.getN() <<"D: <";
    for(int i = 0; i < vect.getN();i++){
        os<< vect.get(i);
        if(i!=vect.getN()-1){
            os<<", ";
        }
    }
    os<<">";
    return os;
}

using VectorF2 = Vector<float, 2>;
using VectorD2 = Vector<double, 2>;
using VectorF3 = Vector<float, 3>;
using VectorD3 = Vector<double, 3>;


#endif /* Vector_h */
