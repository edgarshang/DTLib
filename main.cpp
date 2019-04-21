#include <iostream>
#include "circlelist.h"
#include "dualinklist.h"

using namespace std;
using namespace DTLib;


void josephus(int n, int s, int m)
{
    CircleList<int> cl;

    for (int i = 1; i <= n; i++)
    {
        cl.insert(i);
    }

    cl.move(s-1, m-1 );

    while (cl.length() > 0)
    {
        cl.next();

        cout <<cl.current() << endl;

       cl.remove(cl.find(cl.current()));
     }
}



int main()
{
    DuaLinkList<int> dl;

    for (int i = 0; i < 5; ++i)
    {
        dl.insert(0, i);
    }

    for (int i = 0; i < dl.length(); ++i)
    {
        cout << dl.get(i) << endl;
    }

    cout << "begin" << endl;

    for (dl.move(dl.length() - 1); !dl.end(); dl.pre())
    {
        cout <<dl.current() << endl;
    }

    cout <<"end" <<endl;
   return 0;
}
