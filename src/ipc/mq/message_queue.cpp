#include "sephi/ipc/mq/message_queue.h"


using std::make_unique;

using boost_ipc::message_queue;
using boost::posix_time::microsec_clock;
using boost::posix_time::ptime;
using boost::posix_time::seconds;

using sephi::ipc::create_only;
using sephi::ipc::open_only;
using sephi::ipc::open_or_create;


sephi::ipc::MessageQueue::MessageQueue(
    create_only_t,
    std::string const& name,
    size_type max_msg_count,
    size_type max_msg_size)
    : name_{name}
{
    mq_ = make_unique<message_queue>(
        create_only, name_.c_str(), max_msg_count, max_msg_size);
}

sephi::ipc::MessageQueue::MessageQueue(open_only_t, std::string const& name)
    : name_{name}
{
    mq_ = make_unique<message_queue>(open_only, name_.c_str());
}

sephi::ipc::MessageQueue::MessageQueue(
    open_or_create_t,
    std::string const& name,
    size_type max_msg_count,
    size_type max_msg_size)
    : name_{name}
{
    mq_ = make_unique<message_queue>(
        open_or_create, name_.c_str(), max_msg_count, max_msg_size);
}

sephi::ipc::MessageQueue::~MessageQueue() noexcept
{
    destroy();
}

void sephi::ipc::MessageQueue::send(
    const void* buffer,
    size_type buffer_size,
    unsigned priority)
{
    mq_->send(buffer, buffer_size, priority);
}

bool sephi::ipc::MessageQueue::try_send(
    const void* buffer,
    size_type buffer_size,
    unsigned priority)
{
    return mq_->try_send(buffer, buffer_size, priority);
}

bool sephi::ipc::MessageQueue::timed_send(
    const void* buffer,
    size_type buffer_size,
    unsigned sec,
    unsigned priority)
{
    return mq_->timed_send(
        buffer, buffer_size, priority, posix_time_from(sec));
}

void sephi::ipc::MessageQueue::receive(
    void* buffer,
    size_type buffer_size,
    size_type& rcvd_size,
    unsigned& priority)
{
    mq_->receive(buffer, buffer_size, rcvd_size, priority);
}

bool sephi::ipc::MessageQueue::try_receive(
    void* buffer,
    size_type buffer_size,
    size_type& rcvd_size,
    unsigned& priority)
{
    return mq_->try_receive(buffer, buffer_size, rcvd_size, priority);
}

bool sephi::ipc::MessageQueue::timed_receive(
    void* buffer,
    size_type buffer_size,
    size_type& rcvd_size,
    unsigned sec,
    unsigned& priority)
{
    return mq_->timed_receive(
        buffer, buffer_size, rcvd_size, priority, posix_time_from(sec));
}

sephi::ipc::MessageQueue::size_type
sephi::ipc::MessageQueue::get_max_msg() const
{
    return mq_->get_max_msg();
}

sephi::ipc::MessageQueue::size_type
sephi::ipc::MessageQueue::get_max_msg_size() const
{
    return mq_->get_max_msg_size();
}

sephi::ipc::MessageQueue::size_type
sephi::ipc::MessageQueue::get_num_msg() const
{
    return mq_->get_num_msg();
}


void sephi::ipc::MessageQueue::destroy()
{
#if !_WIN32
    mq_->remove(name_.c_str());
#endif
}

ptime sephi::ipc::MessageQueue::posix_time_from(unsigned sec) const
{
    return microsec_clock::universal_time() + seconds(sec);
}
