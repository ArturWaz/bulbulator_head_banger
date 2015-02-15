/*
 * Matrix.h
 *
 *  Created on: 15.02.15
 *      Author: artur
 */


#ifndef Matrix_H_
#define Matrix_H_


#include <cstring>
#include "myMath.h"



template <class Type> class Matrix {
protected:

    Type **table;
    unsigned int rows;
    unsigned int columns;
//    bool transposed;

public:

    Matrix(unsigned int rows, unsigned int columns): rows(rows), columns(columns)/*, transposed(false) */{
        table = new Type*[rows];
        for (unsigned int i = 0; i < rows; ++i)
            table[i] = new Type[columns];
    }
    Matrix(Matrix const &m): rows(m.rows), columns(m.columns)/*, transposed(m.false) */{
        table = new Type*[rows];
        for (unsigned int i = 0; i < rows; ++i) {
            table[i] = new Type[columns];
            memcpy(table[i],m.table[i],columns*sizeof(Type));
        }
    }
    ~Matrix() {
        for (unsigned int i = 0; i < rows; ++i)
            delete [](table[i]);
        delete [](*table);
    }

    Matrix<Type> const &operator=(Matrix<Type> const &m) {
        if (rows != m.rows || columns != m.columns) throw -2;
        for (int i = 0; i < rows; ++i)
            memcpy(table[i],m.table[i],columns*sizeof(Type));
    }

    inline Type const &operator()(unsigned int row, unsigned int col) const { return table[row][col]; }
    inline Type &operator()(unsigned int row, unsigned int col) { return table[row][col]; }
    inline unsigned int getNumberOfRows() const { return rows; }
    inline unsigned int getNumberOfColumns() const { return columns; }

    Matrix<Type> const &operator+=(Matrix<Type> const &m) {
        if (rows != m.rows || columns != m.columns) throw -2;
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
                table[i][j] += m.table[i][j];
        return *this;
    }

    Matrix<Type> const &operator-=(Matrix<Type> const &m) {
        if (rows != m.rows || columns != m.columns) throw -2;
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
                table[i][j] -= m.table[i][j];
        return *this;
    }

//    Matrix<Type> const &operator*=(Matrix<Type> const &m) {
//
//        return *this;
//    }

    Matrix<Type> const &operator*=(Type const &m) {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
                table[i][j] *= m;
        return *this;
    }

    Matrix<Type> const &operator/=(Type const &m) {
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
                table[i][j] /= m;
        return *this;
    }


    Matrix<Type> operator+(Matrix<Type> const &m) {
        if (rows != m.rows || columns != m.columns) throw -2;
        Matrix<Type> matrix(*this);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
                matrix.table[i][j] += m.table[i][j];
        return matrix;
    }

    Matrix<Type> operator-(Matrix<Type> const &m) {
        if (rows != m.rows || columns != m.columns) throw -2;
        Matrix<Type> matrix(*this);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
                matrix.table[i][j] -= m.table[i][j];
        return matrix;
    }

    Matrix<Type> operator*(Type const &m) {
        Matrix<Type> matrix(*this);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
                matrix.table[i][j] *= m;
        return matrix;
    }

    Matrix<Type> operator/(Type const &m) {
        Matrix<Type> matrix(*this);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
                matrix.table[i][j] /= m;
        return matrix;
    }

};


#endif //__Matrix_H_