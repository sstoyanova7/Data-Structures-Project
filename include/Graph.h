#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>
#include <queue>
#include "Vertex.h"
#include "Edge.h"

using namespace std;
class Graph
{
    public:

        void addCrossroad (string&); //������ ���� (����������) � �����
        void addStreet (string&, string&, unsigned); //������ ����� (�����) � �����
        void loadFile (string&); //���������, ������ �� �����

        void print()
        {
            for(unordered_map<string, Vertex> :: iterator it = cityCrossroads.begin();
                            it != cityCrossroads.end(); it++)
            {
                cout << it -> first << ": ";
                vector<Edge> tmp = it -> second.getOutStreets();
                for(size_t i = 0; i < tmp.size(); i++)
                    cout << tmp[i].getEnd().getName() << "_" << tmp[i].getLength() << "  ";
                cout << endl;
            }
        }

        //1. �������� ���� ��� ��� ����� ��� �������� ����������,
        bool hasRoute (string&, string&);
        //2. �������� �� ���-������ ��� ����� ��� ����������
        //+3. ��� ��������� �� ��������� ����������
        //(����� �� ����� �������� ���� ������) �� ������ ������������
        //���-������ ��� ����� ����� ����������
        queue<string> shortestRoute (string&, string&);
        //4. �������� ���� ��� ������ ���������� �� ������� �����
        //� �������� �� �������� ���� �� ����� � �� �� ������
        //���, �� ������ ��� ��������
        bool hasCycle (string&);
        //5. �������� ���� ����� �� �������� ����� ������������ ��������
        //�� ������ ����� ��� �� �������� �� ���� � ���� ����� ��� ����
        queue<string> tour (string&); //
        //6. �������� ���� � �������� �� ������� �� ������ ����������
        // �� ������ ��������
        bool areAllAvailable (string&);
        //7. �������� �� ������ �������� �����
        vector<string> deadends () const;

        void closeCrossroad (string&);
        void openCrossroad (string&);
        vector<string> findClosed();
        vector<string> getOutStreets (string&);

    private:
        unordered_map <string, Vertex> cityCrossroads;
        bool hasCycleHelper (string&, string&, vector<string>&);
};

#endif // GRAPH_H
