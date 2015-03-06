/*
 * ClientTCP.h
 *
 *  Created on: 3/5/2015
 *      Author: Artur
 */


#ifndef ClientTCP_H_
#define ClientTCP_H_

#include <boost/asio.hpp>



class ClientTCP {

    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket s;
    boost::asio::ip::tcp::resolver resolver;

    char const *server;
    char const *port;

public:

    ClientTCP(char const *server, char const *port): s(io_service), resolver(io_service), server(server), port(port) {}
    ~ClientTCP() { close(); }

    inline void open() { boost::asio::connect(s, resolver.resolve({server, port})); }
    inline void close() {}

    inline uint8_t readBlock(uint8_t *buffer, uint8_t const length) { return (uint8_t)boost::asio::read(s,boost::asio::buffer(buffer, length)); }
    inline void sendBlock(uint8_t const *buffer, uint8_t const length) { boost::asio::write(s, boost::asio::buffer(buffer, length)); }

};


#endif //__ClientTCP_H_