#
#
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
configure_file(net/serial/config.yaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)

if(ENABLE_CATCH)
    set(ipc
        ipc/shm/shared_memory_test.cpp)

    set(net
        net/message/message_test.cpp
        net/serial/serial_test.cpp
        net/tcp/tcp_test.cpp
        net/udp/udp_test.cpp)

    add_executable(sephi_test 
        main.cpp
        ${ipc}
        ${net})

    target_link_libraries(sephi_test
        PRIVATE Catch2::Catch2
        PRIVATE yaml-cpp
        PRIVATE sephi)

    add_test(NAME sephi_test COMMAND sephi_test)
endif()
