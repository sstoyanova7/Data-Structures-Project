#include "Vertex.h"


Vertex::Vertex()
{
 closed = 0;
}

Vertex::Vertex(const string& name):
    name(name), closed(0){}

//променя статуса на кръстовището
//ако кръстовището е било затворено -> отваряме го
//ако е било отворено -> затваряме го
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

//добавя улица, която приключва в кръстовището
void Vertex::addInStreet(Vertex& nameOfCrossroad, unsigned length)
{
    inStreets.push_back(Edge(&nameOfCrossroad, this, length));
}

//добавя улица, която започва от кръстовището
void Vertex::addOutStreet(Vertex& nameOfCrossroad, unsigned length)
{
    outStreets.push_back(Edge(this, &nameOfCrossroad, length));
    nameOfCrossroad.addInStreet(*this, length);
}



