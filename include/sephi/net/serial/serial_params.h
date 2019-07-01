#pragma once


#include "asio.hpp"


namespace sephi::net::serial {

    enum class BaudRate {
        br1200 = 1200,
        br2400 = 2400,
        br4800 = 4800,
        br9600 = 9600,
        br14400 = 14400,
        br19200 = 19200,
        br38400 = 38400,
        br57600 = 57600,
        br115200 = 115200
    };

    enum class CharacterSize {
        cs5 = 5,
        cs6 = 6,
        cs7 = 7,
        cs8 = 8,
        cs9 = 9
    };

    enum class Parity {
        none = asio::serial_port_base::parity::none,
        odd = asio::serial_port_base::parity::odd,
        even = asio::serial_port_base::parity::even
    };

    enum class StopBits {
        one = asio::serial_port_base::stop_bits::one,
        onepointfive = asio::serial_port_base::stop_bits::onepointfive,
        two = asio::serial_port_base::stop_bits::two
    };

    enum class FlowControl {
        none = asio::serial_port_base::flow_control::none,
        software = asio::serial_port_base::flow_control::software,
        hardware = asio::serial_port_base::flow_control::hardware
    };


    class SerialParams {
    public:
        SerialParams(
            BaudRate baud_rate,
            CharacterSize character_size,
            Parity parity,
            StopBits stop_bits,
            FlowControl flow_control);

        [[nodiscard]] asio::serial_port_base::baud_rate baud_rate() const;
        [[nodiscard]]
        asio::serial_port_base::character_size character_size() const;
        [[nodiscard]] asio::serial_port_base::parity parity() const;
        [[nodiscard]] asio::serial_port_base::stop_bits stop_bits() const;
        [[nodiscard]]
        asio::serial_port_base::flow_control flow_control() const;

    private:
        BaudRate baud_rate_;
        CharacterSize character_size_;
        Parity parity_;
        StopBits stop_bits_;
        FlowControl flow_control_;
    };

    class SerialParams8N1 : public SerialParams {
    public:
        explicit SerialParams8N1(
            BaudRate baud_rate,
            CharacterSize character_size = CharacterSize::cs8,
            Parity parity = Parity::none,
            StopBits stop_bits = StopBits::one,
            FlowControl flow_control = FlowControl::none)
            : SerialParams{
                baud_rate, character_size, parity, stop_bits, flow_control}
        {}
    };

}
