/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <base_UM7LT.h>
#include <DefineFunctions.h>
#include <serial/PortCOM.h>


#define PORT_NR 4


using boost::asio::ip::tcp;


PortCOM portCOM(PORT_NR,115200);


class session;

std::list<session*> sessionsList;
std::mutex sessionListMutex;


class session
        : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket)
            : socket_(std::move(socket))
    {
        sessionListMutex.lock();
        sessionsList.push_back(this);
        sessionListMutex.unlock();
//        std::cout << "Session opened.\n";
    }

    ~session(){
        sessionListMutex.lock();
        sessionsList.remove(this);
        sessionListMutex.unlock();
//        std::cout << "Session closed.\n";
    }

    void start()
    {
        do_read();
    }

    inline void writeData(uint8_t const *data, std::size_t length) {
        boost::asio::async_write(socket_, boost::asio::buffer(data, length),
                [this](boost::system::error_code ec, std::size_t){});
    }

private:
    void do_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                [this, self](boost::system::error_code ec, std::size_t length){});
        // todo implement later
    }

    void do_write(std::size_t length)
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                {
                    if (!ec)
                    {
                        do_read();
                    }
                });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class server
{
public:
    server(boost::asio::io_service& io_service, short port)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
              socket_(io_service)
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
                [this](boost::system::error_code ec)
                {
                    if (!ec)
                    {
                        std::make_shared<session>(std::move(socket_))->start();
                    }

                    do_accept();
                });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};





void sendingThread() {
    portCOM.open();

    uint8_t buffer[253];
    uint8_t bufferRead;

    base_UM7LT::Packet packets[10];
    uint8_t packetsRead;

    base_UM7LT um7;

    while (true) {

        bufferRead = portCOM.readBlock(buffer, 250);
        if (!bufferRead) continue;

        packetsRead = um7.parseData(buffer, bufferRead, packets, 10);
        if (!packetsRead) continue;

        sessionListMutex.lock();
        if (sessionsList.empty()) {
            sessionListMutex.unlock();
            continue;
        }

        for (int i = 0; i < packetsRead; ++i) {
            buffer[0] = 's';
            buffer[1] = 'n';
            buffer[2] = 'p';
            buffer[3] = packets[i].packetType;
            buffer[4] = packets[i].address;
            memcpy(&(buffer[5]), packets[i].data, (packets[i].dataLength+2)*sizeof(uint8_t));

            for (auto &elem : sessionsList) {
                elem->writeData(buffer, packets[i].dataLength+7);
            }
        }
        sessionListMutex.unlock();

    }

    portCOM.close();
}



int main(int argc, char* argv[])
{

    std::thread Thread(sendingThread);
    Thread.detach();

    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: bulbulator_on_board <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        server s(io_service, std::atoi(argv[1]));

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
