#ifndef EDGE_H
#define EDGE_H

#include "Vertex.h"
class Vertex;

class Edge //����� �� ����� (����� �� �������)
{
    public:
        Edge(Vertex*, Vertex*, unsigned);
        Vertex& getStart () const;
        Vertex& getEnd () const;
        unsigned getLength () const;
    protected:
    private:
        Vertex* start; //������� ���������
        Vertex* end;   //������ ����������
        unsigned length; //������� �� �������
};

#endif // EDGE_H
