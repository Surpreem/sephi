#pragma once


#include "sephi/net/tcp/client/client.h"


using namespace std::chrono_literals;
using std::placeholders::_1;
using std::placeholders::_2;


class ClientWrapper {
public:
    ClientWrapper(sephi::net::Remote const& remote)
        : client_{
            remote,
            std::bind(&ClientWrapper::connection_handler, this, _1),
            std::bind(&ClientWrapper::packet_handler, this, _1, _2)}
    {}
    ~ClientWrapper()
    {
        client_.close();
    }

    bool write(sephi::net::Message const& message)
    {
        return client_.write(message);
    }

    [[nodiscard]] bool is_connected() const
    {
        return client_.is_connected();
    }

    [[nodiscard]] bool are_all_messages_sent() const
    {
        return client_.are_all_messages_sent();
    }

    [[nodiscard]] size_t received_bytes() const
    {
        return received_bytes_;
    }

private:
    void connection_handler(sephi::net::ConnectionState connection_state)
    {
        if (sephi::net::ConnectionState::connected == connection_state) {
            return;
        }

        std::this_thread::sleep_for(1s);
        client_.reconnect();
    }

    void packet_handler(uint8_t const* /*packet*/, size_t size)
    {
        received_bytes_ += size;
    }


    sephi::net::tcp::Client client_;
    size_t received_bytes_{0};
};
