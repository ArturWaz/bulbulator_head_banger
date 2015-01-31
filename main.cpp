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
	bool read = false;
	while (!read){
		try {
			//uint8_t c = connection.readByte();
			uint8_t ch[30] = {0};
			//int readint = connection.readBlock(ch,20);
            ch[0] = connection.readByte();
			for (int i = 0; ch[i] != 0; ++i) {
                std::cout << std::endl << i << '\t' << (char) ch[i] << std::endl;
            }
			//if (readint > 0) read = true;
		} catch(bool e){}
	}
}

int main(){
	connection.open();

	std::thread test(readData);
	//Sleep(1000);
	connection.sendByte((uint8_t)'A');
    //readData();
	//Sleep(1000);
    sleep(1);

    uint8_t ch[] = "asdfa";
	connection.sendBlock(ch,30);
    //sleep(1);
    //readData();

	test.join();
	connection.close();
}
