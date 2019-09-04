#pragma once


#include <cstdint>
#include <vector>


namespace sephi::net {

    class Chunk {
    public:
        Chunk() = default;

        Chunk(uint8_t const* data, size_t size)
            : data_{data, data + size}
        {}

        template <typename InputIter>
        Chunk(InputIter first, InputIter last)
            : data_{first, last}
        {}

        Chunk(Chunk const&) = default;
        Chunk(Chunk&&) noexcept = default;

        ~Chunk() noexcept = default;

        Chunk& operator=(Chunk const&) = default;
        Chunk& operator=(Chunk&&) noexcept = default;

        Chunk& operator+=(Chunk const& rhs)
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
                const_cast<Chunk const*>(this)->data());
        }

        [[nodiscard]] size_t size() const
        {
            return data_.size();
        }

    private:
        std::vector<uint8_t> data_;
    };


    Chunk operator+(Chunk const& lhs, Chunk const& rhs);

}
