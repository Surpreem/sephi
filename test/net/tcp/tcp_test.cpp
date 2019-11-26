#include "catch2/catch.hpp"

#include "test/net/tcp/client.h"
#include "test/net/tcp/server.h"


using namespace std::chrono_literals;
using std::string;
using std::this_thread::sleep_for;

using sephi::net::Chunk;


namespace {
    
    constexpr auto port{65005};
    constexpr auto limit{10'000};
    string const local_host{"127.0.0.1"};
    string const str{"Async TCP/IP communication test message."};
    Chunk const chunk{cbegin(str), cend(str)};

}

SCENARIO("Server and client can communicate via TCP/IP", "[tcp]")
{
    GIVEN("server and client")
    {
        // To check sephi::net::tcp::Server::close()
        // destroy the server object first
        ClientWrapper client{{local_host, port}};
        ServerWrapper server{port};

        WHEN("client can communicate with server")
        {
            while (!client.is_connected())
                sleep_for(1s);

            auto transferred_bytes{0U};
            for (auto i{0}; limit != i; ++i) {
                auto result{client.write(chunk)};
                while (!result) {
                    while (!client.is_connected())
                        sleep_for(1s);
                    result = client.write(chunk);
                }
                transferred_bytes += static_cast<uint32_t>(str.size());
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
