/*
 * MovingArray.h
 *
 *  Created on: 15.02.15
 *      Author: artur
 */


#ifndef MovingArray_H_
#define MovingArray_H_


#include <cstdlib>


template <class T>class MovingArray {

    T *array;
    unsigned int lenght;
    unsigned int index;

    void operator=(MovingArray) {}
    MovingArray(MovingArray&) {}

public:

    class iterator {

        unsigned int index;
        MovingArray *movingArray;

    public:

        iterator(MovingArray &movingArray): movingArray(&movingArray), index(0) {}
        iterator(iterator const &it): movingArray(it.movingArray), index(it.index) {}
        ~iterator() {}

        inline iterator const &operator=(MovingArray &movingArray) { index = 0; iterator::movingArray = &movingArray; }
        inline iterator const &operator=(iterator const &it) { index = it.index; movingArray = it.movingArray; }

        inline T const &value() const { return movingArray->array[index]; }
        inline T &value() { return movingArray->array[index]; }

        inline iterator &operator++() { ++index; if (index == movingArray->lenght) index = 0; return *this; }
        inline iterator &operator--() { if (index == 0) index = movingArray->lenght; --index; return *this; }

        iterator const &operator+=(unsigned int i) {
            if (i >= movingArray->lenght) throw -1;
            index += i;
            if (index >= movingArray->lenght) index -= movingArray->lenght;
            return *this;
        }
        iterator const &operator-=(unsigned int i) {
            if (i >= movingArray->lenght) throw -1;
            if (index < i) {
                index += movingArray->lenght - i;
                return *this;
            }
            index -= i;
            return *this;
        }

        iterator operator+(unsigned int i) const {
            if (i >= movingArray->lenght) throw -1;
            iterator out(*this);
            out.index += i;
            if (out.index >= out.movingArray->lenght) out.index -= out.movingArray->lenght;
            return out;
        }
        iterator operator-(unsigned int i) const {
            if (i >= movingArray->lenght) throw -1;
            iterator out(*this);
            if (out.index < i) {
                out.index += out.movingArray->lenght - i;
                return out;
            }
            out.index -= i;
            return out;
        }
    };

    MovingArray(unsigned int size): lenght(size), index(0) {
        array = (T*)calloc(lenght,sizeof(T));
    }
    ~MovingArray() {
        free(array);
    }

    inline T const &operator[](unsigned int i) const {
        if (i >= lenght) throw -1;
        return array[i];
    }
    inline T &operator[](unsigned int i) {
        if (i >= lenght) throw -1;
        return array[i];
    }

    inline unsigned int size() { return lenght; }

};


#endif