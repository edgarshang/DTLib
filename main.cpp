#include <iostream>
#include "smartpointer.h"
using namespace std;
using namespace DTLib;

class Test
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
    cout << "Hello World!" << endl;

    SmartPointer<Test> sp = new Test();
    return 0;
}
