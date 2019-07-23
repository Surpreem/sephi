#include "sephi/util/random_string.h"

#include "boost/uuid/random_generator.hpp"
#include "boost/uuid/uuid_io.hpp"


std::string sephi::util::random_string()
{
    // random_generator isn't thread safe
    thread_local boost::uuids::random_generator s_generator{};
    return to_string(s_generator());
}
