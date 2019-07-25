#pragma once


#include <condition_variable>
#include <mutex>


namespace sephi::sync {

    class Event {
    public:
        Event() = default;
        ~Event() = default;

        void set();
        void wait();

    private:
        bool flag_{false};

        std::condition_variable condition_;
        std::mutex mutex_;
    };

}
