#include "catch2/catch.hpp"

#include "sephi/ipc/shm/shared_memory.h"
#include "sephi/ipc/shm/types.h"
#include "sephi/ipc/container/string.h"
#include "sephi/ipc/container/vector.h"
#include "sephi/util/random_string.h"


using std::make_pair;

using sephi::ipc::SharedMemory;
using sephi::ipc::shm_create_only;
using sephi::ipc::shm_open_only;
using sephi::ipc::String;
using sephi::ipc::Vector;
using sephi::util::random_string;


namespace {
    
    constexpr auto shm_size{1024};

}

SCENARIO("Shared memory can be created successfully", "[shm]")
{
    GIVEN("not exist any shared memory")
    {
        WHEN("create shared memory")
        {
            THEN("create shared memory successfully")
            {
                auto const name{random_string()};

                REQUIRE_NOTHROW(
                    SharedMemory{shm_create_only, name, shm_size},
                    SharedMemory{shm_open_only, name});
            }
            
        }
    }
}

SCENARIO("Objects can be created successfully", "[shm]")
{
    auto const shm_name{random_string()};
    auto m1{SharedMemory{shm_create_only, shm_name, shm_size}};
    auto m2{SharedMemory{shm_open_only, shm_name}};

    GIVEN("primitive type")
    {
        auto const x{147};

        WHEN("create the object in the shared memory and find it")
        {
            auto const obj_name{random_string()};
            auto& m1_obj{m1.construct<decltype(x)>(obj_name, x)};
            auto& m2_obj{m2.find<decltype(x)>(obj_name)};

            THEN("the values of the both objects are equal")
            {
                REQUIRE(x == m1_obj);
                REQUIRE(&m1_obj != &m2_obj);
                REQUIRE(m1_obj == m2_obj);

                m1.destruct(m1_obj);
            }
        }
    }

    GIVEN("values for std::pair type")
    {
        auto const x{149};
        auto const y{491};

        WHEN("create the object in the shared memory and find it")
        {
            auto const obj_name{random_string()};
            auto& m1_obj{m1.construct<std::pair<int, int>>(obj_name, x, y)};
            auto& m2_obj{m2.find<std::pair<int, int>>(obj_name)};

            THEN("the values of the both objects are equal")
            {
                REQUIRE(make_pair(x, y) == m1_obj);
                REQUIRE(&m1_obj != &m2_obj);
                REQUIRE(m1_obj == m2_obj);

                m1.destruct(m1_obj);
            }
        }
    }

    GIVEN("string for interprocess")
    {
        auto str{random_string()};

        WHEN("create the object in the shared memory and find it")
        {
            auto const obj_name{random_string()};
            auto& m1_obj{m1.construct<String>(obj_name, str.c_str(), m1.allocator<char>())};
            auto& m2_obj{m2.find<String>(obj_name)};

            THEN("the values of the both objects are equal")
            {
                REQUIRE(str.c_str() == m1_obj);
                REQUIRE(&m1_obj != &m2_obj);
                REQUIRE(m1_obj == m2_obj);

                m1.destruct(m1_obj);
            }
        }
    }

    GIVEN("vector for interprocess")
    {
        auto const size{11};

        WHEN("create the object in the shared memory and find it")
        {
            auto const obj_name{random_string()};
            auto& m1_obj{m1.construct<Vector<int>>(obj_name, m1.allocator<char>())};
            for (auto i{0}; size != i; ++i)
                m1_obj.push_back(size - i);

            auto& m2_obj{m2.find<Vector<int>>(obj_name)};

            THEN("the values of the both objects are equal")
            {
                REQUIRE(size == m1_obj.size());
                REQUIRE(&m1_obj != &m2_obj);
                REQUIRE(m1_obj == m2_obj);
                for (auto i{0}; size != i; ++i)
                    REQUIRE(m1_obj[i] == m2_obj[i]);

                m1.destruct(m1_obj);
            }
        }
    }

    GIVEN("default initialized vector for interprocess")
    {
        auto const size{11};

        WHEN("create the object in the shared memory and find it")
        {
            auto const obj_name{random_string()};
            auto& m1_obj{m1.construct<Vector<int>>(
                obj_name,
                size,
                boost::container::default_init, m1.allocator<char>())};
            auto& m2_obj{m2.find<Vector<int>>(obj_name)};

            THEN("the values of the both objects are equal")
            {
                REQUIRE(size == m1_obj.size());
                REQUIRE(&m1_obj != &m2_obj);
                REQUIRE(size == m2_obj.size());

                m1.destruct(m1_obj);
            }
        }
    }
}
