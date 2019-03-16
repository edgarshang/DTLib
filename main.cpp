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

void Demo1()
{
    try {
        throw 'c';
    } catch (int)
    {
        cout << "catch(int i) " << endl;
    }
    catch (double d)
    {
        cout << "catch(double d)" << endl;
    }
    catch (char c)
    {
        cout << "catch(char i)" << endl;
    }
}


int main()
{
    cout << "Hello World!" << endl;

    Demo1();

    SmartPointer<Test> sp = new Test();
    return 0;
}
