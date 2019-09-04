#include "sephi/net/udp/udp.h"


using namespace std::placeholders;
using std::error_code;
using std::next;
using std::thread;

using asio::buffer;
using asio::ip::make_address;
using asio::ip::network_v4;
using asio::null_buffers;
using asio::socket_base;

using sephi::net::Remote;


namespace sephi::net::udp {
    
    Remote to_remote(endpoint const& ep)
    {
        return {ep.address().to_string(), ep.port()};
    }

}


sephi::net::udp::Udp::Udp(
    uint16_t port,
    CommunicationErrorHandler communication_error_handler,
    MessageHandler message_handler)
    : socket_{io_context_, asio::ip::udp::v4()},
      communication_error_handler_{move(communication_error_handler)},
      message_handler_{move(message_handler)}
{
    socket_.set_option(asio::socket_base::reuse_address{true});
    socket_.set_option(asio::socket_base::broadcast{true});
    socket_.bind(endpoint{asio::ip::udp::v4(), port});

    do_read();
    runner_ = std::thread{[this] {io_context_.run(); }};
}

sephi::net::udp::Udp::~Udp()
{
    io_context_.stop();
    runner_.join();
}

void sephi::net::udp::Udp::close()
{
    post(
        io_context_,
        [this] {socket_.shutdown(socket_base::shutdown_both); });
}

void sephi::net::udp::Udp::write_to(
    Remote const& remote, Chunk const& chunk)
{
    endpoint ep{make_address(remote.address()), remote.port()};
    socket_.async_send_to(
        buffer(chunk.data(), chunk.size()),
        ep,
        bind(&Udp::handle_write, this, ep, _1, _2));
}

void sephi::net::udp::Udp::broadcast(uint16_t port, Chunk const& chunk)
{
    static network_v4 net_obj;
    endpoint ep{net_obj.broadcast(), port};
    socket_.async_send_to(
        buffer(chunk.data(), chunk.size()),
        ep,
        bind(&Udp::handle_write, this, ep, _1, _2));
}


void sephi::net::udp::Udp::do_read()
{
    static endpoint ep;
    socket_.async_receive_from(
        null_buffers(),
        ep,
        bind(&Udp::handle_read, this, ep, _1, _2));
}

void sephi::net::udp::Udp::handle_read(
    endpoint& ep, error_code const& ec, size_t /*size*/)
{
    if (ec) {
        communication_error_handler_(to_remote(ep));
        do_read();
        return;
    }

    auto available{socket_.available()};
    auto const received_data{std::make_unique<uint8_t[]>(available)};
    auto read{static_cast<size_t>(0)};
    while (available -= read) {
        read = socket_.receive_from(
            buffer(received_data.get(), available), ep);
        message_handler_(
            to_remote(ep),
            {received_data.get(), next(received_data.get(), read)});
    }

    do_read();
}

void sephi::net::udp::Udp::handle_write
    (endpoint const& ep, error_code const& ec, size_t /*size*/) const
{
    if (ec) {
        communication_error_handler_(to_remote(ep));
    }
}
