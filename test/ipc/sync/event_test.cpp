#include <functional>
#include <thread>

#include "catch2/catch.hpp"

#include "sephi/ipc/shm/shared_memory.h"
#include "sephi/ipc/sync/event.h"
#include "sephi/ipc/sync/named_event.h"
#include "sephi/util/random_string.h"


using namespace std::chrono_literals;

using std::this_thread::sleep_for;
using std::thread;
using std::ref;

using sephi::ipc::create_only;
using sephi::ipc::Event;
using sephi::ipc::NamedEvent;
using sephi::ipc::open_only;
using sephi::ipc::SharedMemory;
using sephi::util::random_string;


namespace {

    constexpr auto event_name{"named_event"};
    constexpr auto limit{10};
    constexpr auto shm_size{1024};

}


void func_named_event(int& val)
{
    NamedEvent event{event_name};
    event.wait();

    for (auto i{0}; limit != i; ++i)
        ++val;
}

void func_anony_event(Event& event, int& val)
{
    event.wait();

    for (auto i{0}; limit != i; ++i)
        ++val;
}


SCENARIO("Synchronise via a named event", "[sync]")
{
    GIVEN("a sharing variable")
    {
        auto result{0};

        NamedEvent event{event_name};

        WHEN("a thread start")
        {
            thread t{func_named_event, ref(result)};
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

SCENARIO("Synchronise via a anonymous event", "[sync]")
{
    auto const shm_name{random_string()};
    auto m1{SharedMemory{create_only, shm_name, shm_size}};
    auto m2{SharedMemory{open_only, shm_name}};

    GIVEN("a sharing variable")
    {
        auto result{0};

        auto const obj_name{random_string()};
        auto& m1_event{m1.construct<Event>(obj_name)};
        auto& m2_event{m2.find<Event>(obj_name)};

        WHEN("a thread start")
        {
            thread t{func_anony_event, ref(m2_event), ref(result)};
            sleep_for(1s);

            m1_event.set();

            THEN("the sum equals the limit")
            {
                t.join();

                REQUIRE(limit == result);

                m1.destruct(m1_event);
            }
        }
    }
}
