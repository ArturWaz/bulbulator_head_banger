/*
 * PortCOM.h
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#ifndef PORTCOM_H_
#define PORTCOM_H_

#include <cstdint>
#include <cstring>
#include "rs232.h"



class PortCOM {

	int portNumber;
	int baudrate;
	char *mode;

	PortCOM(PortCOM&) {}
	void operator=(PortCOM) {}

public:

	PortCOM(int portNumber, int baudrate): portNumber(portNumber), baudrate(baudrate), mode(NULL) {
		mode = new char[4];
		std::strcpy(mode,"8N1");
	}

	PortCOM(int portNumber, int baudrate, const char*mode): portNumber(portNumber), baudrate(baudrate), mode(NULL) {
		mode = new char[strlen(mode)+1];
		std::strcpy(this->mode,mode);
	}

	~PortCOM(){
		close();
		free(mode);
	}

	void open() { RS232_OpenComport(portNumber, baudrate, mode); }
	void close() { RS232_CloseComport(portNumber); }

	void sendByte(uint8_t byte) { if (RS232_SendByte(portNumber,(unsigned char)byte) < 0) throw false; }
	uint8_t readByte() {
		unsigned char c[] = {0x00};
		if (RS232_PollComport(portNumber, c, 1) <= 0) throw false;
		return c[0];
	}

	void sendBlock(uint8_t *buffer, uint8_t length) { if (RS232_SendBuf(portNumber, buffer, length) != length) throw false;	}
	uint8_t readBlock(uint8_t *buffer, uint8_t length) {
		uint8_t number = RS232_PollComport(portNumber, buffer, (unsigned char)length);
		if (number <= 0) throw false;
		return number;
	}

    int getPortNumber() const { return portNumber; }
    int getBaudrate() const { return baudrate; }
	char const * const getMode() const { return mode; }
};

#endif /* PORTCOM_H_ */
