//
// Created by Akarsh Kumar on 1/13/20.
//

#pragma once

#include <vector>

template<class T>
using Array1D = std::vector<T>;
template<class T>
using Array2D = std::vector<Array1D<T>>;
template<class T>
using Array3D = std::vector<Array2D<T>>;

using FArray1D = Array1D<float>;
using FArray2D = Array2D<float>;
using FArray3D = Array3D<float>;

template<class T>
std::shared_ptr<Array1D<T>> newArray1D(unsigned int l1, const T& defaultValue){
    return std::make_shared<Array1D<T>>(l1, defaultValue);
}

template<class T>
std::shared_ptr<Array2D<T>> newArray2D(unsigned int l2, unsigned int l1, const T& defaultValue){
    return std::make_shared<Array2D<T>>(l2, *newArray1D(l1,defaultValue));
}

template<class T>
std::shared_ptr<Array3D<T>> newArray3D(unsigned int l3, unsigned int l2, unsigned int l1, const T& defaultValue){
    return std::make_shared<Array3D<T>>(l3, *newArray2D(l2,l1,defaultValue));
}

template<class T>
T scaleValue(const T& oa, const T& ob, const T& na,const  T& nb, const T& val) {
   val -= oa;
   val *= (nb-na)/(ob-oa);
   val += na;
   return val;
}

template<class T>
class Array {
private:
    const std::vector<unsigned int> mDimensions;
    const std::vector<unsigned int> mOffsets;
    std::vector<T> mData;

    unsigned int calcNumElements();
    void calcOffsets();
public:
    Array(std::vector<unsigned int>& dimensions, const T& fillValue);

    T& at(std::vector<unsigned int>& coordinates);
    std::vector<T>& data();

    unsigned int numDimensions();
    const std::vector<unsigned int>& dimensions();
    const std::vector<unsigned int>& offsets();
};





template<class T>
Array<T>::Array(std::vector<unsigned int> &dimensions, const T &fillValue): mDimensions(dimensions), mData(calcNumElements(), fillValue), mOffsets(mDimensions.size(), 0) {
    calcOffsets();
}

template<class T>
unsigned int Array<T>::calcNumElements() {
    unsigned int num = 1;
    for(unsigned int dimen: mDimensions){
        num *= dimen;
    }
    return num;
}

template<class T>
void Array<T>::calcOffsets() {
    unsigned int offset = 1;
    for(int dimen = mDimensions.size()-1;dimen>=0;dimen--){
        mOffsets[dimen] = offset;
        offset *= mDimensions[dimen];
    }
}

template<class T>
T &Array<T>::at(std::vector<unsigned int> &coordinates) {
    unsigned int offset = 0;
    for(int i=0;i<mOffsets.size();i++){
        offset+=mOffsets[i] * (*(coordinates.begin()+i));
    }

    return mData.at(offset);
}

template<class T>
std::vector<T> &Array<T>::data() {
    return mData;
}


template<class T>
unsigned int Array<T>::numDimensions(){
    return mDimensions.size();
}
template<class T>
const std::vector<unsigned int>& Array<T>::dimensions(){
    return mDimensions;
}
template<class T>
const std::vector<unsigned int>& Array<T>::offsets(){
    return mOffsets;
}


