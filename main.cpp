/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#include "PortCOM.h"
#include <iostream>
#include <thread>

//#include <windows.h>
#include <unistd.h>



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

int main(){
	connection.open();

	std::thread test(readData);
	connection.sendByte((uint8_t)'A');
	//Sleep(1000);
    sleep(1);

    uint8_t ch[30] = {0x00};
    ch[0] = 'a'; ch[1] = 'b'; ch[2] = 'f'; ch[3] = 'c'; ch[4] = 'd';
	connection.sendBlock(ch,30);

	test.join();
	connection.close();

    return 0;
}
