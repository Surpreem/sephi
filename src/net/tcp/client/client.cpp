#include "sephi/net/tcp/client/client.h"


using namespace std::placeholders;
using std::error_code;
using std::make_unique;
using std::string;
using std::thread;

using asio::buffer;
using asio::ip::make_address;
using asio::socket_base;


sephi::net::tcp::Client::Client(
    Remote const& remote,
    ClientConnectionHandler connection_handler,
    ClientPacketHandler packet_handler)
    : endpoint_{make_address(remote.address()), remote.port()},
      connection_handler_{move(connection_handler)},
      packet_handler_{move(packet_handler)}
{
    do_connect();
    runner_ = thread{[this] {io_context_.run(); }};
}

sephi::net::tcp::Client::~Client()
{
    close();
    io_context_.stop();
    runner_.join();
}

void sephi::net::tcp::Client::reconnect()
{
    socket_->close();
    do_connect();
}

void sephi::net::tcp::Client::close()
{
    if (is_connected()) {
        connection_state_= ConnectionState::disconnected;
        post(
            io_context_,
            [this] {socket_->shutdown(socket_base::shutdown_both); });
    }
}

bool sephi::net::tcp::Client::write(Chunk const& chunk)
{
    if (!is_connected())
        return false;

    post(
        io_context_,
        [this, chunk] {
            auto const write_in_progress{!sending_messages_.empty()};
            sending_messages_.emplace_back(chunk);
            if (!write_in_progress)
                do_write();
        });
    return true;
}

inline bool sephi::net::tcp::Client::is_connected() const
{
    return ConnectionState::connected == connection_state_;
}

bool sephi::net::tcp::Client::are_all_messages_sent() const
{
    return sending_messages_.empty();
}


void sephi::net::tcp::Client::do_connect()
{
    connection_state_ = ConnectionState::disconnected;
    socket_ = make_unique<asio::ip::tcp::socket>(io_context_);
    socket_->async_connect(
        endpoint_, bind(&Client::handle_connect, this, _1));
}

void sephi::net::tcp::Client::handle_connect(error_code const& ec)
{
    if (ec) {
        call_back_connection_handler(ConnectionState::disconnected, ec);
        return;
    }

    call_back_connection_handler(ConnectionState::connected, ec);
    do_read();
}

void sephi::net::tcp::Client::do_read()
{
    socket_->async_read_some(
        buffer(received_packet_),
        bind(&Client::handle_read, this, _1, _2));
}

void sephi::net::tcp::Client::handle_read(error_code const& ec, size_t size)
{
    if (ec) {
        call_back_connection_handler(ConnectionState::disconnected, ec);
        return;
    }

    packet_handler_(received_packet_.data(), size);
    do_read();
}

void sephi::net::tcp::Client::do_write()
{
    auto const& message{sending_messages_.front()};
    async_write(
        *socket_,
        buffer(message.data(), message.size()),
        bind(&Client::handle_write, this, _1, _2));
}

void sephi::net::tcp::Client::handle_write(
    error_code const& ec, size_t /*size*/)
{
    if (ec) {
        call_back_connection_handler(ConnectionState::disconnected, ec);
        return;
    }

    sending_messages_.pop_front();
    if (!sending_messages_.empty())
        do_write();
}

void sephi::net::tcp::Client::call_back_connection_handler(
    ConnectionState state, error_code const& ec)
{
    connection_state_ = state;
    connection_handler_(connection_state_, ec);
}
