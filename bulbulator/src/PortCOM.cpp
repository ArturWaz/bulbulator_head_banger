/*
 * PortCOM.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#include <PortCOM.h>
#include "rs232.h"
#include <cstring>



PortCOM::PortCOM(int portNumber, int baudrate): portNumber(portNumber), baudrate(baudrate), mode(NULL) {
	mode = new char[4];
	strcpy(mode,"8N1");
}

PortCOM::PortCOM(int portNumber, int baudrate, const char*mode): portNumber(portNumber), baudrate(baudrate), mode(NULL) {
	mode = new char[strlen(mode)];
	strcpy(this->mode,mode);
}

PortCOM::~PortCOM(){ delete[] mode; }


void PortCOM::open(){ RS232_OpenComport(portNumber, baudrate, mode); }
void PortCOM::close(){ RS232_CloseComport(portNumber); }


void PortCOM::sendByte(std::uint8_t byte){	if (RS232_SendByte(portNumber,(unsigned char)byte) < 0) throw false; }
std::uint8_t PortCOM::readByte(){
	unsigned char c[] = {0x00};
	if (!RS232_PollComport(portNumber, c, 1)) throw false;
	return c[0];
}


void PortCOM::sendBlock(std::uint8_t *buffer, std::uint8_t length){
	if (RS232_SendBuf(portNumber, buffer, length) != length) throw false;
}
uint8_t PortCOM::readBlock(std::uint8_t *buffer, std::uint8_t length){
	uint8_t number = RS232_PollComport(portNumber, buffer, (unsigned char)length);
	if (number < 0) throw false;
	return number;
}

