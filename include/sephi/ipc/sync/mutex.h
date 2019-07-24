#pragma once


#include "boost/interprocess/sync/interprocess_mutex.hpp"


namespace sephi::ipc {
    
    class Mutex : public boost::interprocess::interprocess_mutex {
    public:
        using interprocess_mutex::interprocess_mutex;
    };

}
