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

        void addCrossroad (string&); //добавя връх (кръстовище) в графа
        void addStreet (string&, string&, unsigned); //добавя ребро (улица) в графа
        void loadFile (string&); //функцията, четяща от файла

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

        //1. Проверка дали има път между две зададени кръстовища,
        bool hasRoute (string&, string&);
        //2. Намиране на най-кратък път между две кръстовища
        //+3. При наличието на затворени кръстовища
        //(които да бъдат подавани като списък) се намира алтернативен
        //най-кратък път между двете кръстовища
        queue<string> shortestRoute (string&, string&);
        //4. Проверка дали при дадено кръстовище за начална точка
        //е възможно да обиколим част от града и да се върнем
        //там, от където сме тръгнали
        bool hasCycle (string&);
        //5. Проверка дали можем да направим пълна туристическа обиколка
        //на всички улици без да минаваме по една и съща улица два пъти
        queue<string> tour (string&); //
        //6. Проверка дали е възможно да стигнем от дадено кръстовище
        // до всички останали
        bool areAllAvailable (string&);
        //7. Намиране на всички задънени улици
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
