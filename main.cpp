/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include <thread>
#include <UM7LT.h>
#include <mutex>
#include "RobotControl.h"
#include "DefineFunctions.h"

using namespace std;


mutex mut;
mutex mut1;

void threadTest(int nb){
    mut.lock();
    long i = 0;
    while(true){
        std::cout << "Thread: " << nb << ", is working, iter: " << i++ << std::endl;
        SLEEP_MS(1000);
    }
    mut.unlock();
}

void threadTestMain(){
    mut1.lock();
    long i = 0;
    while(true){
        std::cout << "Main" << " is working, iter: " << i++ << std::endl;
        SLEEP_MS(1000);
    }
    mut.unlock();
}


int main(){

    UM7_LT test(4);
    test.threadedReading();
    while(true);



//    thread t1(threadTest,1);
//    thread t2(threadTest,2);
//
//    SLEEP_MS(1000);
//    threadTestMain();

    return 0;
}
