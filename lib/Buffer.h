/*
 * Buffer.h
 *
 *  Created on: 15.02.15
 *      Author: artur
 */


#ifndef Buffer_H_
#define Buffer_H_


#include <cstdlib>


template <class T, unsigned long LENGTH>
class Buffer {

    T array_[LENGTH] = {};
    unsigned long length_;
    unsigned long index_;

    T * const beginPtr_;
    T * const endPtr_;

    void operator=(Buffer) {}
    Buffer(Buffer &): beginPtr_(&(array_[0])), endPtr_(&(array_[LENGTH-1])) {}

public:

    Buffer(): length_(LENGTH), index_(0), beginPtr_(&(array_[0])), endPtr_(&(array_[LENGTH-1])) {   }
    Buffer(T const &initValue): length_(LENGTH), index_(0), beginPtr_(&(array_[0])), endPtr_(&(array_[LENGTH-1])) { for (T &elem : array_) elem = initValue; }
    ~Buffer() {}

    inline T *ptrToBuffer() { return &(array_[0]); }

    inline T const &operator[](unsigned long i) const {
        if (i >= length_) throw -1;
        return array_[i];
    }
    inline T &operator[](unsigned long i) {
        if (i >= length_) throw -1;
        return array_[i];
    }

    inline unsigned long size() const { return length_; }
    inline unsigned long length() const { return length_; }
    inline unsigned long indexPosition() const { return index_; }

    inline void push(T const &element) { ++index_; if (index_ == length_) index_ = 0; array_[index_] = element; }

    inline T const &last() const { return array_[index_]; }
    inline T &last() { return array_[index_]; }

    inline T const &get(unsigned long i) const { return (index_ < i) ? array_[index_ + length_ -i] : array_[index_ -i]; }

    inline T const &operator()(unsigned long i) const { return (index_ < i) ? array_[index_ + length_ -i] : array_[index_ -i]; }
    inline T &operator()(unsigned long i) { return (index_ < i) ? array_[index_ + length_ -i] : array_[index_ -i]; }



    class iterator {
        friend class Buffer;

        T *ptr;
        Buffer *buffer;

        iterator(Buffer *buffer, T *ptr): buffer(buffer), ptr(ptr) {}

    public:

        iterator(): ptr(nullptr) {}
        ~iterator() {}

        inline iterator const &operator=(iterator const &it) { buffer = it.buffer; ptr = it.ptr; }

        inline T const &value() const { return *ptr; }
        inline T &value() { return *ptr; }

        inline T &operator*() { return *ptr; }

        inline iterator &operator++() { (ptr == buffer->beginPtr_) ? ptr = buffer->endPtr_ : --ptr; return *this; }
        inline iterator &operator--() { (ptr == buffer->endPtr_) ? ptr = buffer->beginPtr_ : ++ptr; return *this; }

    };

    inline iterator begin() { return iterator(this,&(array_[index_])); }


};


#endif