#pragma once


#if _WIN32
#   include "boost/interprocess/managed_windows_shared_memory.hpp"
#else
#   include "boost/interprocess/managed_shared_memory.hpp"
#endif

#include "sephi/ipc/creation_tags.h"
#include "sephi/ipc/exception.h"


namespace {
    
    auto not_found{"Unable to find the object."};

}


namespace boost_ipc = boost::interprocess;


namespace sephi::ipc {

    class SharedMemory {
#if _WIN32
        using ManagedSharedMemory = boost_ipc::managed_windows_shared_memory;
#else
        using ManagedSharedMemory = boost_ipc::managed_shared_memory;
#endif

    public:
        using Handle = ManagedSharedMemory::handle_t;

        template <typename T>
        class Allocator : public ManagedSharedMemory::allocator<T>::type {
            using Base = typename ManagedSharedMemory::allocator<T>::type;

        public:
            template <typename U>
            struct rebind {
                using other = Allocator<U>;
            };

            using Base::Base;
            using Base::construct;

            void construct(
                typename Base::pointer const& ptr,
                boost::container::default_init_t const&)
            {
                ::new (
                    static_cast<void*>(boost::interprocess::ipcdetail::to_raw_pointer(ptr)),
                    boost_container_new_t())
                    typename Base::value_type;
            }
        };

        template <typename T>
        using Deleter = typename ManagedSharedMemory::deleter<T>::type;

        SharedMemory(create_only_t, std::string const& name, size_t size);
        SharedMemory(open_only_t, std::string const& name);
        SharedMemory(open_or_create_t, std::string const& name, size_t size);

        SharedMemory(SharedMemory const&) = delete;
        SharedMemory(SharedMemory&&) noexcept = default;

        ~SharedMemory() noexcept;

        SharedMemory& operator=(SharedMemory const&) = delete;
        SharedMemory& operator=(SharedMemory&&) noexcept = default;

        template <typename T, typename... Args>
        T& construct(std::string const& name, Args&&... args)
        {
            return *memory_->construct<std::decay_t<T>>(name.c_str())(
                std::forward<Args>(args)...);
        }

        template <typename T>
        T& find(std::string const& name)
        {
            auto obj{memory_->find<T>(name.c_str()).first};
            if (obj)
                return *obj;

            throw NotFound{not_found};
        }

        template <typename T>
        void destruct(T const& obj)
        {
            assert(memory_->belongs_to_segment(&obj));

            memory_->destroy_ptr(&obj);
        }

        template <typename T>
        Allocator<T> allocator()
        {
            return memory_->get_allocator<T>();
        }

        template <typename T>
        Deleter<T> deleter()
        {
            return memory_->get_deleter<T>();
        }

        template <typename T>
        Handle to_handle(T const& obj) const
        {
            assert(contains(&obj));
            return memory_->get_handle_from_address(&obj);
        }

        template <typename T>
        T& from_handle(Handle const handle) const
        {
            auto ptr{memory_->get_address_from_handle(handle)};
            assert(contains(ptr));
            return *static_cast<T*>(ptr);
        }

        template <typename Func>
        void invoke_atomic(Func&& func)
        {
            memory_->atomic_func(func);
        }

        bool contains(void const* ptr) const;

        [[nodiscard]] size_t free_size() const;
        [[nodiscard]] std::string name() const;

    private:
        void destroy();

        std::unique_ptr<ManagedSharedMemory> memory_;
        std::string name_;
    };

}
