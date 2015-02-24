/*
 * Buffer.h
 *
 *  Created on: 15.02.15
 *      Author: artur
 */


#ifndef Buffer_H_
#define Buffer_H_


#include <cstdlib>


template <class T, unsigned long LENGTH> class Buffer {

    T array[LENGTH] = {};
    unsigned long length;
    unsigned long index;

    void operator=(Buffer) {}
    Buffer(Buffer &) {}

public:

    Buffer(): length(LENGTH), index(0) {}
    Buffer(T const &initValue): length(LENGTH), index(0) { for (T &elem : array) elem = initValue; }
    ~Buffer() {}

    inline T const &operator[](unsigned long i) const {
        if (i >= length) throw -1;
        return array[i];
    }
    inline T &operator[](unsigned long i) {
        if (i >= length) throw -1;
        return array[i];
    }

    inline unsigned long size() const { return length; }
    inline unsigned long indexPosition() const { return index; }

    inline void push(T const &element) { ++index; if (index == length) index = 0; array[index] = element; }

    inline T const &last() const { return array[index]; }
    inline T &last() { return array[index]; }

    inline T const &get(unsigned long i) const { return (index < i) ? array[index+length-i] : array[index-i]; }

    inline T const &operator()(unsigned long i) const { return (index < i) ? array[index+length-i] : array[index-i]; }
    inline T &operator()(unsigned long i) { return (index < i) ? array[index+length-i] : array[index-i]; }



/*  TEST ITERATOR CLASS BEFORE USE

    class iterator {
        friend class Buffer;

        unsigned long index;
        Buffer *buffer;

        iterator(Buffer *buffer, unsigned long index): buffer(buffer), index(index) {}

    public:

        iterator(iterator const &it): buffer(it.buffer), index(it.index) {}
        ~iterator() {}

        inline iterator const &operator=(iterator const &it) { index = it.index; buffer = it.buffer; }

        inline T const &value() const { return buffer->array[index]; }
        inline T &value() { return buffer->array[index]; }

        inline iterator &operator++() { ++index; if (index == buffer->lenght) index = 0; return *this; }
        inline iterator &operator--() { if (index == 0) index = buffer->lenght; --index; return *this; }

        iterator const &operator+=(unsigned long i) {
            if (i >= buffer->lenght) throw -1;
            index += i;
            if (index >= buffer->lenght) index -= buffer->lenght;
            return *this;
        }
        iterator const &operator-=(unsigned long i) {
            if (i >= buffer->lenght) throw -1;
            if (index < i) {
                index += buffer->lenght - i;
                return *this;
            }
            index -= i;
            return *this;
        }

        iterator operator+(unsigned long i) const {
            if (i >= buffer->lenght) throw -1;
            iterator out(*this);
            out.index += i;
            if (out.index >= out.buffer->lenght) out.index -= out.buffer->lenght;
            return out;
        }
        iterator operator-(unsigned long i) const {
            if (i >= buffer->lenght) throw -1;
            iterator out(*this);
            if (out.index < i) {
                out.index += out.buffer->lenght - i;
                return out;
            }
            out.index -= i;
            return out;
        }
    };

    inline iterator begin() { return iterator(this,index); }
*/

};


#endif