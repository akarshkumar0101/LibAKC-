//
//  TensorMath.h
//  LibAKC++
//
//  Created by Akarsh Kumar on 5/23/19.
//  Copyright © 2019 Akarsh Kumar. All rights reserved.
//

#ifndef TensorMath_h
#define TensorMath_h

namespace TensorMath {
    template<class T>
    void tensorAdd(const T* arr1, const T* arr2, int N, T* dest){
        for(int i=0; i < N;i++){
            dest[i] = arr1[i] + arr2[i];
        }
    }
    template<class T>
    void tensorAdd(const T** arr1, const T** arr2, int M, int N, T** dest){
        for(int i=0; i < M;i++){
            tensorAdd(arr1[i], arr2[i], N, dest[i]);
        }
    }
    template<class T>
    void tensorAdd(const T*** arr1, const T*** arr2, int O, int M, int N, T*** dest){
        for(int i=0; i < O;i++){
            tensorAdd(arr1[i], arr2[i], M, N, dest[i]);
        }
    }
    
    template<class T>
    void tensorScale(const T* arr1, const T& scalar, int N, T* dest){
        for(int i=0; i < N;i++){
            dest[i] = scalar*arr1[i];
        }
    }
    template<class T>
    void tensorAdd(const T** arr1, const T& scalar, int M, int N, T** dest){
        for(int i=0; i < M;i++){
            tensorScale(arr1[i], scalar, N, dest[i]);
        }
    }
    template<class T>
    void tensorAdd(const T*** arr1, const T& scalar, int O, int M, int N, T*** dest){
        for(int i=0; i < O;i++){
            tensorScale(arr1[i], scalar, M, N, dest[i]);
        }
    }
    
}


#endif /* TensorMath_h */
