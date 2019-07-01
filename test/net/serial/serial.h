#pragma once


#include "sephi/net/serial/serial.h"


using namespace std::chrono_literals;
using std::placeholders::_1;
using std::placeholders::_2;


class SerialWrapper {
public:
    SerialWrapper(
        std::string port_name,
        sephi::net::serial::SerialParams params,
        sephi::net::serial::DataHandler const& data_handler)
        : serial_{
            move(port_name),
            params,
            std::bind(&SerialWrapper::connection_handler, this, _1),
            data_handler}
    {}
    virtual ~SerialWrapper() = default;

    bool write(sephi::net::Message const& message)
    {
        return serial_.write(message);
    }

    void close()
    {
        serial_.close();
    }

    [[nodiscard]] bool is_opened() const
    {
        return serial_.is_opened();
    }

    [[nodiscard]] bool are_all_messages_sent() const
    {
        return serial_.are_all_messages_sent();
    }

    [[nodiscard]] size_t received_bytes() const
    {
        return received_bytes_;
    }

protected:
    size_t received_bytes_{0};

private:
    void connection_handler(sephi::net::ConnectionState connection_state)
    {
        if (sephi::net::ConnectionState::connected == connection_state) {
            return;
        }

        std::this_thread::sleep_for(1s);
        serial_.reopen();
    }


    sephi::net::serial::Serial serial_;
};

class SerialServer : public SerialWrapper {
public:
    SerialServer(
        std::string port_name, sephi::net::serial::SerialParams params)
        : SerialWrapper{
            move(port_name),
            params,
            std::bind(&SerialServer::data_handler, this, _1, _2)}
    {}
    ~SerialServer() override = default;

private:
    void data_handler(uint8_t const* data, size_t size)
    {
        received_bytes_ += size;
        write(sephi::net::Message{data, size});
    }
};

class SerialClient : public SerialWrapper {
public:
    SerialClient(
        std::string port_name, sephi::net::serial::SerialParams params)
        : SerialWrapper{move(port_name),
            params,
            std::bind(&SerialClient::data_handler, this, _1, _2)}
    {}
    ~SerialClient() override = default;

private:
    void data_handler(uint8_t const* /*data*/, size_t size)
    {
        received_bytes_ += size;
    }
};
