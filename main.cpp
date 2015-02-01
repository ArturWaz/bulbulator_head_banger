/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include <thread>
#include <EulerAngles.h>
#include <UM7LT.h>

#include "PortCOM.h"
#include "DefineFunctions.h"

using namespace std;



void threadTest(int nb){
    long i = 0;
    while(true){
        std::cout << "Thread: " << nb << ", is working, iter: " << i++ << std::endl;
        SLEEP_MS(1000);
    }
}

int main(){

    UM7_LT test(16);

    test.threadedReading();

    while(true);

    return 0;
}
