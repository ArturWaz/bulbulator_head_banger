/*
 * PortCOM.h
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#ifndef PORTCOM_H_
#define PORTCOM_H_

#include <cstdint>

class PortCOM {

	int portNumber;
	int baudrate;
	char *mode;

public:

	PortCOM(int portNumber, int baudrate);
	PortCOM(int portNumber, int baudrate, const char*mode);
	~PortCOM();

	void open();
	void close();

	void sendByte(uint8_t byte);
	uint8_t readByte();

	void sendBlock(uint8_t *buffer, uint8_t length);
	uint8_t readBlock(uint8_t *buffer, uint8_t length);


    int getPortNumber() const { return portNumber; }
    int getBaudrate() const { return baudrate; }
};

#endif /* PORTCOM_H_ */
