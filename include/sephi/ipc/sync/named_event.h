#pragma once


#include <string>

#include "sephi/ipc/sync/named_condition.h"
#include "sephi/ipc/sync/named_mutex.h"


namespace sephi::ipc {

    class NamedEvent {
    public:
        NamedEvent(std::string const& name);
        ~NamedEvent();

        void set();
        void wait();

    private:
        std::string cv_name_;
        std::string mtx_name_;

        NamedCondition condition_;
        NamedMutex mutex_;
    };

}
