/*
* 使用数组形成一个线性表(内存连续，可以通过下表访问)
*/
#ifndef LIST_LINK_HPP_
#define LIST_LINK_HPP_

#include <iostream>
#include <string.h>

#define MAXSIZE 100

template<typename T>
class ListLink
{
private:
    /* data */
    T array[MAXSIZE];
    int length;
public:
    ListLink(/* args */);
    ~ListLink();

    void InitList();
    bool IsListEmpty();
    int getSize();
    void ClearList();
    T GetElem(int index);
    int LocateElem(T value);
    int InsertELem(int index, T value);
    int DeleteElem(int index, T* value);
};



template<typename T>
ListLink<T>::ListLink() {

}

template<typename T>
ListLink<T>::~ListLink() {

}

template<typename T>
void ListLink<T>::InitList() {
    memset(array, 0, MAXSIZE);
    length = 0;
}

template<typename T>
bool ListLink<T>::IsListEmpty() {
    if (length == 0) {
        return true;
    }
    return false;
}

template<typename T>
int ListLink<T>::getSize() {
    return length;
}

template<typename T>
void ListLink<T>::ClearList() {
    memset(array, 0, MAXSIZE);
    length = 0;
}

template<typename T>
T ListLink<T>::GetElem(int index) {
    return array[index - 1];
}

template<typename T>
int ListLink<T>::LocateElem(T value) {
    for (int i = 0; i< length; ++i) {
        if (array[i] == value ) {
            return i + 1;
        }
    }
    return -1;
}


template<typename T>
int ListLink<T>::InsertELem(int index, T value) {
    if (length == MAXSIZE) {
        return -1;
    }

    if (index < 1 || index > length + 1) {
        return -1;
    }

    if (index <= length + 1) {
        for (int k = length - 1; k > index - 1 ;k--) {
            array[k + 1] = array[k]; 
        }
    }

    array[index - 1] = value;
    length++;
    return 0;
}

template<typename T>
int ListLink<T>::DeleteElem(int index, T* value) {

    if (length == 0) {
        return -1;
    }

    if (index < 1 || index > length) {
        return -1;
    }

    if (index <= length) {
        for (int k = length - 1; k > index -1 ; k--) {
            array[k] = array[k + 1];
        }
    }
    *value = array[index - 1];
    length--;
    return 0;
}



#endif