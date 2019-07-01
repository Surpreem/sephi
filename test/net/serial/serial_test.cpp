#pragma warning(push, 3)
// C4459 declaration of 'str' hides global declaration
#include "catch2/catch.hpp"
#pragma warning(pop)
#include "yaml-cpp/yaml.h"

#include "test/net/serial/serial.h"


using std::string;
using std::this_thread::sleep_for;

using YAML::LoadFile;

using sephi::net::Message;
using sephi::net::serial::BaudRate;
using sephi::net::serial::CharacterSize;
using sephi::net::serial::FlowControl;
using sephi::net::serial::SerialParams8N1;
using sephi::net::serial::StopBits;
using sephi::net::serial::Parity;


namespace {
    
    string const config{"config.yaml"};
    string const port1{"PORT1"};
    string const port2{"PORT2"};
    constexpr auto limit{1'000};
    string const str{"Async serial communication test message."};
    Message const message{cbegin(str), cend(str)};

}


SCENARIO("Two end points can communicate via serial port", "[serial]")
{
    GIVEN("two end points")
    {
        auto yaml_node{LoadFile(config)};
        auto ep1_name{yaml_node[port1].as<string>()};
        auto ep2_name{yaml_node[port2].as<string>()};

        REQUIRE("COM3" == ep1_name);
        REQUIRE("COM4" == ep2_name);

        SerialServer server{ep2_name, SerialParams8N1{BaudRate::br115200}};
        SerialClient client{ep1_name, SerialParams8N1{BaudRate::br115200}};

        WHEN("two end points communicate with each other")
        {
            while (!server.is_opened() || !client.is_opened())
                sleep_for(1s);

            auto transferred_bytes{0U};
            for (auto i{0}; limit != i; ++i) {
                auto result{client.write(message)};
                while (!result) {
                    while (!client.is_opened())
                        sleep_for(1s);
                    result = client.write(message);
                }
                transferred_bytes += static_cast<uint32_t>(str.size());
                // sleep_for(1ms);
            }

            while (!client.are_all_messages_sent())
                sleep_for(1s);

            THEN("the transferred bytes and the received one are the same")
            {
                REQUIRE(transferred_bytes == client.received_bytes());
                REQUIRE(str.length() * limit == transferred_bytes);
            }
        }
    }
}
