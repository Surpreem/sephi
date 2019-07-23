set(ipc
    ipc/shm/shared_memory.cpp)

set(net
    net/serial/serial.cpp
    net/serial/serial_params.cpp
    net/tcp/client/client.cpp
    net/tcp/server/server.cpp
    net/tcp/server/session.cpp
    net/udp/udp.cpp)

set(util
    util/random_string.cpp)

add_library(sephi
    ${ipc}
    ${net}
    ${util})
