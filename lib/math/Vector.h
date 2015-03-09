/*
 * Vector.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef Vector_H_
#define Vector_H_

#include <cstring>
#include <cstdlib>
#include "myMath.h"



template <class Type> class Vector {
protected:

    Type *table;
    size_t length;
    bool isVertical; // vector orientation, needed in matrix multiplications

    Vector(): isVertical(true) {}

public:

    class Exception : public std::exception {
        char const *info;
    public:
        explicit Exception(char const *info): info(info) {}
        virtual char const *what() const throw() { return info; }
    };

    Vector(size_t length): length(length), isVertical(true) {
        table = (Type*)calloc(length,sizeof(Type));
        if (table == NULL) throw Exception("Cannot allocate memory, function Vector::Vector(size_t)");
    }
    Vector(const Vector<Type> &v): length(v.length), isVertical(v.isVertical) {
        table = (Type*)malloc(length*sizeof(Type));
        if (table == NULL) throw Exception("Cannot allocate memory, function Vector::Vector(Vector const &)");
        memcpy(table,v.table,length*sizeof(Type));
    }
    Vector<Type> const &operator=(const Vector<Type> &v) {
        if (length != v.length || isVertical != v.isVertical) throw Exception("Size of vectors are different, function Vector::operator=(Vector const &)");
        memcpy(table,v.table,length*sizeof(Type));
        return *this;
    }
    ~Vector() { free(table); }

    inline Type *tablePtr() const { return table; }

    inline size_t getLength() const { return length; }
    inline size_t getRows() const { return isVertical?length:1; }
    inline size_t getColumns() const { return isVertical?1:length; }

    inline Vector<Type> const &transpose() { isVertical = !isVertical; return *this; }
    Type norm() const {
        Type norm(0);
        for (int i = 0; i < length; ++i) norm += table[i]*table[i];
        return sqrt(norm);
    }

    inline Type const &operator[](size_t i) const {
        if (i >= length) throw Exception("Out of range error, function: Vector::operator[](size_t)");
        return table[i];
    }
    inline Type &operator[](size_t i) {
        if (i >= length) throw Exception("Out of range error, function: Vector::operator[](size_t)");
        return table[i];
    }

    inline Type const &operator()(size_t i) const { return table[i]; }
    inline Type &operator()(size_t i) { return table[i]; }

    Vector<Type> operator+(Vector<Type> const &v){
        if (length != v.length) throw Exception("Size of vectors are different, function Vector::operator+(Vector const &)");
        Vector<Type> vector(*this);
        for (size_t i = 0; i < length; ++i)
            vector.table[i] += v.table[i];
        return vector;
    }

    Vector<Type> operator-(Vector<Type> const &v){
        if (length != v.length) throw Exception("Size of vectors are different, function Vector::operator-(Vector const &)");
        Vector<Type> vector(*this);
        for (size_t i = 0; i < length; ++i)
            vector.table[i] -= v.table[i];
        return vector;
    }

    Vector<Type> operator*(Type const &v){
        Vector<Type> vector(*this);
        for (size_t i = 0; i < length; ++i)
            vector.table[i] *= v;
        return vector;
    }

    Vector<Type> operator/(Type const &v){
        Vector<Type> vector(*this);
        for (size_t i = 0; i < length; ++i)
            vector.table[i] /= v;
        return vector;
    }

    Vector<Type> const &operator+=(Vector<Type> const &v){
        if (length != v.length) throw Exception("Size of vectors are different, function Vector::operator+=(Vector const &)");
        for (size_t i = 0; i < length; ++i)
            table[i] += v.table[i];
        return *this;
    }

    Vector<Type> const &operator-=(Vector<Type> const &v){
        if (length != v.length) throw Exception("Size of vectors are different, function Vector::operator-=(Vector const &)");
        for (size_t i = 0; i < length; ++i)
            table[i] -= v.table[i];
        return *this;
    }

    Vector<Type> const &operator*=(Type const &v){
        for (size_t i = 0; i < length; ++i)
            table[i] *= v;
        return *this;
    }

    Vector<Type> const &operator/=(Type const &v){
        for (size_t i = 0; i < length; ++i)
            table[i] /= v;
        return *this;
    }

};

#endif //__Vector_H_