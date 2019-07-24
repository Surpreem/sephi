#include "sephi/ipc/sync/event.h"

#include "sephi/ipc/sync/scoped_lock.h"


using sephi::ipc::Mutex;
using sephi::ipc::ScopedLock;


void sephi::ipc::Event::set()
{
    ScopedLock<Mutex> lock{mutex_};
    flag_ = true;
    condition_.notify_one();
}

void sephi::ipc::Event::wait()
{
    ScopedLock<Mutex> lock{mutex_};
    condition_.wait(lock, [this] {return flag_; });
    flag_ = false;
}
