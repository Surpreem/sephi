#include "test/net/tcp/client.h"

#include <iostream>


using namespace std::chrono_literals;
using std::cout;
using std::string;
using std::this_thread::sleep_for;

using namespace sephi::net;


constexpr auto limit{1'000'000};


int main()
{
    ClientWrapper client{"127.0.0.1", 6705};

    string str{"Async TCP/IP communication test message."};
    Chunk chunk{reinterpret_cast<uint8_t const*>(str.c_str()), str.size()};

    while (!client.is_connected()) {
        sleep_for(1s);
    }

    auto transferred_bytes{0U};
    for (auto i{0}; limit != i; ++i) {
        auto result{client.write(chunk)};
        if (!result) {
            while (!client.is_connected())
                sleep_for(1s);
            result = client.write(chunk);
        } else {
            transferred_bytes += static_cast<uint32_t>(str.size());
        }
    }
    cout << "Total transferred bytes: " << transferred_bytes << '\n';

    while (!client.are_all_messages_sent())
        sleep_for(1s);

    return 0;
}
