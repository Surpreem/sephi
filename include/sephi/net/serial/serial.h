#pragma once


#include <deque>

#include "asio.hpp"

#include "sephi/net/types/types.h"
#include "sephi/net/serial/serial_params.h"


namespace sephi::net::serial {

    class Serial {
    public:
        Serial(
            std::string port_name,
            SerialParams params,
            ConnectionHandler connection_handler,
            DataHandler data_handler);
        ~Serial();

        void reopen();
        void close();
        std::error_code flush() const;

        bool write(Chunk const& chunk);

        [[nodiscard]] bool is_opened() const;
        [[nodiscard]] bool are_all_messages_sent() const;

    private:
        void do_open();
        void handle_open(std::error_code const& ec);
        
        void do_read();
        void handle_read(std::error_code const& ec, size_t size);

        void do_write();
        void handle_write(std::error_code const& ec, size_t size);

        void call_back_connection_handler(
            ConnectionState state, std::error_code const& ec);


        std::string port_name_;
        SerialParams params_;
        ConnectionState connection_state_{ConnectionState::disconnected};
		
        std::thread runner_;
        asio::io_context io_context_;
        std::unique_ptr<asio::serial_port> port_;

        ConnectionHandler connection_handler_;
        DataHandler data_handler_;

        PacketBuffer received_data_{};
        std::deque<Chunk> sending_messages_;
    };

}
