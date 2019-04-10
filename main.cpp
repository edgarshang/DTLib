#include <iostream>
#include "smartpointer.h"
#include "exception.h"
#include "object.h"
#include "seqlish.h"
#include "staticlist.h"
#include "dynamiclist.h"
#include "staticArray.h"
#include "dynamicarray.h"
#include "linklist.h"
#include "staticlinklist.h"


using namespace std;
using namespace DTLib;


int main()
{
     Staticlinklist<int, 7> list;

     for (int i = 0; i < 5; i++)
     {
        list.insert(0, i);
     }

     for (list.move(0); !list.end(); list.next())
     {
            cout << list.current() << endl;
     }

    return 0;
}
