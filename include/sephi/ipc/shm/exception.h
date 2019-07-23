#pragma once


#include <stdexcept>


namespace sephi::ipc {
    
    class Exception : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class NotFound : public Exception {
    public:
        using Exception::Exception;
    };

}
