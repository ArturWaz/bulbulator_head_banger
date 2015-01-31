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
#include "crossOSfunctions.h"

using namespace std;


PortCOM connection(16,9600);

void readData(){
	while (true){
		try {
			uint8_t ch[30] = {0};
			connection.readBlock(ch,30);
			for (int i = 0; ch[i] != 0; ++i) {
                std::cout << i << '\t' << (char) ch[i] << std::endl;
            }
		} catch(bool e){}
	}
}

void threadTest(int nb){
    long i = 0;
    while(true){
        std::cout << "Thread: " << nb << ", is working, iter: " << i++ << std::endl;
        SLEEP_MS(1000);
    }
}

int main(){

    EulerAnglesTime test(5.5,6.5,5.7,8.9);

    cout<<"Angles+Time: ("<<test.getPhi()<<", "<<test.getTheta()<<", "<<test.getPsi()<<", "<<test.getTime()<<"); \n";

    return 0;
}
