#pragma once


#include "sephi/net/udp/udp.h"


using namespace std::chrono_literals;
using std::placeholders::_1;
using std::placeholders::_2;


class UdpWrapper {
public:
    UdpWrapper(
        uint16_t port, sephi::net::udp::MessageHandler const& message_handler)
        : udp_{
            port,
            bind(&UdpWrapper::communication_error_handler, this, _1),
            message_handler}
    {}
    virtual ~UdpWrapper() = default;

    void write_to(
        sephi::net::Remote const& remote, sephi::net::Chunk const& chunk)
    {
        udp_.write_to(remote, chunk);
    }

    void broadcast(uint16_t port, sephi::net::Chunk const& chunk)
    {
        udp_.broadcast(port, chunk);
    }

    void close()
    {
        udp_.close();
    }

    [[nodiscard]] size_t received_bytes() const
    {
        return received_bytes_;
    }

protected:
    size_t received_bytes_{0};

private:
    void communication_error_handler(sephi::net::Remote const& /*remote*/)
    {}


    sephi::net::udp::Udp udp_;
};

class UdpServer : public UdpWrapper {
public:
    UdpServer(uint16_t port)
        : UdpWrapper{port, bind(&UdpServer::message_handler, this, _1, _2)}
    {}
    ~UdpServer() override = default;

private:
    void message_handler(
        sephi::net::Remote const& remote, sephi::net::Chunk const& chunk)
    {
        received_bytes_ += chunk.size();
        write_to(remote, chunk);
    }
};

class UdpClient : public UdpWrapper {
public:
    UdpClient(uint16_t port)
        : UdpWrapper{port, bind(&UdpClient::message_handler, this, _1, _2)}
    {}
    ~UdpClient() override = default;

private:
    void message_handler(
        sephi::net::Remote const& /*remote*/,
        sephi::net::Chunk const& chunk)
    {
        received_bytes_ += chunk.size();
    }
};
