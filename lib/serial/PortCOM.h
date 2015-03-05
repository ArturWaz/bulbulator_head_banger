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
#include <exception>

#include "rs232.h"



class PortCOM {

	int portNumber;
	int baudrate;
	char *mode;
	bool opened;

	PortCOM(PortCOM&) {}
	void operator=(PortCOM) {}

public:

	class Exception : public std::exception {
		char const *info;
	public:
		explicit Exception(char const *info): info(info) {}
		virtual char const *what() const throw() { return info; }
	};

	PortCOM(int const portNumber, int const baudrate): portNumber(portNumber), baudrate(baudrate), opened(false) {
		PortCOM::mode = (char*)malloc(4*sizeof(char));
		std::strcpy(PortCOM::mode,"8N1");
	}
	PortCOM(int const portNumber, int const baudrate, char const *mode): portNumber(portNumber), baudrate(baudrate), opened(false) {
		PortCOM::mode = (char*)malloc((strlen(mode)+1)*sizeof(char));
		std::strcpy(PortCOM::mode,mode);
	}
	~PortCOM() {
		this->close();
		free(mode);
	}

	void open() {
		if (RS232_OpenComport(portNumber, baudrate, mode) != 0) throw Exception("Cannot open port, function PortCOM::open()");
		opened = true;
	}
	void close() {
		RS232_CloseComport(portNumber);
		opened = false;
	}

	void sendByte(uint8_t byte) {
		if (!opened) throw Exception("Port is closed, function: PortCOM::sendByte(uint8_t)");
		if (RS232_SendByte(portNumber,(unsigned char)byte) < 0) throw Exception("Cannot send byte, function: PortCOM::sendByte(uint8_t)");
	}
	uint8_t readByte() {
		if (!opened) throw Exception("Port is closed, function: PortCOM::readByte()");
		unsigned char c[] = {0x00};
		if (RS232_PollComport(portNumber, c, 1) <= 0) throw Exception("Cannot read byte, function: PortCOM::readByte()");
		return c[0];
	}

	void sendBlock(uint8_t *buffer, uint8_t const length) {
		if (!opened) throw Exception("Port is closed, function PortCOM::sendBlock(uint8_t*,uint8_t)");
		if (RS232_SendBuf(portNumber, buffer, length) != length) throw Exception("Cannot send block, function: PortCOM::sendBlock(uint8_t*,uint8_t)");
	}
	int readBlock(uint8_t *buffer, uint8_t const length) {
		if (!opened) throw Exception("Port is closed, function: PortCOM::readBlock(uint8_t*,uint8_t)");
		int number = RS232_PollComport(portNumber, buffer, (unsigned char)length);
		if (number < 0) throw Exception("Cannot read block, function: PortCOM::readBlock(uint8_t*,uint8_t)");
		return number;
	}

	inline int getPortNumber() const { return portNumber; }
	inline int getBaudrate() const { return baudrate; }
	inline char const * const getMode() const { return mode; }
	inline bool isOpen() const { return opened; }

};

#endif /* PORTCOM_H_ */
