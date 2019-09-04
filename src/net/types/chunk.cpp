#include "sephi/net/types/chunk.h"


sephi::net::Chunk sephi::net::operator+(
    Chunk const& lhs, Chunk const& rhs)
{
    Chunk sum{lhs};
    sum += rhs;
    return sum;
}
