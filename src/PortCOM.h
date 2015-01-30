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

	void sendByte(std::uint8_t byte);
	std::uint8_t readByte();

	void sendBlock(std::uint8_t *buffer, std::uint8_t length);
	uint8_t readBlock(std::uint8_t *buffer, std::uint8_t length);

};

#endif /* PORTCOM_H_ */
