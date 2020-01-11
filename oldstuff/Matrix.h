//
//  Matrix.h
//  LibAKC++
//
//  Created by Akarsh Kumar on 5/20/19.
//  Copyright © 2019 Akarsh Kumar. All rights reserved.
//

#ifndef Matrix_h
#define Matrix_h



//m rows by n columns
template <class T>
class AbstractMatrix {
public:
    virtual int getM() const = 0;
    virtual int getN() const = 0;
    virtual void set(const int& m, const int& n, const T& t) = 0;
    virtual const T& get(const int& m, const int& n) const = 0;
};


template <class T, int M, int N>
class Matrix: public AbstractMatrix<T> {
#define i(m, n) m*N+n
private:
    T matrix[M][N];
    
public:
    Matrix(){
        
    }
    Matrix(const T*& matrix){
        for(int m = 0; m < M; m++) {
            for(int n = 0; n < N; n++) {
                this->set(m, n, matrix[i(m, n)]);
            }
        }
    }
    Matrix(const T**& matrix){
        for(int m = 0; m < M; m++) {
            for(int n = 0; n < N; n++) {
                this->set(m, n, matrix[m][n]);
            }
        }
    }
    Matrix(const T matrix[][N]){
        for(int m = 0; m < M; m++) {
            for(int n = 0; n < N; n++) {
                this->set(m, n, matrix[m][n]);
            }
        }
    }
    Matrix(const Matrix<T, M, N>& matrix){
        for(int m = 0; m < M; m++) {
            for(int n = 0; n < N; n++) {
                this->set(m, n, matrix.get(m,n));
            }
        }
    }
    
    int getM() const {
        return M;
    }
    int getN() const {
        return N;
    }
    void set(const int& m, const int& n, const T& t){
        matrix[m][n] = t;
    }
    const T& get(const int& m, const int& n) const {
        return matrix[m][n];
    }
    
    friend Matrix<T, M, N> operator* (const double& x, const Matrix<T, M, N>& y);
    
};

template<int M, int N>
using MatrixD = Matrix<double, M, N>;


template <class T, int M, int N, class Scalar>
Matrix<T, M, N> operator* (const Scalar& c, const Matrix<T, M, N>& matrix) {
    Matrix<T, M, N> ret(matrix);
    for(int m = 0; m < M; m++) {
        for(int n = 0; n < N; n++) {
            ret.set(m, n, c * ret.get(m,n));
        }
    }
    return ret;
}
template <class T, int M, int N, class Scalar>
Matrix<T, M, N> operator* (const Matrix<T, M, N>& matrix, const Scalar& c) {
    return c * matrix;
}


#endif /* Matrix_h */
