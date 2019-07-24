#pragma once


#include "boost/interprocess/creation_tags.hpp"


namespace sephi::ipc {

    using create_only_t = boost::interprocess::create_only_t;
    using open_only_t = boost::interprocess::open_only_t;
    using open_or_create_t = boost::interprocess::open_or_create_t;

    static create_only_t create_only;
    static open_only_t open_only;
    static open_or_create_t open_or_create;

}
