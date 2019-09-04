set(ipc
    ipc/mq/message_queue.cpp
    ipc/shm/shared_memory.cpp
    ipc/sync/event.cpp
    ipc/sync/named_event.cpp)

set(net
    net/serial/serial.cpp
    net/serial/serial_params.cpp
    net/tcp/client/client.cpp
    net/tcp/server/server.cpp
    net/tcp/server/session.cpp
    net/udp/udp.cpp)

set(sync
    sync/event.cpp)

set(util
    util/random_string.cpp)

# for debug library
set(CMAKE_DEBUG_POSTFIX d)

add_library(sephi
    ${ipc}
    ${net}
    ${sync}
    ${util})
