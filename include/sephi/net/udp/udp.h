#pragma once


#include <deque>

#include "asio.hpp"

#include "sephi/net/types/types.h"


namespace sephi::net::udp {

    using endpoint = asio::ip::udp::endpoint;


    class Udp {
    public:
        Udp(
            uint16_t port,
            CommunicationErrorHandler communication_error_handler,
            MessageHandler message_handler);
        ~Udp();

        void close();

        void write_to(Remote const& remote, Chunk const& chunk);
        void broadcast(uint16_t port, Chunk const& chunk);

    private:
        void do_read();
        void handle_read(endpoint& ep, std::error_code const& ec, size_t size);

        void handle_write(
            endpoint const& ep, std::error_code const& ec, size_t size) const;


        std::thread runner_;
        asio::io_context io_context_;
        asio::ip::udp::socket socket_;

        CommunicationErrorHandler communication_error_handler_;
        MessageHandler message_handler_;
    };

}
