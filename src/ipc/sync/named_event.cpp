#include "sephi/ipc/sync/named_event.h"

#include "sephi/ipc/sync/scoped_lock.h"
#include "sephi/ipc/sync/types.h"


using std::string;

using sephi::ipc::NamedMutex;
using sephi::ipc::ScopedLock;


namespace {
    
    string const cv_prefix{"_cv_prefix_"};
    string const mtx_prefix{"_mtx_prefix_"};

}


sephi::ipc::NamedEvent::NamedEvent(string const& name)
    : cv_name_{cv_prefix + name},
      mtx_name_{mtx_prefix + name},
      condition_{open_or_create, cv_name_.c_str()},
      mutex_{open_or_create, mtx_name_.c_str()}
{}

sephi::ipc::NamedEvent::~NamedEvent()
{
    NamedCondition::remove(cv_name_.c_str());
    NamedMutex::remove(mtx_name_.c_str());
}


void sephi::ipc::NamedEvent::set()
{
    // If you use a scoped_lock like anonymous event, the deadlock occurs.
    // Why? I don't know.
    condition_.notify_one();
}

void sephi::ipc::NamedEvent::wait()
{
    // If you use a scoped_lock and a predicate like anonymous event,
    // the deadlock occurs.
    // Why? I don't know.
    ScopedLock<NamedMutex> lock{mutex_};
    condition_.wait(lock);
}
