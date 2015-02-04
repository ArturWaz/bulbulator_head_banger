/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include <thread>
#include <UM7LT.h>
#include "RobotControl.h"
#include "DefineFunctions.h"


using namespace std;

void threadTest(int nb){
    long i = 0;
    while(true){
        std::cout << "Thread: " << nb << ", is working, iter: " << i++ << std::endl;
        SLEEP_MS(1000);
    }
}


struct Test{

    void test();
    void test1();

};

#define STR(x) #x
#define TEST(x) \
    void Test::x(){ \
        cout << STR(x) << endl; \
    }


TEST(test)
TEST(test1)


int main(){

    UM7_LT test(16);
    test.threadedReading();
    while(true);


    uint8_t byte1 = 0b00001111;
    uint8_t byte2 = 0b11001100;

    cout<<hex<< (byte1^byte2) <<endl;



    return 0;
}
