#include "catch2/catch.hpp"

#include "sephi/ipc/mq/message_queue.h"
#include "sephi/util/random_string.h"


using sephi::ipc::create_only;
using sephi::ipc::MessageQueue;
using sephi::ipc::open_only;
using sephi::util::random_string;


namespace {

    constexpr auto max_msg_count{1024};
    constexpr auto limit{100};

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
