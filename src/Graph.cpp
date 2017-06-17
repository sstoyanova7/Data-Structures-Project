#include <fstream>
#include <stack>
#include <stdexcept>
#include "Graph.h"

//������ ���������� ��� �����������
void Graph::addCrossroad(string& name)
{
    //��� ������������ �� ���� � ��� ���������
    //��������� ������ ����������(��� �����)
    if (cityCrossroads.count(name) == 0)
    {
        cityCrossroads[name] = Vertex(name);
    }
}

//������ ����� ��� ����������� (������� ��� ����������)
void Graph::addStreet(string& start, string& end, unsigned length)
{
    //��� �� ����������� ����� ���������� �� �������
    addCrossroad(start);
    addCrossroad(end);
    cityCrossroads[start].addOutStreet(cityCrossroads[end], length);
}

void Graph::loadFile(string& url)
{
    ifstream file (url); //�������� �����
    if (file.is_open())  //��� � ������� �����
    {
        string row;
        //��������� ����� ��� �� �������
        while (getline(file, row))
        {
            string startCrossroad, endCrossroad;
            unsigned length, counter, start = 0;
            counter = row.find(' ');
            //��������� ������� ���������� �� ����
            startCrossroad = row.substr(start, counter - start);
            //� �� �������� ��� �����
            addCrossroad(startCrossroad);
            //���������� �����
            start = counter;
            //������ ��� ���� �� ����
            //��������� ���������� -> ������� -> ���������� -> �������....
            while (start < row.size())
            {
                start ++;
                counter = row.find (' ', start);
                endCrossroad = row.substr (start, counter - start);
                start = counter+1;
                length = 0;
                //��������� ��������� ���� �����
                while (row[start] != ' ' && start < row.size())
                {
                    length *= 10;
                    length += row[start] - '0';
                    start++;
                }
                //�������� ������� ��� ����������� ������� ����������,
                //������ ���������� � ������� �� ������ �����.
                //�� �������� ������������ �� �������, ������
                //��� ��������� addStreet() ���� �� ���������
                //� ��� �� ���� �� ������� addCrossroad()
                addStreet(startCrossroad, endCrossroad, length);
            }
        }
    }
    else //��� �� � ������� �����
    {
        throw domain_error("File not found");
    }
}

//1. �������� ���� ��� ��� ����� ��� �������� ����������,
//���� ���������� BFS
bool Graph::hasRoute(string& start, string& end)
{
    vector<string> reachable; //������������, �� ����� ����� �� ������� �� start
    queue<string> unReached; //����������, �� ����� ������ �� ��������
                             //�� ���� ����� �� �������

    unReached.push(start);
    reachable.push_back(start);
    while(!unReached.empty())
    {

        vector<Edge> outStreets = cityCrossroads[unReached.front()].getOutStreets();
        unReached.pop();
        for(size_t i = 0; i < outStreets.size(); i++)
        {
            //��� ������������ � ��������� �� ����������
            if(outStreets[i].getEnd().isClosed())
                continue;

            bool flag = 1; //��� �� �� ������������ � reachable(0 -> ��, 1 -> ��)
            for(size_t j = 0; j < reachable.size()&&flag; j++)
            {
                //��� ������������ ����, �� ��� � reachable
                if(reachable[j] == outStreets[i].getEnd().getName())
                {
                    flag = 0;
                }
            }
            //��� ������������ �� ���� � reachable �� ��������
            // reachable � � unReached
            if(flag)
            {   //��� ��� �������� �� ������������, ����� ������
                //������� true
                if(outStreets[i].getEnd().getName() == end)
                    return 1;
                //��� �� ��� �������� ���, �������� ���������� ����������
                //�� ��������
                reachable.push_back(outStreets[i].getEnd().getName());
                unReached.push(outStreets[i].getEnd().getName());
            }
        }
    }
    return 0;
}

//2. �������� �� ���-������ ��� ����� ��� ����������
//+3. ��� ��������� �� ��������� ����������
//(����� �� ����� �������� ���� ������) �� ������ ������������
//���-������ ��� ����� ����� ����������,
//���� ���������� ��������� �� ��������
queue<string> Graph::shortestRoute (string& startName, string& endName)
{
    if(!hasRoute(startName, endName))
    {
        cout << "Route not found!!!!"; //ufvhyigujhikjgfdsafsghjkilgfvhjukyjytgrfedwdeyuhgtrfedfrgtujytgr
        return queue<string>();
    }
    struct helper
    {
        string name;
        unsigned distance;
        string parent;

        //� �������� �� �� ������
        //������ � ��-������ ��������� �� �
        //� ��-����� ��������� => ���������� ��
        //distance � ������� true ���
        //������ ����� � > �� ������� �����
        bool operator<(const helper& other) const
        {
            return !(this -> distance < other.distance);
        }

        helper ()
        {}
        helper(string name, unsigned distance, string parent)
        {
            this -> name = name;
            this -> distance = distance;
            this -> parent = parent;
        }
    };

    priority_queue<helper> uncheckedCrossroads;
    uncheckedCrossroads.push(helper(startName, 0, ""));
    vector<string> done; //��������� ���������� �������
    unordered_map<string, helper> shortestsRoutes; //������ ����������� ����������
                                                   //�� ����� ����

    while(!uncheckedCrossroads.empty())
    {
        helper crossroad = uncheckedCrossroads.top();
        uncheckedCrossroads.pop();
        bool flag = 1;
        for(size_t i = 0; i < done.size() && flag; i++)
        {
            if(crossroad.name == done[i])
            {
                flag = 0;
            }
        }
        if(!flag)
        {
            continue;
        }
        vector<Edge> outStreets = cityCrossroads[crossroad.name].getOutStreets();
        for(size_t i = 0; i < outStreets.size(); i++)
        {
            if(outStreets[i].getEnd().isClosed())
            {             //��� ������������ � ���������
                continue; //�� ����������
            }

            helper tmp(outStreets[i].getEnd().getName(), crossroad.distance + outStreets[i].getLength(), crossroad.name);
            uncheckedCrossroads.push(tmp);
        }

        done.push_back(crossroad.name);
        if (shortestsRoutes.count(crossroad.name) == 0)
        {
            shortestsRoutes[crossroad.name] = crossroad;
        }
        else if (shortestsRoutes[crossroad.name].distance > crossroad.distance)
        {
            shortestsRoutes[crossroad.name] = crossroad;
        }
     }

     helper cursor = shortestsRoutes[endName];
     stack<string> routeReversed;
     unsigned distance = 0;
     while (cursor.name != startName)
     {

         routeReversed.push(cursor.name);
         unsigned tmp = cursor.distance;
         cursor = shortestsRoutes[cursor.parent];
         distance += tmp - cursor.distance;
     }
    queue<string> route;
    routeReversed.push(startName);
    while (!routeReversed.empty())
    {
        route.push(routeReversed.top());
        routeReversed.pop();
    }
    return route;
}

