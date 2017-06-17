#ifndef EDGE_H
#define EDGE_H

#include "Vertex.h"
class Vertex;

class Edge //ребро на графа (улица от картата)
{
    public:
        Edge(Vertex*, Vertex*, unsigned);
        Vertex& getStart () const;
        Vertex& getEnd () const;
        unsigned getLength () const;
    protected:
    private:
        Vertex* start; //начално къстовище
        Vertex* end;   //крайно кръстовище
        unsigned length; //дължина на улицата
};

#endif // EDGE_H
