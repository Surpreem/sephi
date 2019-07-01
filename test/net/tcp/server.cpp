#include "test/net/tcp/server.h"

#include <thread>


using namespace std::chrono_literals;
using std::this_thread::sleep_for;


int main()
{
    ServerWrapper server{6705};

    while (true)
        sleep_for(1ms);
}
