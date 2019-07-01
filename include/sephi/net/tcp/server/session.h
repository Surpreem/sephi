#pragma once


#include <deque>

#include "asio.hpp"

#include "sephi/net/types/types.h"


namespace sephi::net::tcp {

    class Server;

    class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(
            asio::ip::tcp::socket&& socket,
            Server& server,
            ServerPacketHandler packet_handler);
        ~Session() noexcept = default;

        void start();
        void write(Message const& message);

        void close();

    private:
        void do_read();
        void handle_read(std::error_code const& ec, size_t size);

        void do_write();
        void handle_write(std::error_code const& ec, size_t size);

        asio::ip::tcp::socket socket_;
        Server& server_;
        ServerPacketHandler packet_handler_;

        PacketBuffer received_packet_{};
        std::deque<Message> sending_messages_;
    };

}
