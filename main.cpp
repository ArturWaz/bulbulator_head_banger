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


union convert {
    float f;
    uint32_t i;
};


int main(){

//    convert c;
//    c.f = 1;
//
//    cout<<hex<<c.i<<endl;




    UM7_LT test(4);
    test.threadedReading();
    while(true);



//    RotationMatrix rot;
//    RotationMatrix rot1;
//    RotationMatrix rot2;
//
//
//    for (int i = 0; i < 9; ++i) {
//        rot1[i] = i;
//        rot2[i] = i;
//    }
//
//    rot = rot1*rot2;
//
//    for (int i = 0; i < 9; ++i) {
//        cout<<rot[i]<<'\t';
//        if (!((i+1)%3)) cout << endl;
//    }
//    cout << endl;
//    for (int i = 0; i < 3; ++i) {
//        for (int j = 0; j < 3; ++j) {
//            cout<<rot(i,j)<<'\t';
//        }
//        cout << endl;
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
