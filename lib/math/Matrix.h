/*
 * Matrix.h
 *
 *  Created on: 15.02.15
 *      Author: artur
 */


#ifndef Matrix_H_
#define Matrix_H_


#include <cstring>
#include <cstdlib>
#include "Vector.h"
#include "myMath.h"



template <class Type> class Matrix {
protected:

    Type **table;
    Type *ptrTable;
    unsigned int rows;
    unsigned int columns;

    Matrix() {}

    void swapMatrices(Matrix<Type> &m) {
        Type *tmp_ptrTable = ptrTable;
        unsigned int tmp_rows = rows;
        unsigned int tmp_columns = columns;
        ptrTable = m.ptrTable;
        rows = m.rows;
        columns = m.columns;
        m.ptrTable = tmp_ptrTable;
        m.rows = tmp_rows;
        m.columns = tmp_columns;
        Type **tmp_table = table;
        table = m.table;
        m.table = tmp_table;
//        free(table);
//        free(m.table);
//        table = (Type**)malloc(rows*sizeof(Type*));
//        m.table = (Type**)malloc(m.rows*sizeof(Type*));
//        if (table == NULL || m.table == NULL) throw -1;
//        for (unsigned int i = 0; i < rows; ++i)
//            table[i] = &ptrTable[i*columns];
//        for (unsigned int i = 0; i < m.rows; ++i)
//            m.table[i] = &m.ptrTable[i*m.columns];
    }

public:

    Matrix(unsigned int rows, unsigned int columns): rows(rows), columns(columns) {
        if (rows == 0 || columns == 0) throw -2;
        ptrTable = (Type*)calloc(rows*columns,sizeof(Type));
        table = (Type**)malloc(rows*sizeof(Type*));
        if (ptrTable == NULL || table == NULL) throw -1;
        for (unsigned int i = 0; i < rows; ++i)
            table[i] = &ptrTable[i*columns];
    }
    Matrix(Matrix<Type> const &m): rows(m.rows), columns(m.columns) {
        ptrTable = (Type*)malloc(rows*columns*sizeof(Type));
        table = (Type**)malloc(rows*sizeof(Type*));
        if (ptrTable == NULL || table == NULL) throw -1;
        memcpy(ptrTable,m.ptrTable,rows*columns*sizeof(Type));
        for (unsigned int i = 0; i < rows; ++i)
            table[i] = &ptrTable[i*columns];
    }
    Matrix(Vector<Type> const &v): rows(v.getRows()), columns(v.getColumns()) {
        ptrTable = (Type*)malloc(v.getLength()*sizeof(Type));
        table = (Type**)malloc(rows*sizeof(Type*));
        if (ptrTable == NULL || table == NULL) throw -1;
        for (unsigned int i = 0; i < rows; ++i)
            table[i] = &ptrTable[i*columns];
        memcpy(ptrTable,v.tablePtr(),v.getLength()*sizeof(Type));
    }
    ~Matrix() {
        free(ptrTable);
        free(table);
    }

    Matrix<Type> const &operator=(Matrix<Type> const &m) {
        if (rows != m.rows || columns != m.columns) throw -2;
        memcpy(ptrTable,m.ptrTable,rows*columns*sizeof(Type));
    }

    inline Type const &operator()(unsigned int row, unsigned int col) const { return table[row][col]; }
    inline Type &operator()(unsigned int row, unsigned int col) { return table[row][col]; }
    inline unsigned int getRows() const { return rows; }
    inline unsigned int getColumns() const { return columns; }

    inline Type const &operator()(unsigned int i) const { return ptrTable[i]; }
    inline Type &operator()(unsigned int i) { return ptrTable[i]; }
    inline unsigned int length() const { return columns*rows; }

    inline bool isVector() const { return columns == 1 || rows == 1; }
    inline bool isScalar() const { return columns == 1 && rows == 1; }

    Matrix<Type> const &transpose() {
        Matrix<Type> matrix(columns,rows);
        for (unsigned int i = 0; i < rows; ++i)
            for (unsigned int j = 0; j < columns; ++j)
                matrix.table[j][i] = table[i][j];
        swapMatrices(matrix);
        return *this;
    }

    Matrix<Type> const &operator+=(Matrix<Type> const &m) {
        if (rows != m.rows || columns != m.columns) throw -2;
        for (unsigned int i = 0; i < rows; ++i)
            for (unsigned int j = 0; j < columns; ++j)
                table[i][j] += m.table[i][j];
        return *this;
    }

    Matrix<Type> const &operator-=(Matrix<Type> const &m) {
        if (rows != m.rows || columns != m.columns) throw -2;
        for (unsigned int i = 0; i < rows; ++i)
            for (unsigned int j = 0; j < columns; ++j)
                table[i][j] -= m.table[i][j];
        return *this;
    }

    Matrix<Type> const &operator*=(Matrix<Type> const &m) {
        if (columns != m.rows) throw -2;
        Matrix<Type> matrix(rows,m.columns);
        for (unsigned int i = 0; i < matrix.rows; ++i)
            for (unsigned int j = 0; j < matrix.columns; ++j)
                for (unsigned int k = 0; k < columns; ++k)
                    matrix.table[i][j] += table[i][k] * m.table[k][j];
        swapMatrices(matrix);
        return *this;
    }

    Matrix<Type> const &operator*=(Type const &m) {
        for (unsigned int i = 0; i < rows; ++i)
            for (unsigned int j = 0; j < columns; ++j)
                table[i][j] *= m;
        return *this;
    }

    Matrix<Type> const &operator/=(Type const &m) {
        for (unsigned int i = 0; i < rows; ++i)
            for (unsigned int j = 0; j < columns; ++j)
                table[i][j] /= m;
        return *this;
    }


    Matrix<Type> operator+(Matrix<Type> const &m) const {
        if (rows != m.rows || columns != m.columns) throw -2;
        Matrix<Type> matrix(*this);
        for (unsigned int i = 0; i < rows; ++i)
            for (unsigned int j = 0; j < columns; ++j)
                matrix.table[i][j] += m.table[i][j];
        return matrix;
    }

    Matrix<Type> operator-(Matrix<Type> const &m) const {
        if (rows != m.rows || columns != m.columns) throw -2;
        Matrix<Type> matrix(*this);
        for (unsigned int i = 0; i < rows; ++i)
            for (unsigned int j = 0; j < columns; ++j)
                matrix.table[i][j] -= m.table[i][j];
        return matrix;
    }

    Matrix<Type> operator*(Matrix<Type> const &m) const {
        if (columns != m.rows) throw -2;
        Matrix<Type> matrix(rows,m.columns);
        for (unsigned int i = 0; i < matrix.rows; ++i)
            for (unsigned int j = 0; j < matrix.columns; ++j)
                for (unsigned int k = 0; k < columns; ++k)
                    matrix.table[i][j] += table[i][k] * m.table[k][j];
        return matrix;
    }

    Matrix<Type> operator*(Type const &m) const {
        Matrix<Type> matrix(*this);
        for (unsigned int i = 0; i < rows; ++i)
            for (unsigned int j = 0; j < columns; ++j)
                matrix.table[i][j] *= m;
        return matrix;
    }

    Matrix<Type> operator/(Type const &m) const {
        Matrix<Type> matrix(*this);
        for (unsigned int i = 0; i < rows; ++i)
            for (unsigned int j = 0; j < columns; ++j)
                matrix.table[i][j] /= m;
        return matrix;
    }

};


#endif //__Matrix_H_