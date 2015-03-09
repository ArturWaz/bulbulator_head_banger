/*
 * Buffer.h
 *
 *  Created on: 15.02.15
 *      Author: artur
 */


#ifndef Buffer_H_
#define Buffer_H_


#include <cstdlib>


template <typename T/*, size_t LENGTH*/>
class Buffer {

    T * /*const*/ array_;
    size_t length_;
    size_t index_;

    T * /*const*/ beginPtr_; // todo remove this pointer, array_ is sufficient actually
    T * /*const*/ endPtr_;

    void operator=(Buffer) {}
    Buffer(Buffer &): beginPtr_(nullptr), endPtr_(nullptr) {}

public:

    class Exception : public std::exception {
        char const *info;
    public:
        explicit Exception(char const *info): info(info) {}
        virtual char const *what() const throw() { return info; }
    };

    Buffer(size_t length): length_(length), index_(0)/*, beginPtr_(&(array_[0])), endPtr_(&(array_[LENGTH-1]))*/ {
        array_ = (T*)malloc(length_*sizeof(T));
        if (array_ == nullptr) throw Exception("Cannot allocate memory, function: Buffer::Buffer(size_t)");
        beginPtr_ = array_;
        endPtr_ = &(array_[length_-1]);
    }
    Buffer(size_t length, T const &initValue): length_(length), index_(0) {
        array_ = (T*)malloc(length_*sizeof(T));
        if (array_ == nullptr) throw Exception("Cannot allocate memory, function: Buffer::Buffer(size_t, T const &)");
        beginPtr_ = array_;
        endPtr_ = &(array_[length_-1]);
        T *ptr = beginPtr_;
        while (ptr != endPtr_) {
            *ptr = initValue;
            ++ptr;
        }
    }
    ~Buffer() { free(array_); }

    inline T *ptrToBuffer() { return &(array_[0]); }

    inline T const &operator[](size_t i) const {
        if (i >= length_) throw Exception("Out of range error, function: Buffer::operator[](size_t)");
        return array_[i];
    }
    inline T &operator[](size_t i) {
        if (i >= length_) throw Exception("Out of range error, function: Buffer::operator[](size_t)");
        return array_[i];
    }

    inline size_t size() const { return length_; }
    inline size_t length() const { return length_; }
    inline size_t indexPosition() const { return index_; }

    inline void push(T const &element) { ++index_; if (index_ == length_) index_ = 0; array_[index_] = element; }

    inline T const &last() const { return array_[index_]; }
    inline T &last() { return array_[index_]; }

    inline T const &get(size_t i) const { return (index_ < i) ? array_[index_ + length_ -i] : array_[index_ -i]; }

    inline T const &operator()(size_t i) const { return (index_ < i) ? array_[index_ + length_ -i] : array_[index_ -i]; }
    inline T &operator()(size_t i) { return (index_ < i) ? array_[index_ + length_ -i] : array_[index_ -i]; }



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