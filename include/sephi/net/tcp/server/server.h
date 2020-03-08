#pragma once


#include "asio.hpp"

#include "sephi/net/types/types.h"


namespace sephi::net::tcp {

    class Session;

    using SessionPtr = std::shared_ptr<Session>;

    SessionId session_ptr_to_id(SessionPtr const& session_ptr);


    class Server {
    public:
        Server(
            uint16_t port,
            ServerConnectionHandler connection_handler,
            ServerPacketHandler packet_handler);
        ~Server() noexcept;

        void write_to_all(Chunk const& chunk);
        bool write_to(SessionId session_id, Chunk const& chunk);
        void write_to_all_except(
            SessionId session_id, Chunk const& chunk);

        void leave(SessionPtr session, std::error_code const& ec);
        void close();

    private:
        void do_accept();
        void handle_accept(std::error_code const& ec);


        std::thread runner_;
        asio::io_context io_context_;
        asio::ip::tcp::socket socket_;
        std::unique_ptr<asio::ip::tcp::acceptor> acceptor_;

        ServerConnectionHandler connection_handler_;
        ServerPacketHandler packet_handler_;

        std::unordered_map<SessionId, SessionPtr> sessions_;
    };

}
