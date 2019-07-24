#pragma once


#include "boost/interprocess/containers/vector.hpp"

#include "sephi/ipc/shm/shared_memory.h"


namespace sephi::ipc {

    template <typename T>
    using VectorBase = boost_ipc::vector<T, SharedMemory::Allocator<T>>;

    template <typename T>
    class Vector : public VectorBase<T> {
    public:
        using VectorBase<T>::VectorBase;
    };

}