//����������� ���� ����� �� ������� �� ���������
//���������� ���� ����� ���� ����� �����
//���������� ����� �������� DFS(��� �������
//���� ����� DFS-�� �����
bool Graph::hasCycle(string& start)
{
    vector<string> visited;
    return hasCycleHelper(start, start, visited);
}

//�������, ����������� �������
//�������� ������� ����������(�� ����� ������ �� �� ������),
//cursor � ������������ �� ����� ��� ��������
//� �����, � ���� ���������� (�� �� �� �������� ����� �� ������ �� ������
bool Graph::hasCycleHelper(string& start, string& cursor, vector<string>& visited)
{
    //��������� �������, �� ����� ����� �� ������� �� cursor
    vector<Edge> outStreets = cityCrossroads[cursor].getOutStreets();
    //� ��������� ����� �� ���
    for (int i = 0; i < outStreets.size(); i++)
    {
        //��������� ����� �� ������������, ����� ����� �� �������� �� cursor
        string tmp = outStreets[i].getEnd().getName();
        //���������� ���������, ��� ���� ���������� � ���������
        if (start == tmp)
            return 1;
        bool flag = 1; // 1-> �� ��� �������� ������������, -> 0 �������� ��� ��
        //����������� ���� ������������ � ��������
        for (int j = 0; j < visited.size() && flag; j++)
        {
            if (tmp == visited[j])
                flag = 0;
        }
        //� ��� �� � �� �������� ��� ���������� � ������� ����������
        //��� ��������� ���� �� cursor ������ ���� ����������
        if (flag)
        {
            visited.push_back(tmp);
            if (hasCycleHelper(start, tmp, visited))
                return 1;
        }
    }
    return 0;
}


//��������� ���� ���� �� �� �������
//������������ �������� �� ������ �����(���
//�� �� ������ ��� ���� �� ���� � ���� �����)
//(���� ��� ������� ���)
queue<string> Graph::tour(string& start)
{
    for (unordered_map<string, Vertex>::const_iterator it = cityCrossroads.begin(); it != cityCrossroads.end(); it++)
    {
        //���� �� �������� ����� != �� ���� �� ��������� �
        //���������� ������� �� �� ���������� ������� ���
        if (it -> second.getOutStreets().size() != it -> second.getInStreets().size())
        {
            cout << "Sorry! Tour is not available." << endl;
            return queue<string>();
        }
    }
}

//��������� ���� ��� ��� �� ����� �� ������������
//����������� ��������� �������, �����
//��������� �� ������������ �� �����
bool Graph::areAllAvailable(string& start)
{
    vector<string> visited;
    visited.push_back(start);
    //������ ����� ��� ����, ����� ��� �������, �� ������
    //=> ������������ ������� �� �� ����� 0,
    //�� ��� �� ������� ����������� � �����
    //(���������� ������ � �������� ����������)
    //� ��� ���� ������ � ������ ���������
    //�������� ����� ���� ���������� => �� ��� ������ �� ������ ����������
    string emptyString = "";
    hasCycleHelper(emptyString, start, visited);
    return visited.size() == cityCrossroads.size();
}


//����� ������ ����������, �� ����� ���� outStreets
vector<string> Graph::deadends() const
{
    vector<string> allDeadends;
    for (unordered_map<string, Vertex>::const_iterator it = cityCrossroads.begin(); it != cityCrossroads.end(); it++)
    {
        if (it -> second.getOutStreets().size() == 0)
            allDeadends.push_back(it -> second.getName());
    }
    return allDeadends;
}

void Graph::closeCrossroad (string& crossroad)
{
    if (!cityCrossroads[crossroad].isClosed())
        cityCrossroads[crossroad].changeStatusOfCrossroad();
}

void Graph::openCrossroad (string& crossroad)
{
    if (cityCrossroads[crossroad].isClosed())
        cityCrossroads[crossroad].changeStatusOfCrossroad();
}

vector<string> Graph::findClosed()
{
    vector<string> closedCrossroads;
    for(unordered_map<string, Vertex>::iterator it = cityCrossroads.begin(); it != cityCrossroads.end(); it++)
    {
        if(it -> second.isClosed())
        {
            closedCrossroads.push_back(it -> second.getName());
        }
    }
    return closedCrossroads;
}

vector<string> Graph::getOutStreets(string& crossroad)
{
    vector<string> tmp;
    vector<Edge> outStreets = cityCrossroads[crossroad].getOutStreets();
    for(int i = 0; i < outStreets.size(); i++)
        tmp.push_back(outStreets[i].getEnd().getName());
    return tmp;
}
