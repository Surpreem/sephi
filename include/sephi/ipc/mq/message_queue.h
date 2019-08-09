#pragma once


#include "boost/date_time/posix_time/posix_time_types.hpp"

#include "sephi/ipc/creation_tags.h"
#include "sephi/ipc/mq/boost/message_queue_mod.hpp"


namespace boost_ipc = boost::interprocess;


namespace sephi::ipc {

    class MessageQueue {
    public:
        using size_type = boost_ipc::message_queue::size_type;
        unsigned static const priority_none{0};


        MessageQueue(
            create_only_t,
            std::string const& name,
            size_type max_msg_count,
            size_type max_msg_size);
        MessageQueue(open_only_t, std::string const& name);
        MessageQueue(
            open_or_create_t,
            std::string const& name,
            size_type max_msg_count,
            size_type max_msg_size);

        ~MessageQueue() noexcept;

        void send(
            void const* buffer,
            size_type buffer_size,
            unsigned priority = priority_none);
        bool try_send(
            void const* buffer,
            size_type buffer_size,
            unsigned priority = priority_none);
        bool timed_send(
            void const* buffer,
            size_type buffer_size,
            unsigned sec,
            unsigned priority = priority_none);

        void receive(
            void* buffer,
            size_type buffer_size,
            size_type& rcvd_size,
            unsigned& priority);
        bool try_receive(
            void* buffer,
            size_type buffer_size,
            size_type& rcvd_size,
            unsigned& priority);
        bool timed_receive(
            void* buffer,
            size_type buffer_size,
            size_type& rcvd_size,
            unsigned sec,
            unsigned& priority);

        [[nodiscard]] size_type get_max_msg() const;
        [[nodiscard]] size_type get_max_msg_size() const;
        [[nodiscard]] size_type get_num_msg() const;

    private:
        void destroy();
        boost::posix_time::ptime posix_time_from(unsigned sec) const;


        std::unique_ptr<boost_ipc::message_queue> mq_;
        std::string name_;
    };

}
