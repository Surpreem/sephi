#include "sephi/net/types/message.h"


sephi::net::Message sephi::net::operator+(
    Message const& lhs, Message const& rhs)
{
    Message sum{lhs};
    sum += rhs;
    return sum;
}
