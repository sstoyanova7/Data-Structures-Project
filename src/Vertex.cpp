#include "Vertex.h"


Vertex::Vertex()
{
 closed = 0;
}

Vertex::Vertex(const string& name):
    name(name), closed(0){}

//������� ������� �� ������������
//��� ������������ � ���� ��������� -> �������� ��
//��� � ���� �������� -> ��������� ��
void Vertex::changeStatusOfCrossroad()
{
    closed = !closed;
}

bool Vertex::isClosed() const
{
    return closed;
}

string Vertex::getName() const
{
    return name;
}

vector<Edge> Vertex::getInStreets() const
{
    return inStreets;
}

vector<Edge> Vertex::getOutStreets() const
{
    return outStreets;
}

//������ �����, ����� ��������� � ������������
void Vertex::addInStreet(Vertex& nameOfCrossroad, unsigned length)
{
    inStreets.push_back(Edge(&nameOfCrossroad, this, length));
}

//������ �����, ����� ������� �� ������������
void Vertex::addOutStreet(Vertex& nameOfCrossroad, unsigned length)
{
    outStreets.push_back(Edge(this, &nameOfCrossroad, length));
    nameOfCrossroad.addInStreet(*this, length);
}



