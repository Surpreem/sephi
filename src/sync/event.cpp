#include "sephi/sync/event.h"


using std::lock_guard;
using std::mutex;
using std::unique_lock;


void sephi::sync::Event::set()
{
    lock_guard<mutex> guard{mutex_};
    flag_ = true;
    condition_.notify_one();
}

void sephi::sync::Event::wait()
{
    unique_lock<mutex> guard{mutex_};
    condition_.wait(guard, [this] {return flag_; });
    flag_ = false;
}
