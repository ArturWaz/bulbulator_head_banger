/*
 * Vector.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef Vector_H_
#define Vector_H_

#include <cstring>
#include "myMath.h"



template <class Type> class Vector {
protected:

    Type *table;
    unsigned int length;
    bool isVertical; // vector orientation, needed in matrix multiplications

public:

    inline Vector(unsigned int length): length(length), isVertical(true) { table = new Type[length]; }
    inline Vector(const Vector<Type> &v): length(v.length), isVertical(v.isVertical) {
        table = new Type[length];
        memcpy(table,v.table,length*sizeof(Type));
    }
    Vector<Type> const &operator=(const Vector<Type> &v) {
        if (length != v.length) throw -1;
        length = v.length;
        isVertical = v.isVertical;
        memcpy(table,v.table,length*sizeof(Type));
        return *this;
    }
    inline ~Vector() { delete []table; }


    inline unsigned int getLength() const { return length; }
    inline unsigned int getNumberOfRows() const { return isVertical?length:1; }
    inline unsigned int getNumberOfColumns() const { return isVertical?1:length; }

    inline Vector<Type> const &transpose() { isVertical = !isVertical; return *this; }
    Type norm() const {
        Type norm(0);
        for (int i = 0; i < length; ++i) norm += table[i]*table[i];
        return sqrt(norm);
    }

    inline Type const &operator[](unsigned int i) const {
        if (i >= length) throw -2;
        return table[i];
    }
    inline Type &operator[](unsigned int i) {
        if (i >= length) throw -2;
        return table[i];
    }

    inline Type const &operator()(unsigned int i) const { return table[i]; }
    inline Type &operator()(unsigned int i) { return table[i]; }

    Vector<Type> operator+(Vector<Type> const &v){
        if (length != v.length) throw -1;
        Vector<Type> vector(*this);
        for (int i = 0; i < length; ++i)
            vector.table[i] += v.table[i];
        return vector;
    }

    Vector<Type> operator-(Vector<Type> const &v){
        if (length != v.length) throw -1;
        Vector<Type> vector(*this);
        for (int i = 0; i < length; ++i)
            vector.table[i] -= v.table[i];
        return vector;
    }

    Vector<Type> operator*(Type const &v){
        Vector<Type> vector(*this);
        for (int i = 0; i < length; ++i)
            vector.table[i] *= v;
        return vector;
    }

    Vector<Type> operator/(Type const &v){
        Vector<Type> vector(*this);
        for (int i = 0; i < length; ++i)
            vector.table[i] /= v;
        return vector;
    }

    Vector<Type> const &operator+=(Vector<Type> const &v){
        if (length != v.length) throw -1;
        for (int i = 0; i < length; ++i)
            table[i] += v.table[i];
        return *this;
    }

    Vector<Type> const &operator-=(Vector<Type> const &v){
        if (length != v.length) throw -1;
        for (int i = 0; i < length; ++i)
            table[i] -= v.table[i];
        return *this;
    }

    Vector<Type> const &operator*=(Type const &v){
        for (int i = 0; i < length; ++i)
            table[i] *= v;
        return *this;
    }

    Vector<Type> const &operator/=(Type const &v){
        for (int i = 0; i < length; ++i)
            table[i] /= v;
        return *this;
    }
};

#endif //__Vector_H_