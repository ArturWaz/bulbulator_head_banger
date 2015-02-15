/*
 * Vector.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef Vector_H_
#define Vector_H_

#include <cstring>



template <class Type> class Vector {

    Type *table;
    unsigned int length;
    bool isVertical; // vector orientation, needed in matrix multiplications

public:

    inline Vector(unsigned int length): length(length), isVertical(true) { table = new Type[length]; }
    inline Vector(const Vector<Type> &v): length(v.length), isVertical(v.isVertical) {
        table = new Type[length];
        memcpy(table,v.table,length*sizeof(Type));
    }
    Vector const &operator=(const Vector<Type> &v) {
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

    inline Vector const &transpose() { isVertical = !isVertical; return *this; }

    inline Type const &operator[](unsigned int i) const { return table[i]; }
    inline Type &operator[](unsigned int i) { return table[i]; }

    inline Type const &operator()(unsigned int i) const {
        if (i >= length) throw -2;
        return table[i];
    }
    inline Type &operator()(unsigned int i) {
        if (i >= length) throw -2;
        return table[i];
    }

    Vector operator+(Vector<Type> const &v){
        if (length != v.length) throw -1;
        Vector vector(*this);
        for (int i = 0; i < length; ++i)
            vector.table[i] += v.table[i];
        return vector;
    }

    Vector operator-(Vector<Type> const &v){
        if (length != v.length) throw -1;
        Vector vector(*this);
        for (int i = 0; i < length; ++i)
            vector.table[i] -= v.table[i];
        return vector;
    }

    Vector operator*(Type const &v){
        Vector vector(*this);
        for (int i = 0; i < length; ++i)
            vector.table[i] *= v;
        return vector;
    }

    Vector operator/(Type const &v){
        Vector vector(*this);
        for (int i = 0; i < length; ++i)
            vector.table[i] /= v;
        return vector;
    }

    Vector const &operator+=(Vector<Type> const &v){
        if (length != v.length) throw -1;
        for (int i = 0; i < length; ++i)
            table[i] += v.table[i];
        return *this;
    }

    Vector const &operator-=(Vector<Type> const &v){
        if (length != v.length) throw -1;
        for (int i = 0; i < length; ++i)
            table[i] -= v.table[i];
        return *this;
    }

    Vector const &operator*=(Type const &v){
        for (int i = 0; i < length; ++i)
            table[i] *= v;
        return *this;
    }

    Vector const &operator/=(Type const &v){
        for (int i = 0; i < length; ++i)
            table[i] /= v;
        return *this;
    }
};

#endif //__Vector_H_