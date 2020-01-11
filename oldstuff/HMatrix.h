//
//  HMatrix.h
//  LibAKC++
//
//  Created by Akarsh Kumar on 5/20/19.
//  Copyright © 2019 Akarsh Kumar. All rights reserved.
//

#ifndef HMatrix_h
#define HMatrix_h

#include "Matrix.h"
#include "HVector.h"


//m rows by n columns
template <class T>
class HMatrix: public AbstractMatrix<T> {
private:
    int M, N;
    //T* matrix;
    HVector<T>* matrix;
    
    
public:
    HMatrix(): M(0), N(0), matrix(nullptr){
    }
    HMatrix(int M, int N): M(M), N(N), matrix(new HVector<T>[M]){
        for(int m=0; m <M;m++){
            matrix[m] = HVector<T>(N);
        }
    }
    template<int M, int N>
    HMatrix(const std::array<std::array<T, N>, M>& matrix): M(M), N(N), matrix(new HVector<T>[M]){
        for(int m=0; m <M;m++){
            this->matrix[m] = HVector<T>(matrix[m]);
        }
    }
    
    ~HMatrix(){
        delete[] matrix;
    }
    
    int getM() const {
        return M;
    }
    void setMN(int M, int N){
        this->M = M;
        this->N = N;
        delete[] matrix;
        matrix = new HVector<T>[M];
        for(int m=0; m <M;m++){
            matrix[m] = HVector<T>(N);
        }
    }
    int getN() const {
        return N;
    }
    void set(const int& m, const int& n, const T& t) {
        matrix[m].set(n, t);
    }
    const HVector<T>& getRow(const int& m) const{
        return matrix[m];
    }
    const T& get(const int& m, const int& n) const {
        return matrix[m].get(n);
    }
    
    void mul(const HVector<T>& v, const HVector<T>& dest){
        if(this->getN() != v.getN() || this->getM()!=dest.getN()){
            throw -1;
        }
        
        for(int m=0;m<dest.getN();m++){
            T entry = 0;
            for(int i=0; i<this->getN();i++){
                entry = entry + (this->get(m, i) * v.get(i));
            }
            dest.set(m, entry);
        }
    }
    
    void muladd(const HVector<T>& v, const HVector<T>& b, const HVector<T>& dest){
        if(this->getN() != v.getN() || this->getM()!=dest.getN()){
            throw -1;
        }
        
        for(int m=0;m<dest.getN();m++){
            T entry = 0;
            for(int i=0; i<this->getN();i++){
                entry = entry + (this->get(m, i) * v.get(i));
            }
            dest.set(m, entry+b.get(m));
        }
    }
    
    
    friend HMatrix<T> operator* (double x, const HMatrix<T>& y);
};


template<class T>
std::ostream& operator<<(std::ostream& os, const HMatrix<T>& mat)
{
    using namespace std;
    os << "Matrix" << mat.getM() <<"x" <<mat.getN()<<": "<<endl;
    
    for(int m = 0; m < mat.getM(); m++){
        os<<(m==0?"[":" ");
        os<< mat.getRow(m);
        os<<(m==mat.getM()-1?"]":" ")<<endl;
        
    }
    return os;
}

template <class T>
HMatrix<T> operator* (const HMatrix<T>& m1, const HMatrix<T>& m2) {
    if(m1.getN() != m2.getM()){
        throw -1;
    }
    HMatrix<T> res (m1.getM(), m2.getN());
    for(int m=0;m<res.getM();m++){
        for(int n=0;n<res.getN();n++){
            T entry = 0;
            for(int i=0; i<m1.getN();i++){
                entry = entry + (m1.get(m, i) * m2.get(i,n));
            }
            res.set(m, n, entry);
        }
    }
    
    return res;
}


template <class T>
HVector<T> operator* (const HMatrix<T>& m1, const HVector<T>& v) {
    if(m1.getN() != v.getN()){
        throw -1;
    }
    HVector<T> res (m1.getM());
    
    for(int m=0;m<res.getN();m++){
        T entry = 0;
        for(int i=0; i<m1.getN();i++){
            entry = entry + (m1.get(m, i) * v.get(i));
        }
        res.set(m, entry);
    }
    
    return res;
}

/*
template <class T, class Scalar>
HMatrix<T> operator* (const Scalar& c, const HMatrix<T>& matrix) {
    HMatrix<T> ret(matrix);
    for(int m = 0; m < ret.getM(); m++) {
        for(int n = 0; n < ret.getN(); n++) {
            ret.set(m, n, c * ret.get(m,n));
        }
    }
    return ret;
}

template <class T, class Scalar>
HMatrix<T> operator* (const HMatrix<T>& matrix, const Scalar& c) {
    return c * matrix;
}
*/


#endif /* HMatrix_h */
