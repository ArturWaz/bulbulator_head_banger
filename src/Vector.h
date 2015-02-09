/*
 * Vector.h
 *
 *  Created on: 31.01.15
 *      Author: artur
 */


#ifndef Vector_H_
#define Vector_H_





class Vector {

    double *vector;
    int length;

public:

    Vector(int length);
    Vector(const Vector&);
    ~Vector();

    inline const double&operator[](int i) const {
        if (i >= length) throw false;
        return vector[i];
    }
    inline double&operator[](int i) {
        if (i >= length) throw false;
        return vector[i];
    }

    inline int getLength() const { return length; }

};


#endif //__Vector_H_

