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
#include <_base_UM7LT.h>
#include <DefineFunctions.h>


#define PORT_NR 4


using boost::asio::ip::tcp;


_base_UM7LT um7(PORT_NR);


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
    um7.turnOnThreadedRead();

    UM7_LT_packet packet;
    uint8_t data[100];

    while (true) {

        if (!um7.getLastPacket(packet)) {
            if (packet.data != NULL) {
                free(packet.data);
                packet.data = NULL;
            }
            continue;
        }

        if (packet.data == NULL) continue;

        data[0] = uint8_t('s');
        data[1] = uint8_t('n');
        data[2] = uint8_t('p');
        data[3] = packet.length;
        data[4] = packet.address;
        memcpy(&(data[5]), packet.data, packet.length*sizeof(uint8_t));

        sessionListMutex.lock();
        if (sessionsList.empty()) {
            sessionListMutex.unlock();
            if (packet.data != NULL) {
                free(packet.data);
                packet.data = NULL;
            }
            SLEEP_MS(1000);
            continue;
        }
        for (auto &elem : sessionsList) {
            elem->writeData(data,packet.length+5);
        }
        sessionListMutex.unlock();

        if (packet.data != NULL) {
            free(packet.data);
            packet.data = NULL;
        }
    }

    um7.turnOffThreadedRead();
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
