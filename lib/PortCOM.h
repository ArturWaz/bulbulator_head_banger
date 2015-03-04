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
		Exception(char const *info): info(info) {}
		virtual const char* what() const throw() { return info; }
	};

	PortCOM(int portNumber, int baudrate): portNumber(portNumber), baudrate(baudrate), mode(NULL), opened(false) {
		mode = new char[4];
		std::strcpy(mode,"8N1");
	}

	PortCOM(int portNumber, int baudrate, const char*mode): portNumber(portNumber), baudrate(baudrate), mode(NULL), opened(false) {
		mode = new char[strlen(mode)+1];
		std::strcpy(this->mode,mode);
	}

	~PortCOM(){
		this->close();
		free(mode);
	}

	void open() {
		if (RS232_OpenComport(portNumber, baudrate, mode) != 0) throw Exception("Cannot open COM port.");
		opened = true;
	}
	void close() {
		RS232_CloseComport(portNumber);
		opened = false;
	}

	void sendByte(uint8_t byte) {
		if (!opened) throw Exception("Port COM is closed.");
		if (RS232_SendByte(portNumber,(unsigned char)byte) < 0) throw Exception("Cannot send byte.");
	}
	uint8_t readByte() {
		if (!opened) throw Exception("Port COM is closed.");
		unsigned char c[] = {0x00};
		if (RS232_PollComport(portNumber, c, 1) <= 0) throw Exception("Cannot read byte.");
		return c[0];
	}

	void sendBlock(uint8_t *buffer, uint8_t length) {
		if (!opened) throw Exception("Port COM is closed.");
		if (RS232_SendBuf(portNumber, buffer, length) != length) throw Exception("Cannot send block.");
	}
	int readBlock(uint8_t *buffer, uint8_t length) {
		if (!opened) throw Exception("Port COM is closed.");
		int number = RS232_PollComport(portNumber, buffer, (unsigned char)length);
		if (number <= 0) throw Exception("Cannot read block.");
		return number;
	}

    int getPortNumber() const { return portNumber; }
    int getBaudrate() const { return baudrate; }
	char const * const getMode() const { return mode; }
	bool isOpen() const { return opened; }

};

#endif /* PORTCOM_H_ */
