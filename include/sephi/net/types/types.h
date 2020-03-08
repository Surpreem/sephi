#pragma once


#include <array>
#include <cstdint>
#include <functional>

#include "sephi/net/types/remote.h"


namespace sephi::net {

    constexpr size_t max_buffer_size{4096};
    using Chunk = std::vector<uint8_t>;
    using PacketBuffer = std::array<uint8_t, max_buffer_size>;

    enum class ConnectionState {
        disconnected, connected
    };

    namespace serial {

        using ConnectionHandler = std::function<
            void(ConnectionState state, std::error_code const& ec)>;
        using DataHandler =
            std::function<void(uint8_t const* data, size_t size)>;

    }

    namespace tcp {

        using SessionId = uint64_t;

        using ServerConnectionHandler = std::function<
            void(SessionId id,
                ConnectionState state,
                std::error_code const& ec)>;
        using ServerPacketHandler = std::function<
            void(SessionId id, uint8_t const* packet, size_t size)>;

        using ClientConnectionHandler = std::function<
                void(ConnectionState state, std::error_code const& ec)>;
        using ClientPacketHandler = 
            std::function<void(uint8_t const* packet, size_t size)>;

        constexpr uint64_t session_none{0};

    }

    namespace udp {

        using CommunicationErrorHandler = std::function<
            void(Remote const& remote, std::error_code const& ec)>;
        using MessageHandler =
            std::function<void(Remote const& remote, Chunk const& chunk)>;

    }

}
