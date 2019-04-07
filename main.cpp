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


using namespace std;
using namespace DTLib;


int main()
{
     LinkList<int> list;

    // cout << list.length() << endl;


  //  cout << "hello, wrold" << endl;
     for (int i = 0; i < 6; i++) {
       //  cout << "abc" << endl;
        list.insert(i);

     }
      // cout << "hello, wrold" << endl;

     for(int i = 0; i < list.length(); i++)
     {


//         cout <<  list.get(i); << endl;
         cout << list.get(i) << endl;

     }

     list.remove(2);

     cout << endl;

     for(int i = 0; i < list.length(); i++)
     {


//         cout <<  list.get(i); << endl;
         cout << list.get(i) << endl;

     }

     cout << list.find(-3) << endl;


    //  cout << "hello, wrold" << endl;




    return 0;
}
