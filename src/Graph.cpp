#include <fstream>
#include <stack>
#include <stdexcept>
#include "Graph.h"

//добавя кръстовище към структурата
void Graph::addCrossroad(string& name)
{
    //ако кръстовището го няма в хеш таблицата
    //създаваме празно кръстовище(без улици)
    if (cityCrossroads.count(name) == 0)
    {
        cityCrossroads[name] = Vertex(name);
    }
}

//добавя улица към структурата (свързва две кръстовиша)
void Graph::addStreet(string& start, string& end, unsigned length)
{
    //ако не съществуват двете кръстовища ги създава
    addCrossroad(start);
    addCrossroad(end);
    cityCrossroads[start].addOutStreet(cityCrossroads[end], length);
}

void Graph::loadFile(string& url)
{
    ifstream file (url); //отваряме файла
    if (file.is_open())  //ако е намерен файла
    {
        string row;
        //обхождаме всеки ред по отделно
        while (getline(file, row))
        {
            string startCrossroad, endCrossroad;
            unsigned length, counter, start = 0;
            counter = row.find(' ');
            //записваме първото кръстовище от реда
            startCrossroad = row.substr(start, counter - start);
            //и го добавяме към графа
            addCrossroad(startCrossroad);
            //изместваме старт
            start = counter;
            //докато има нещо на реда
            //записваме кръстовище -> дължина -> кръстовище -> дължина....
            while (start < row.size())
            {
                start ++;
                counter = row.find (' ', start);
                endCrossroad = row.substr (start, counter - start);
                start = counter+1;
                length = 0;
                //записваме дължината като число
                while (row[start] != ' ' && start < row.size())
                {
                    length *= 10;
                    length += row[start] - '0';
                    start++;
                }
                //добавяме улицата със съответните начално кръстовище,
                //крайно кръстовище и дължина на самата улица.
                //не добавяме кръстовищата по отделно, защото
                //във функцията addStreet() това се проверява
                //и ако ги няма се извиква addCrossroad()
                addStreet(startCrossroad, endCrossroad, length);
            }
        }
    }
    else //ако не е намерен файла
    {
        throw domain_error("File not found");
    }
}

//1. Проверка дали има път между две зададени кръстовища,
//като използваме BFS
bool Graph::hasRoute(string& start, string& end)
{
    vector<string> reachable; //кръстовищата, до които можем да стигнем от start
    queue<string> unReached; //кръстовища, за които трябва да проверим
                             //до къде можем да стигнем

    unReached.push(start);
    reachable.push_back(start);
    while(!unReached.empty())
    {

        vector<Edge> outStreets = cityCrossroads[unReached.front()].getOutStreets();
        unReached.pop();
        for(size_t i = 0; i < outStreets.size(); i++)
        {
            //ако кръстовището е затворено го пропускаме
            if(outStreets[i].getEnd().isClosed())
                continue;

            bool flag = 1; //има ли го кръстовището в reachable(0 -> да, 1 -> не)
            for(size_t j = 0; j < reachable.size()&&flag; j++)
            {
                //Ако кръстовището вече, го има в reachable
                if(reachable[j] == outStreets[i].getEnd().getName())
                {
                    flag = 0;
                }
            }
            //ако кръстовището го няма в reachable го добавяме
            // reachable и в unReached
            if(flag)
            {   //ако сме стигнали до кръстовището, което искаме
                //връшаме true
                if(outStreets[i].getEnd().getName() == end)
                    return 1;
                //ако не сме стигнали оше, добавяме следвашите кръстовища
                //за проверка
                reachable.push_back(outStreets[i].getEnd().getName());
                unReached.push(outStreets[i].getEnd().getName());
            }
        }
    }
    return 0;
}

//2. Намиране на най-кратък път между две кръстовища
//+3. При наличието на затворени кръстовища
//(които да бъдат подавани като списък) се намира алтернативен
//най-кратък път между двете кръстовища,
//като използваме Алгоритъм на Дийкстра
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

        //в опашката ще ни трябва
        //обекта с по-голяма дистанция да е
        //с по-малък приоритет => сравняваме по
        //distance и връшаме true ако
        //левият обект е > от десният обект
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
    vector<string> done; //записваме обходените върхове
    unordered_map<string, helper> shortestsRoutes; //държим минималните разстояния
                                                   //за всеки връх

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
            {             //ако кръстовището е затворено
                continue; //го пропускаме
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

//Проверяваме дали можем да стигнем до началното
//кръстовище като минем през някое друго
//използваме малко изменено DFS(ако намерим
//един цикъл DFS-то спира
bool Graph::hasCycle(string& start)
{
    vector<string> visited;
    return hasCycleHelper(start, start, visited);
}

//помощна, рекиурсивна функция
//подаваме начално кръстовище(до което искаме да се върнем),
//cursor е кръстовището до което сме стигнали
//и масив, с вече обходените (за да не напраивм цикъл не където ни трябва
bool Graph::hasCycleHelper(string& start, string& cursor, vector<string>& visited)
{
    //записваме улиците, до които можем да стигнем от cursor
    vector<Edge> outStreets = cityCrossroads[cursor].getOutStreets();
    //и обхождаме всяка от тях
    for (int i = 0; i < outStreets.size(); i++)
    {
        //записваме името на кръстовището, което можем да достъпим от cursor
        string tmp = outStreets[i].getEnd().getName();
        //прекъсваме функцията, ако това кръстовище е началното
        if (start == tmp)
            return 1;
        bool flag = 1; // 1-> не сме обходили кръстовищетп, -> 0 обходили сме го
        //проверяваме дали кръстовището е обходено
        for (int j = 0; j < visited.size() && flag; j++)
        {
            if (tmp == visited[j])
                flag = 0;
        }
        //и ако не е го добавяме към обходените и влизаме рекурсивно
        //във функцията като за cursor даваме това кръстовище
        if (flag)
        {
            visited.push_back(tmp);
            if (hasCycleHelper(start, tmp, visited))
                return 1;
        }
    }
    return 0;
}


//проверява дали може да се направи
//туристическа обиколка на всички улици(без
//да се минава два пъти по една и съща улица)
//(дали има Ойлеров път)
queue<string> Graph::tour(string& start)
{
    for (unordered_map<string, Vertex>::const_iterator it = cityCrossroads.begin(); it != cityCrossroads.end(); it++)
    {
        //броя на входните ребра != на броя на изходните е
        //достатъчно условие да не съществува Ойлеров път
        if (it -> second.getOutStreets().size() != it -> second.getInStreets().size())
        {
            cout << "Sorry! Tour is not available." << endl;
            return queue<string>();
        }
    }
}

//проверява дали има път до всяко от кръстовищата
//използвайки помощната функция, която
//проверява за съществуване на цикъл
bool Graph::areAllAvailable(string& start)
{
    vector<string> visited;
    visited.push_back(start);
    //даваме цикъл към нещо, което сме сигурни, че гоняма
    //=> рекурсивната функция ще ни върне 0,
    //но нас ни вълнува страничният и ефект
    //(напълнения вектор с посетени кръстовища)
    //и ако този вектор и самата структура
    //съдържат равен брой кръстовиша => че има достъп до всички кръстовища
    string emptyString = "";
    hasCycleHelper(emptyString, start, visited);
    return visited.size() == cityCrossroads.size();
}


//връща всички кръстовища, от които няма outStreets
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
