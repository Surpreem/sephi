#pragma once


#include "boost/interprocess/sync/scoped_lock.hpp"


namespace sephi::ipc {

    template <typename Mutex>
    class ScopedLock : public boost::interprocess::scoped_lock<Mutex> {
    public:
        using boost::interprocess::scoped_lock<Mutex>::scoped_lock;
    };

}
