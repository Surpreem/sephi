#pragma once


#include "boost/interprocess/containers/string.hpp"

#include "sephi/ipc/shm/shared_memory.h"


namespace sephi::ipc {

    using BasicString = boost_ipc::basic_string<
        char, std::char_traits<char>, SharedMemory::Allocator<char>>;
    
    class String : public BasicString {
    public:
        using BasicString::BasicString;
    };

}
