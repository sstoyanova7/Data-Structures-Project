#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <iostream>
using namespace std;

#include "Edge.h"
class Edge;

class Vertex //връх от графа (кръстовище от картата)
{
    public:
        Vertex ();
        Vertex (const string&);
        void changeStatusOfCrossroad(); //отваряне/затваряне на кръстовище
        bool isClosed() const;  //кръстовището е в ремонт ли е
        string getName() const;
        vector<Edge> getInStreets() const;
        vector<Edge> getOutStreets() const;

        void addOutStreet (Vertex&, unsigned);

    protected:
    private:
        string name; //име на кръстовището
        vector<Edge> inStreets; //улици, които приключват в кръстовището
        vector<Edge> outStreets; //улици, които започват от кръстовишето
        bool closed; //дали е в ремонт кръстовището

        void addInStreet (Vertex&, unsigned);


};

#endif // VERTEX_H
