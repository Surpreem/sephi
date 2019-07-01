#pragma once


#include <string>

#include "asio.hpp"


namespace sephi::net {

    class Remote {
    public:
        Remote(std::string const& address, uint16_t port)
            : address_{address}, port_{port}
        {}

        Remote(Remote const&) = default;
        Remote(Remote&&) noexcept = default;

        ~Remote() noexcept = default;

        Remote& operator=(Remote const&) = default;
        Remote& operator=(Remote&&) noexcept = default;


        void address(std::string const& address)
        {
            address_ = address;
        }

        [[nodiscard]] std::string address() const
        {
            return address_;
        }

        void port(uint16_t port)
        {
            port_ = port;
        }

        [[nodiscard]] uint16_t port() const
        {
            return port_;
        }

    private:
        std::string address_;
        uint16_t port_;
    };

}
