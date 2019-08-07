#include <functional>
#include <thread>

#include "catch2/catch.hpp"

#include "sephi/ipc/shm/shared_memory.h"
#include "sephi/ipc/sync/condition.h"
#include "sephi/ipc/sync/mutex.h"
#include "sephi/ipc/sync/named_condition.h"
#include "sephi/ipc/sync/named_mutex.h"
#include "sephi/ipc/sync/scoped_lock.h"
#include "sephi/util/random_string.h"


using namespace std::chrono_literals;

using std::this_thread::sleep_for;
using std::thread;
using std::ref;

using sephi::ipc::Condition;
using sephi::ipc::create_only;
using sephi::ipc::Mutex;
using sephi::ipc::NamedCondition;
using sephi::ipc::NamedMutex;
using sephi::ipc::ScopedLock;
using sephi::ipc::SharedMemory;
using sephi::ipc::open_only;
using sephi::ipc::open_or_create;
using sephi::util::random_string;


namespace {

    constexpr auto mutex_name{"named_mutex"};
    constexpr auto cv_name{"named_cv"};
    constexpr auto limit{10};
    constexpr auto shm_size{1024};

}


void func_named_condition(int& val)
{
    NamedMutex mutex{open_or_create, mutex_name};
    ScopedLock<NamedMutex> lock{mutex};

    NamedCondition thread_cv{open_or_create, cv_name};
    thread_cv.wait(lock);

    for (auto i{0}; limit != i; ++i)
        ++val;
}

void func_anony_condition(Mutex& mutex, Condition& cv, int& val)
{
    ScopedLock<Mutex> lock{mutex};
    cv.wait(lock);

    for (auto i{0}; limit != i; ++i)
        ++val;
}


SCENARIO("Synchronise via a named condition variable", "[sync]")
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
    } mtx_remover;

    struct ConditionRemove {
        ConditionRemove()
        {
            NamedCondition::remove(cv_name);
        }
        ~ConditionRemove()
        {
            NamedCondition::remove(cv_name);
        }
    } cv_remover;

    GIVEN("a sharing variable")
    {
        auto result{0};

        NamedCondition thread_cv{open_or_create, cv_name};

        WHEN("a thread start")
        {
            thread t{func_named_condition, ref(result)};
            sleep_for(1s);

            thread_cv.notify_one();

            THEN("the sum equals the limit")
            {
                t.join();

                REQUIRE(limit == result);
            }
        }
    }
}

SCENARIO("Synchronise via a anonymous condition variable", "[sync]")
{
    auto const shm_name{random_string()};
    auto m1{SharedMemory{create_only, shm_name, shm_size}};
    auto m2{SharedMemory{open_only, shm_name}};

    GIVEN("a sharing variable")
    {
        auto result{0};

        auto const obj_name_mtx{random_string()};
        auto const obj_name_cv{random_string()};
        auto& m1_mutex{m1.construct<Mutex>(obj_name_mtx)};
        auto& m1_cv{m1.construct<Condition>(obj_name_cv)};

        auto& m2_mutex{m2.find<Mutex>(obj_name_mtx)};
        auto& m2_cv{m2.find<Condition>(obj_name_cv)};

        WHEN("a thread start")
        {
            thread t{
                func_anony_condition,
                ref(m2_mutex),
                ref(m2_cv),
                ref(result)};
            sleep_for(1s);

            m1_cv.notify_one();

            THEN("the sum equals the limit")
            {
                t.join();

                REQUIRE(limit == result);

                m1.destruct(m1_mutex);
                m1.destruct(m1_cv);
            }
        }
    }
}
