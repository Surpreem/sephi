#pragma once


#include "boost/interprocess/sync/named_mutex.hpp"


namespace sephi::ipc {
    
    class NamedMutex : public boost::interprocess::named_mutex {
    public:
        using named_mutex::named_mutex;
    };

}
