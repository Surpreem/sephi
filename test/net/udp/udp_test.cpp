//#pragma warning(push, 3)
// C4459 declaration of 'str' hides global declaration
#include "catch2/catch.hpp"
//#pragma warning(pop)

#include "test/net/udp/udp.h"


using std::string;
using std::this_thread::sleep_for;

using sephi::net::Message;
using sephi::net::Remote;


namespace {

    constexpr auto port1{65001};
    constexpr auto port2{65002};
    constexpr auto limit{10'000};
    string const local_host{"127.0.0.1"};
    string const str{"Async tcp/ip communication test message."};
    Message const message{cbegin(str), cend(str)};

}


SCENARIO("Two end points can communicate via UDP", "[udp]")
{
    GIVEN("two end points using the same ip")
    {
        UdpServer server{port2};
        UdpClient client{port1};
        Remote remote{local_host, port2};

        WHEN("two end points communicate with each other")
        {
            auto transferred_bytes{0U};
            for (auto i{0}; limit != i; ++i) {
                client.write_to(remote, message);
                transferred_bytes += static_cast<uint32_t>(str.size());
                // required to receive all data correctly
                sleep_for(1ms);
            }

            // required to receive all data correctly
            sleep_for(1s);

            THEN("the transferred bytes and the received one are the same")
            {
                REQUIRE(transferred_bytes == client.received_bytes());
                REQUIRE(str.length() * limit == transferred_bytes);
            }
        }

        WHEN("broadcasting to the other")
        {
            auto transferred_bytes{0U};
            for (auto i{0}; limit != i; ++i) {
                client.broadcast(port2, message);
                transferred_bytes += static_cast<uint32_t>(str.size());
                // required to receive all data correctly
                sleep_for(1ms);
            }

            // required to receive all data correctly
            sleep_for(1s);

            THEN("the transferred bytes and the received one are the same")
            {
                REQUIRE(transferred_bytes == client.received_bytes());
                REQUIRE(str.length() * limit == transferred_bytes);
            }
        }
    }
}
