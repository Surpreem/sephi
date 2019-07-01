#include "catch2/catch.hpp"

#include "sephi/net/types/message.h"


using namespace sephi::net;


namespace {

    constexpr auto raw_data_size{10};
    constexpr auto size_zero{0};
    constexpr uint8_t raw_data[]{0, 9, 8, 7, 6, 5, 4, 3, 2, 1};

}


SCENARIO(
    "Message can be created with various constructors and operators",
    "[message]")
{
    GIVEN("a raw C array with 10 elements")
    {
        REQUIRE(raw_data_size == sizeof(raw_data));

        WHEN("the default constructor is given")
        {
            Message message;

            THEN("the size is zero")
            {
                REQUIRE(size_zero == message.size());
            }
        }

        WHEN("it is given the constructor to receive data and the data's size")
        {
            Message message{raw_data, sizeof(raw_data)};

            THEN("the size is 10")
            {
                REQUIRE(raw_data_size == message.size());
            }
        }

        WHEN("the copy constructor is given")
        {
            Message message1{raw_data, sizeof(raw_data)};
            Message message2{message1};
        
            THEN("the size is 10")
            {
                REQUIRE(raw_data_size == message2.size());
            }
        }
        
        WHEN("the copy assignment operator is given")
        {
            Message message1{raw_data, sizeof(raw_data)};
            Message message2;
            message2 = message1;
        
            THEN("the size is 10")
            {
                REQUIRE(raw_data_size == message2.size());
            }
        }

        WHEN("the move constructor is given")
        {
            Message message1{raw_data, sizeof(raw_data)};
            Message message2{std::move(message1)};

            THEN("the original's size is 0 and the new one's is 10")
            {
                REQUIRE(size_zero == message1.size());
                REQUIRE(raw_data_size == message2.size());
            }
        }

        WHEN("the move assignment operator is given")
        {
            Message message1{raw_data, sizeof(raw_data)};
            Message message2;
            message2 = std::move(message1);

            THEN("the original's size is 0 and the new one's is 10")
            {
                REQUIRE(size_zero == message1.size());
                REQUIRE(raw_data_size == message2.size());
            }
        }

        WHEN("the iterators of data are given")
        {
            std::vector<uint8_t> data{
                std::cbegin(raw_data), std::cend(raw_data)};
            REQUIRE(raw_data_size == data.size());

            Message message(cbegin(data), cend(data));

            THEN("the size is 10")
            {
                REQUIRE(raw_data_size == message.size());
            }
        }
    }
}

SCENARIO("A message can return the data", "[message]")
{
    GIVEN("a raw C array with 10 elements")
    {
        REQUIRE(raw_data_size == sizeof(raw_data));

        WHEN("a message is created with the raw array")
        {
            Message message{raw_data, raw_data_size};

            THEN("the message's data are the same with the raw array")
            {
                REQUIRE(raw_data_size == message.size());
                REQUIRE([&message] {
                        for (auto idx{0}; message.size() != idx; ++idx) {
                            if (message.data()[idx] != raw_data[idx])
                                return false;
                        }
                        return true;
                    }());
            }
        }
    }
}
