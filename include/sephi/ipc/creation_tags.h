#pragma once


#include "boost/interprocess/creation_tags.hpp"


namespace sephi::ipc {

    using create_only_t = boost::interprocess::create_only_t;
    using open_only_t = boost::interprocess::open_only_t;
    using open_or_create_t = boost::interprocess::open_or_create_t;

    using boost::interprocess::create_only;
    using boost::interprocess::open_only;
    using boost::interprocess::open_or_create;

}
