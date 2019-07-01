#include "sephi/net/serial/serial.h"


using namespace std::placeholders;
using std::error_code;
using std::make_unique;
using std::thread;
using std::string;

using asio::buffer;


sephi::net::serial::Serial::Serial(
    string port_name,
    SerialParams params,
    ConnectionHandler connection_handler,
    DataHandler data_handler)
    : port_name_{move(port_name)},
      params_{params},
      connection_handler_{std::move(connection_handler)},
      data_handler_{move(data_handler)}
{
    do_open();
    runner_ = thread{[this] {io_context_.run();}};
}

sephi::net::serial::Serial::~Serial()
{
    close();
    io_context_.stop();
    runner_.join();
}

void sephi::net::serial::Serial::reopen()
{
    close();
    do_open();
}

void sephi::net::serial::Serial::close()
{
    connection_state_ = ConnectionState::disconnected;
    post(io_context_, [this] {port_->close();});
}

error_code sephi::net::serial::Serial::flush() const
{
    error_code ec;
    bool is_flushed{false};
#if !defined(_WIN32) && !defined(__CYGWIN__)
    is_flushed = !::tcflush(port_->native_handle(), TCIOFLUSH);
    if (!is_flushed)
        ec = error_code{
            errno,
            asio::error::get_system_category()};
#else
    is_flushed = ::PurgeComm(
        port_->native_handle(),
        PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
    if (!is_flushed)
        ec = error_code{
            static_cast<int>(::GetLastError()),
            asio::error::get_system_category()};
#endif
    return ec;
}

bool sephi::net::serial::Serial::write(Message const& message)
{
    if (!is_opened())
        return false;

    post(
        io_context_,
        [this, message] {
            auto const write_in_progress{!sending_messages_.empty()};
            sending_messages_.emplace_back(message);
            if (!write_in_progress)
                do_write();
        });
    return true;
}

bool sephi::net::serial::Serial::is_opened() const
{
    return ConnectionState::connected == connection_state_;
}

bool sephi::net::serial::Serial::are_all_messages_sent() const
{
    return sending_messages_.empty();
}


void sephi::net::serial::Serial::do_open()
{
    connection_state_ = ConnectionState::disconnected;
    port_ = make_unique<asio::serial_port>(io_context_);
    post(
        io_context_,
        [this] {
            error_code ec;
            port_->open(port_name_, ec);
            handle_open(ec);
        });
}

void sephi::net::serial::Serial::handle_open(error_code const& ec)
{
    if (ec) {
        call_back_connection_handler(ConnectionState::disconnected);
        return;
    }

    port_->set_option(params_.baud_rate());
    port_->set_option(params_.character_size());
    port_->set_option(params_.stop_bits());
    port_->set_option(params_.parity());
    port_->set_option(params_.flow_control());

    auto const err_code{flush()};
    if (err_code) {
        call_back_connection_handler(ConnectionState::disconnected);
        return;
    }

    call_back_connection_handler(ConnectionState::connected);
    do_read();
}

void sephi::net::serial::Serial::do_read()
{
    port_->async_read_some(
        buffer(received_data_),
        bind(&Serial::handle_read, this, _1, _2));
}

void sephi::net::serial::Serial::handle_read(error_code const& ec, size_t size)
{
    if (ec) {
        call_back_connection_handler(ConnectionState::disconnected);
        return;
    }

    data_handler_(received_data_.data(), size);
    do_read();
}

void sephi::net::serial::Serial::do_write()
{
    auto const& message{sending_messages_.front()};
    async_write(
        *port_,
        buffer(message.data(), message.size()),
        bind(&Serial::handle_write, this, _1, _2));
}

void sephi::net::serial::Serial::handle_write(
    error_code const& ec, size_t /*size*/)
{
    if (ec) {
        call_back_connection_handler(ConnectionState::disconnected);
        return;
    }

    sending_messages_.pop_front();
    if (!sending_messages_.empty())
        do_write();
}

void sephi::net::serial::Serial::call_back_connection_handler(
    ConnectionState state)
{
    connection_state_ = state;
    connection_handler_(connection_state_);
}
