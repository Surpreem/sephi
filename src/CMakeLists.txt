set(net
    net/serial/serial.cpp
    net/serial/serial_params.cpp
    net/tcp/client/client.cpp
    net/tcp/server/server.cpp
    net/tcp/server/session.cpp
    net/udp/udp.cpp)

add_library(sephi
    ${net})
