#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <iostream>
using namespace std;

#include "Edge.h"
class Edge;

class Vertex //���� �� ����� (���������� �� �������)
{
    public:
        Vertex ();
        Vertex (const string&);
        void changeStatusOfCrossroad(); //��������/��������� �� ����������
        bool isClosed() const;  //������������ � � ������ �� �
        string getName() const;
        vector<Edge> getInStreets() const;
        vector<Edge> getOutStreets() const;

        void addOutStreet (Vertex&, unsigned);

    protected:
    private:
        string name; //��� �� ������������
        vector<Edge> inStreets; //�����, ����� ���������� � ������������
        vector<Edge> outStreets; //�����, ����� �������� �� ������������
        bool closed; //���� � � ������ ������������

        void addInStreet (Vertex&, unsigned);


};

#endif // VERTEX_H
