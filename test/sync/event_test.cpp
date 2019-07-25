#include <functional>
#include <thread>

#include "catch2/catch.hpp"

#include "sephi/sync/event.h"


using namespace std::chrono_literals;

using std::this_thread::sleep_for;
using std::thread;
using std::ref;

using sephi::sync::Event;


namespace {

    constexpr auto limit{10};

}


void func(Event& event, int& val)
{
    event.wait();

    for (auto i{0}; limit != i; ++i)
        ++val;
}


SCENARIO("Synchronise via a event", "[sync]")
{
    GIVEN("a sharing variable")
    {
        auto result{0};

        Event event;

        WHEN("a thread start")
        {
            thread t{func, ref(event), ref(result)};
            sleep_for(1s);

            event.set();

            THEN("the sum equals the limit")
            {
                t.join();

                REQUIRE(limit == result);
            }
        }
    }
}
