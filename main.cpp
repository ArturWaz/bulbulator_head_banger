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



PortCOM connection(4,9600);

void readData(){
	bool read = false;
	while (!read){
		try {
			//std::uint8_t c = connection.readByte();
			uint8_t ch[30];
			int read = connection.readBlock(ch,30);
			for (int i = 0; i < read; ++i)
				std::cout<<std::endl<<(char)ch[i]<<std::endl;
//			read = true;
		} catch(bool e){}
	}
}

int main(){
	connection.open();

	std::thread test(readData);
	//Sleep(1000);
	connection.sendByte((std::uint8_t)'A');
	//Sleep(1000);
    std::uint8_t ch[] = "asdfasdf\nsdfsdf";
	connection.sendBlock(ch,30);

	test.join();
	connection.close();
}
