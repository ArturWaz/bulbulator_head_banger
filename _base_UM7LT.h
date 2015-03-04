/*
 * _base_UM7LT.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef _base_UM7LT_H_
#define _base_UM7LT_H_


#include <lmerrlog.h>
#include <midles.h>

class _base_UM7LT {

    static uint8_t const maxLength = 80;
    uint8_t const *endPtr;

    uint8_t data[maxLength+1];
    uint8_t *actualPtr;

public:

    _base_UM7LT(): actualPtr(&(data[0])), endPtr(&(data[maxLength])) {}

    void parseData(uint8_t const *buffer, uint8_t length) {
        for (uint8_t *i = 0; i != &(buffer[length]); ++i) {

        }
    }

};


#endif //__UM7_LT_H_