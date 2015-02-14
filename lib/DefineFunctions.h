/*
 * crossOSfunctions.h
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#ifndef CROSSOSFUNCTIONS_H_
#define CROSSOSFUNCTIONS_H_

#ifdef __linux__

#include <unistd.h>
#define SLEEP_MS(_miliseconds) (usleep(1000*_miliseconds))

#else

#include <windows.h>
#define SLEEP_MS(_miliseconds) (Sleep(_miliseconds))

#endif

#define ERROR_COM(portNr,string) (std::cerr<<"\tERROR: port: "<<portNr<<", message: "<<string<<std::endl)

#endif /* CROSSOSFUNCTIONS_H_ */
