#pragma once


#include "boost/interprocess/sync/named_condition.hpp"


namespace sephi::ipc {

    class NamedCondition : public boost::interprocess::named_condition {
    public:
        using named_condition::named_condition;
    };

}
