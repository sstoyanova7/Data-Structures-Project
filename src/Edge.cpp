#include "Edge.h"

Edge::Edge(Vertex* start, Vertex* end, unsigned length):
        start(start), end(end), length(length){}

Vertex& Edge::getStart () const
{
    return *start;
}
Vertex& Edge::getEnd () const
{
    return *end;
}

unsigned Edge::getLength () const
{
    return length;
}
