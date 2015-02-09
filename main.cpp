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
#include <RotationMatrix.h>
#include "RobotControl.h"
#include "queue"
#include "DefineFunctions.h"

using namespace std;


queue<int> que;
mutex mtx;

void threadTest(int nb){
    long i = 0;
    while(true){
//        mtx.lock();
        que.push(i);
        if (que.size() >= 10)
            que.pop();
        ++i;
//        mtx.unlock();
        SLEEP_MS(1000);
    }
}

void threadTest2(int nb){
    long i = 0;
    while(true){
//        mtx.lock();
        if (!que.empty()) que.pop();
//        mtx.unlock();
        SLEEP_MS(1000);
    }
}


int main(){

//    printf("sdf");
    UM7_LT test(16);
    test.threadedReading();
    while(true);

//    printf("hello");
//    printf("\r          ");
//    printf("\rbye %.6f",67.0);

//    EulerAngles euler;
//    RotationMatrix rot;
//
//    euler.toRotationMatrix(rot);
//
//
//    for (int i = 0; i < 9; ++i) {
//        cout<<rot[i]<<'\t';
//        if (!((i+1)%3)) cout << endl;
//    }



//    thread t1(threadTest,1);
////    SLEEP_MS(500);
////    thread t2(threadTest2,2);
//    while(true){
//        if (!que.empty()) cout<<que.front()<<endl;
//        else cout<<"empty\n";
//        SLEEP_MS(100);
//    }

    return 0;
}
