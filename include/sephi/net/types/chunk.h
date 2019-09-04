#pragma once


#include <cstdint>
#include <vector>


namespace sephi::net {

    class Chunk {
    public:
        Chunk() = default;

        Chunk(uint8_t const* data, size_t size)
            : data_{data, std::next(data, size)}
        {}

        template <typename InputIter>
        Chunk(InputIter first, InputIter last)
            : data_{first, last}
        {}

        Chunk(Chunk const&) = default;
        Chunk(Chunk&&) noexcept = default;

        virtual ~Chunk() noexcept = default;

        Chunk& operator=(Chunk const&) = default;
        Chunk& operator=(Chunk&&) noexcept = default;

        Chunk& operator+=(Chunk const& rhs)
        {
            data_.insert(data_.cend(), rhs.data_.cbegin(), rhs.data_.cend());
            return *this;
        }

        uint8_t& operator[](size_t pos)
        {
            return data_[pos];
        }

        uint8_t const& operator[](size_t pos) const
        {
            return data_[pos];
        }

        uint8_t& at(size_t pos)
        {
            return data_.at(pos);
        }

        [[nodiscard]] uint8_t const& at(size_t pos) const
        {
            return data_.at(pos);
        }

        void clear() noexcept
        {
            data_.clear();
        }

        uint8_t* begin() noexcept
        {
            return &*data_.begin();
        }

        [[nodiscard]] uint8_t const* begin() const noexcept
        {
            return &*data_.cbegin();
        }

        [[nodiscard]] uint8_t const* cbegin() const noexcept
        {
            return &*data_.cbegin();
        }

        uint8_t* end() noexcept
        {
            return &*data_.end();
        }

        [[nodiscard]] uint8_t const* end() const noexcept
        {
            return &*data_.cend();
        }

        [[nodiscard]] uint8_t const* cend() const noexcept
        {
            return &*data_.cend();
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

        template <typename InputIter>
        uint8_t* insert(uint8_t const* pos, InputIter first, InputIter last)
        {
            return &*data_.insert(
                std::next(data_.cbegin(), pos - cbegin()), first, last);
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
