#pragma once


#include <cstdint>
#include <vector>


namespace sephi::net {

    class Message {
    public:
        Message() = default;

        Message(uint8_t const* data, size_t size)
            : data_{data, data + size}
        {}

        template <typename InputIter>
        Message(InputIter first, InputIter last)
            : data_{first, last}
        {}

        Message(Message const&) = default;
        Message(Message&&) noexcept = default;

        ~Message() noexcept = default;

        Message& operator=(Message const&) = default;
        Message& operator=(Message&&) noexcept = default;

        Message& operator+=(Message const& rhs)
        {
            data_.insert(data_.cend(), rhs.data_.cbegin(), rhs.data_.cend());
            return *this;
        }

        [[nodiscard]] uint8_t const* data() const
        {
            return &data_[0];
        }

        [[nodiscard]] uint8_t* data()
        {
            return const_cast<uint8_t*>(
                const_cast<Message const*>(this)->data());
        }

        [[nodiscard]] size_t size() const
        {
            return data_.size();
        }

    private:
        std::vector<uint8_t> data_;
    };


    Message operator+(Message const& lhs, Message const& rhs);

}
