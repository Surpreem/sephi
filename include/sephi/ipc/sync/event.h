#pragma once


#include "sephi/ipc/sync/condition.h"
#include "sephi/ipc/sync/mutex.h"


namespace sephi::ipc {

    class Event {
    public:
        Event() = default;
        ~Event() = default;

        void set();
        void wait();

    private:
        bool flag_{false};

        Condition condition_;
        Mutex mutex_;
    };

}
