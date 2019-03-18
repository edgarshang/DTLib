#include <iostream>
#include "smartpointer.h"
#include "Exception.h"


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

int Demo2()
{
    try {
      //  throw Exception("test", __FILE__, __LINE__);
        THROW_EXCEPTION(ArithmeticException, "test");
    }
    catch (const ArithmeticException& e)
    {
            cout << "catch(const ArithmeticException& e)" << endl;
            cout << e.message() << endl;
            cout << e.location() << endl;
    }
    catch (const Exception& e)
    {
        cout << "catch(const Exception& e)" << endl;
        cout << e.message() << endl;
        cout << e.location() << endl;
    }

    return 0;
}
int main()
{
    cout << "Hello World!" << endl;

    Demo1();
    Demo2();

    SmartPointer<Test> sp = new Test();
    return 0;
}
