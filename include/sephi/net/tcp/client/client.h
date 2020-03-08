#pragma once


#include <deque>

#include "asio.hpp"

#include "sephi/net/types/types.h"


namespace sephi::net::tcp {

    class Client {
    public:
        Client(
            Remote const& remote,
            ClientConnectionHandler connection_handler,
            ClientPacketHandler packet_handler);
        ~Client() noexcept;

        void reconnect();
        void close();

        bool write(Chunk const& chunk);

        [[nodiscard]] bool is_connected() const;
        [[nodiscard]] bool are_all_messages_sent() const;

    private:
        void do_connect();
        void handle_connect(std::error_code const& ec);

        void do_read();
        void handle_read(std::error_code const& ec, size_t size);

        void do_write();
        void handle_write(std::error_code const& ec, size_t size);

        void call_back_connection_handler(
            ConnectionState state, std::error_code const& ec);


        ConnectionState connection_state_{ConnectionState::disconnected};

        std::thread runner_;
        asio::io_context io_context_;
        asio::ip::tcp::endpoint endpoint_;
        std::unique_ptr<asio::ip::tcp::socket> socket_;

        ClientConnectionHandler connection_handler_;
        ClientPacketHandler packet_handler_;

        PacketBuffer received_packet_{};
        std::deque<Chunk> sending_messages_;
    };

}
