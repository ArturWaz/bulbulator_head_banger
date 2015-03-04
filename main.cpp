/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include "PortCOM.h"

using namespace std;



int main(){

    PortCOM t(0,115200);

    t.open();

    t.sendByte(45);

    return 0;
}
