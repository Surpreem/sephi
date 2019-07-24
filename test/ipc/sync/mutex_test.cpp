#include <functional>
#include <thread>

#include "catch2/catch.hpp"

#include "sephi/ipc/shm/shared_memory.h"
#include "sephi/ipc/shm/types.h"
#include "sephi/ipc/sync/mutex.h"
#include "sephi/ipc/sync/named_mutex.h"
#include "sephi/ipc/sync/scoped_lock.h"
#include "sephi/ipc/sync/types.h"
#include "sephi/util/random_string.h"


using std::thread;
using std::ref;

using sephi::ipc::Mutex;
using sephi::ipc::NamedMutex;
using sephi::ipc::ScopedLock;
using sephi::ipc::SharedMemory;
using sephi::ipc::open_or_create;
using sephi::ipc::shm_create_only;
using sephi::ipc::shm_open_only;
using sephi::util::random_string;


namespace {

    constexpr auto mutex_name{"named_mutex"};
    constexpr auto limit{10};
    constexpr auto shm_size{1024};

}


void func_named_mutex(int& val)
{
    NamedMutex mutex{open_or_create, mutex_name};
    ScopedLock<NamedMutex> lock{mutex};

    for (auto i{0}; limit != i; ++i)
        ++val;
}

void func_anony_mutex(Mutex& mutex, int& val)
{
    ScopedLock<Mutex> lock{mutex};

    for (auto i{0}; limit != i; ++i)
        ++val;
}


SCENARIO("Two threads synchronise via a named mutex", "[sync]")
{
    struct MutexRemove {
        MutexRemove()
        {
            NamedMutex::remove(mutex_name);
        }
        ~MutexRemove()
        {
            NamedMutex::remove(mutex_name);
        }
    } remover;

    GIVEN("a sharing variable")
    {
        auto result{0};

        WHEN("two threads start")
        {
            thread t1{func_named_mutex, ref(result)};
            thread t2{func_named_mutex, ref(result)};

            THEN("the sum equals two times of limit")
            {
                t1.join();
                t2.join();

                REQUIRE(limit * 2 == result);
            }
        }
    }
}

SCENARIO("Two threads synchronise via a anonymous mutex", "[sync]")
{
    auto const shm_name{random_string()};
    auto m1{SharedMemory{shm_create_only, shm_name, shm_size}};
    auto m2{SharedMemory{shm_open_only, shm_name}};

    GIVEN("a sharing variable and a anonymous mutex")
    {
        auto result{0};

        auto const obj_name{random_string()};
        auto& m1_mutex{m1.construct<Mutex>(obj_name)};
        auto& m2_mutex{m2.find<Mutex>(obj_name)};

        WHEN("two threads start")
        {
            thread t1{func_anony_mutex, ref(m1_mutex), ref(result)};
            thread t2{func_anony_mutex, ref(m2_mutex), ref(result)};

            THEN("the sum equals two times of limit")
            {
                t1.join();
                t2.join();

                REQUIRE(limit * 2 == result);

                m1.destruct(m1_mutex);
            }
        }
    }
}
