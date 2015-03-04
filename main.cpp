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

    PortCOM t(4,11520);



    t.sendByte(45);

    return 0;
}
