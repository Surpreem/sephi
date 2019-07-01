#pragma once


#include "sephi/net/tcp/server/server.h"


using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;


class ServerWrapper {
public:
    explicit ServerWrapper(uint16_t port)
        : server_{
            port,
            std::bind(&ServerWrapper::connection_handler, this, _1, _2),
            std::bind(&ServerWrapper::message_handler, this, _1, _2, _3)}
    {}
    ~ServerWrapper()
    {
        server_.close();
    }

private:
    void connection_handler(
        sephi::net::tcp::SessionId /*id*/,
        sephi::net::ConnectionState /*connection_state*/)
    {}

    void message_handler(
        sephi::net::tcp::SessionId id, uint8_t const* packet, size_t size)
    {
        total_received_bytes_ += size;
        server_.write_to(id, sephi::net::Message{packet, size});
    }


    sephi::net::tcp::Server server_;

    size_t total_received_bytes_{0};
};
