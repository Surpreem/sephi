#include "sephi/net/serial/serial_params.h"


using asio::serial_port_base;

using namespace sephi::net::serial;


SerialParams::SerialParams(
    BaudRate baud_rate,
    CharacterSize character_size,
    Parity parity,
    StopBits stop_bits,
    FlowControl flow_control)
    : baud_rate_{baud_rate},
      character_size_{character_size},
      parity_{parity},
      stop_bits_{stop_bits},
      flow_control_{flow_control}
{}

asio::serial_port_base::baud_rate SerialParams::baud_rate() const
{
    return asio::serial_port_base::baud_rate{static_cast<unsigned>(baud_rate_)};
}

asio::serial_port_base::character_size SerialParams::character_size() const
{
    return asio::serial_port_base::character_size{
        static_cast<unsigned>(character_size_)};
}

asio::serial_port_base::parity SerialParams::parity() const
{
    return asio::serial_port_base::parity{
        static_cast<asio::serial_port_base::parity::type>(parity_)};
}

asio::serial_port_base::stop_bits SerialParams::stop_bits() const
{
    return asio::serial_port_base::stop_bits{
        static_cast<asio::serial_port_base::stop_bits::type>(stop_bits_)};
}

asio::serial_port_base::flow_control SerialParams::flow_control() const
{
    return asio::serial_port_base::flow_control{
        static_cast<asio::serial_port_base::flow_control::type>(flow_control_)};
}
