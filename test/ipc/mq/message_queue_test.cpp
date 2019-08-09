#include "catch2/catch.hpp"

#include "sephi/ipc/mq/message_queue.h"
#include "sephi/util/random_string.h"


using sephi::ipc::create_only;
using sephi::ipc::MessageQueue;
using sephi::ipc::open_only;
using sephi::ipc::open_or_create;
using sephi::util::random_string;


namespace {

    constexpr auto max_msg_count{1024};
    constexpr auto limit{100};

}


SCENARIO("Message queue can be created successfully", "[mq]")
{
    GIVEN("not exist any message queue")
    {
        WHEN("create message queue")
        {
            THEN("create message queue successfully")
            {
                auto const name{random_string()};

                REQUIRE_NOTHROW(
                    MessageQueue{
                        create_only, name, max_msg_count, sizeof(int)},
                    MessageQueue{open_only, name});
            }
        }

        WHEN("create message queue in any order")
        {
            THEN("create message queue successfully")
            {
                auto const name{random_string()};

                REQUIRE_NOTHROW(
                    MessageQueue{
                        open_or_create, name, max_msg_count, sizeof(int)},
                    MessageQueue{
                        open_or_create, name, max_msg_count, sizeof(int)});
                REQUIRE_THROWS(
                    MessageQueue{
                        open_or_create, name, max_msg_count, sizeof(int)},
                    MessageQueue{
                        create_only, name, max_msg_count, sizeof(int)});
            }
        }
    }
}

SCENARIO("They can communicate via message queue", "[mq]")
{
    auto const mq_name{random_string()};
    auto mq1{
        MessageQueue{create_only, mq_name, max_msg_count, sizeof(int)}};
    auto mq2{MessageQueue{open_only, mq_name}};

    GIVEN("100 numbers")
    {
        for (int i{0}; limit != i; ++i)
            mq1.send(&i, sizeof(i), 0);

        WHEN("receive 100 numbers")
        {
            int number[limit];
            unsigned priority;
            MessageQueue::size_type rcvd_size;

            for (int i{0}; limit != i; ++i)
                mq2.receive(&number[i], sizeof(number), rcvd_size, priority);

            THEN("the value equals to int index")
            {
                for (int i{0}; limit != i; ++i)
                    REQUIRE(i == number[i]);
            }
        }
    }
}
