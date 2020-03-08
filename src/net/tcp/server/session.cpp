#include "sephi/net/tcp/server/session.h"

#include "sephi/net/tcp/server/server.h"


using namespace std::placeholders;
using std::error_code;
using std::move;

using asio::buffer;
using asio::socket_base;


sephi::net::tcp::Session::Session(
    asio::ip::tcp::socket&& socket,
    Server& server,
    ServerPacketHandler packet_handler)
    : socket_{move(socket)},
      server_{server},
      packet_handler_{move(packet_handler)}
{}

void sephi::net::tcp::Session::start()
{
    do_read();
}

void sephi::net::tcp::Session::write(Chunk const& chunk)
{
    auto const write_in_progress{!sending_messages_.empty()};
    sending_messages_.emplace_back(chunk);
    if (!write_in_progress)
        do_write();
}

void sephi::net::tcp::Session::close()
{
    socket_.shutdown(socket_base::shutdown_both);
}


void sephi::net::tcp::Session::do_read()
{
    socket_.async_read_some(
        buffer(received_packet_),
        bind(&Session::handle_read, shared_from_this(), _1, _2));
}

void sephi::net::tcp::Session::handle_read(error_code const& ec, size_t size)
{
    if (ec) {
        server_.leave(shared_from_this(), ec);
        return;
    }

    packet_handler_(
        session_ptr_to_id(shared_from_this()),
        received_packet_.data(),
        size);
    do_read();
}

void sephi::net::tcp::Session::do_write()
{
    auto const& message{sending_messages_.front()};
    async_write(
        socket_,
        buffer(message.data(), message.size()),
        bind(&Session::handle_write, shared_from_this(), _1, _2));
}

void sephi::net::tcp::Session::handle_write(
    error_code const& ec, size_t /*size*/)
{
    if (ec) {
        server_.leave(shared_from_this(), ec);
        return;
    }

    sending_messages_.pop_front();
    if (!sending_messages_.empty())
        do_write();
}
