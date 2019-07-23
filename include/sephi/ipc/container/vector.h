#pragma once


#include "boost/interprocess/containers/vector.hpp"

#include "sephi/ipc/shm/shared_memory.h"


namespace sephi::ipc {

    template <typename T>
    using Vector_t = boost_ipc::vector<T, SharedMemory::Allocator<T>>;

    template <typename T>
    class Vector : public Vector_t<T> {
    public:
        using Vector_t<T>::Vector_t;
    };

}
