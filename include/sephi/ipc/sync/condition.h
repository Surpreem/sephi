#pragma once


#include "boost/interprocess/sync/interprocess_condition.hpp"


namespace sephi::ipc {

    class Condition : public boost::interprocess::interprocess_condition {
    public:
        using interprocess_condition::interprocess_condition;
    };

}
