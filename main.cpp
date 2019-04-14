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

class Test : public Object
{
public:
    Test()
    {
        cout << "Test()" << endl;
    }
    ~Test()
    {
        cout << "~Test()" << endl;
    }
};





int main()
{
   SmartPointer<Test> sp = new Test();
   SmartPointer<Test> spn;

   spn = sp;
    return 0;
}
