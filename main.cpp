#include <iostream>
#include "circlelist.h"
#include "dualinklist.h"
#include "staticstack.h"

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
   StaticStack<int, 5> l;

   for (int i = 0; i < 8; i++)
   {
        l.push(i);
   }

   return 0;
}
