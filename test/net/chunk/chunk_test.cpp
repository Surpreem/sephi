#include "catch2/catch.hpp"

#include "sephi/net/types/Chunk.h"


using namespace sephi::net;


namespace {

    constexpr auto raw_data_size{10};
    constexpr auto size_zero{0};
    constexpr uint8_t raw_data[]{0, 9, 8, 7, 6, 5, 4, 3, 2, 1};

}


SCENARIO(
    "Chunk can be created with various constructors and operators",
    "[chunk]")
{
    GIVEN("a raw C array with 10 elements")
    {
        REQUIRE(raw_data_size == sizeof(raw_data));

        WHEN("the default constructor is given")
        {
            Chunk chunk;

            THEN("the size is zero")
            {
                REQUIRE(size_zero == chunk.size());
            }
        }

        WHEN("it is given the constructor to receive data and the data's size")
        {
            Chunk chunk{raw_data, sizeof(raw_data)};

            THEN("the size is 10")
            {
                REQUIRE(raw_data_size == chunk.size());
            }
        }

        WHEN("the copy constructor is given")
        {
            Chunk chunk1{raw_data, sizeof(raw_data)};
            Chunk chunk2{chunk1};
        
            THEN("the size is 10")
            {
                REQUIRE(raw_data_size == chunk2.size());
            }
        }
        
        WHEN("the copy assignment operator is given")
        {
            Chunk chunk1{raw_data, sizeof(raw_data)};
            Chunk chunk2;
            chunk2 = chunk1;
        
            THEN("the size is 10")
            {
                REQUIRE(raw_data_size == chunk2.size());
            }
        }

        WHEN("the move constructor is given")
        {
            Chunk chunk1{raw_data, sizeof(raw_data)};
            Chunk chunk2{std::move(chunk1)};

            THEN("the original's size is 0 and the new one's is 10")
            {
                REQUIRE(size_zero == chunk1.size());
                REQUIRE(raw_data_size == chunk2.size());
            }
        }

        WHEN("the move assignment operator is given")
        {
            Chunk chunk1{raw_data, sizeof(raw_data)};
            Chunk chunk2;
            chunk2 = std::move(chunk1);

            THEN("the original's size is 0 and the new one's is 10")
            {
                REQUIRE(size_zero == chunk1.size());
                REQUIRE(raw_data_size == chunk2.size());
            }
        }

        WHEN("the iterators of data are given")
        {
            std::vector<uint8_t> data{
                std::cbegin(raw_data), std::cend(raw_data)};
            REQUIRE(raw_data_size == data.size());

            Chunk chunk(cbegin(data), cend(data));

            THEN("the size is 10")
            {
                REQUIRE(raw_data_size == chunk.size());
            }
        }
    }
}

SCENARIO("Chunk can return the data", "[chunk]")
{
    GIVEN("a raw C array with 10 elements")
    {
        REQUIRE(raw_data_size == sizeof(raw_data));

        WHEN("Chunk is created with the raw array")
        {
            Chunk chunk{raw_data, raw_data_size};

            THEN("Chunk's data are the same with the raw array")
            {
                REQUIRE(raw_data_size == chunk.size());
                REQUIRE([&chunk] {
                        for (auto idx{0u}; chunk.size() != idx; ++idx) {
                            if (chunk.data()[idx] != raw_data[idx])
                                return false;
                        }
                        return true;
                    }());
            }
        }
    }
}

SCENARIO("Chunk can be concatenated", "[chunk]")
{
    GIVEN("two Chunks from a raw C array with 10 elements")
    {
        Chunk chunk1{raw_data, sizeof(raw_data)};
        Chunk chunk2{raw_data, sizeof(raw_data)};

        WHEN("two Chunks are concatenated")
        {
            auto chunk3{chunk1 + chunk2};

            THEN("the new Chunk's size is two times of one Chunk")
            {
                REQUIRE(raw_data_size * 2 == chunk3.size());
            }
        }

        WHEN("one Chunk is appended to the other")
        {
            chunk1 += chunk2;

            THEN("the result's size is two times of one Chunk")
            {
                REQUIRE(raw_data_size * 2 == chunk1.size());
            }
        }
    }
}

SCENARIO("Chunk can be inserted")
{
    GIVEN("Chunk from a raw C array with 10 elements")
    {
        Chunk chunk{raw_data, sizeof(raw_data)};

        WHEN("a integer is inserted")
        {
            auto val{3276};
            auto const first_ptr{reinterpret_cast<uint8_t*>(&val)};
            auto const bytes{sizeof(decltype(val))};
            auto const last_ptr{reinterpret_cast<uint8_t*>(&val) + bytes};
            chunk.insert(std::begin(chunk), first_ptr, last_ptr);

            THEN("restored data equals the origianl")
            {
                auto const restored{*reinterpret_cast<int32_t*>(chunk.data())};
                REQUIRE(val == restored);
                for (auto i{0}; raw_data_size != i; ++i)
                    REQUIRE(raw_data[i] == chunk[i + bytes]);
            }
        }
    }
}
