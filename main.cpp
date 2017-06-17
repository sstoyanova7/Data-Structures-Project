#include <iostream>
#include <stdexcept>
using namespace std;

#include "Graph.h"

void interactive()
{
    char rowC[100];
    string url;
    Graph cityMap;
    string crossroad;

    cin >> url;
    cin >> crossroad;
    cityMap.loadFile(url);

    while(cin.getline(rowC,100))
    {
        string row = rowC;
        if(row.find("location") == 0)
        {
            cout << crossroad;
        }
        if(row.find("change") == 0)
        {
            crossroad = row.substr(7, row.size());
        }
        if(row.find("neighbours") == 0)
        {
            vector<string> outStreets = cityMap.getOutStreets(crossroad);
            for(int i = 0; i < outStreets.size(); i++)
                cout << outStreets[i] << endl;
        }
        if(row.find("move") == 0)
        {
            string oldCrossroad = crossroad;
            crossroad = row.substr(5, row.size());
            queue<string> route = cityMap.shortestRoute(oldCrossroad, crossroad);
            while (!route.empty())
            {
                cout << route.front() << ' ';
                route.pop();
            }
        }
        if(row.find("close ") == 0)
        {
            string crossroadName = row.substr(6, row.size());
            cityMap.closeCrossroad(crossroadName);
        }
        if(row.find("open") == 0)
        {
            string crossroadName = row.substr(5, row.size());
            cityMap.openCrossroad(crossroadName);
        }
        if(row.find("closed") == 0)
        {
            vector<string> closedCrossroads = cityMap.findClosed();
            for (int i = 0; i < closedCrossroads.size(); i++)
                cout << closedCrossroads[i] << ' ';
        }
    }
}


int main()
{
    Graph test;
    string url = "test.txt";
    test.loadFile(url);
    string a = "A", b = "B", c = "C", d = "D", f = "F", g = "G";
    //hasRoute test
    //cout << test.hasRoute(c,a) << endl; //expected 1

    //shortestRoad test
    /*
    queue<string> shortestRouteTest = test.shortestRoute(a,d);
    while(!shortestRouteTest.empty())
    {
        cout << shortestRouteTest.front() << " ";
        shortestRouteTest.pop();
    }*/

    //hasCycle test
    /*cout << test.hasCycle(a);
    cout << test.hasCycle(f);*/

    //areAllAvailable test
    //cout << test.areAllAvailable(b);

    //deadends test
    /*vector<string> deadendsTest = test.deadends();
    for(int i = 0; i <deadendsTest.size(); i++)
        cout << deadendsTest[i] << " ";*/

    interactive();






    return 0;
}
