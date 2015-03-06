/*
 * Buffer_UNIT_TESTS.h
 *
 *  Created on: 3/6/2015
 *      Author: Artur
 */


#ifndef Buffer_UNIT_TESTS_H_
#define Buffer_UNIT_TESTS_H_


#include "Buffer.h"

namespace Buffer_UINT_TESTS {
    using namespace std;

    template <class T, unsigned long LENGTH>
    void showBuffer(Buffer<T,LENGTH> const &buf) {
        cout << "Length: " << buf.size() << endl;
        for (int i = 0; i < buf.size(); ++i) {
            cout << buf(i) << " ";
        }
        cout << endl;
    }


    void iteratortest() {
        Buffer<int,9> b;
        for (int i = 0; i < b.size(); ++i) {
            b.push(i);
        }


        showBuffer(b);

        Buffer<int,9>::iterator it = b.begin();
        for (int j = 0; j < b.size()*2; ++j) {
            cout << *(--it) << " ";
//            ++it;
        }
        cout << endl;
    }


}


#endif //__Buffer_UNIT_TESTS_H_