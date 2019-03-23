#include <iostream>
#include "Smartpointer.h"
#include "Exception.h"
#include "Object.h"
#include "Seqlish.h"


using namespace std;
using namespace DTLib;

class TestObject : public Object
{
public:
    int i;
    int j;

};

class Child : public TestObject
{
public:
    int k;
};

void Demo3()
{
    Object* obj1 = new TestObject();
    Object* obj2 = new Child();

    cout << "obj1 = " << obj1 << endl;
    cout << "obj2 = " << obj2 << endl;

    delete  obj1;
    delete  obj2;
}

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
        THROW_EXCEPTION(InvalidParameterException, "test");
    }
    catch (const ArithmeticException& e)
    {
            cout << "catch(const ArithmeticException& e)" << endl;
            cout << e.message() << endl;
            cout << e.location() << endl;
    }
    catch (const InvalidParameterException& e)
    {
            cout << "catch(const InvalidParameterException& e)" << endl;
            cout << e.message() << endl;
            cout << e.location() << endl;
    }
    catch(const IndexOutOfBoundException& e)
    {
        cout << "catch(const IndexOutOfBoundException& e)" << endl;
        cout << e.message() << endl;
        cout << e.location() << endl;
    }
    catch(const NoEnoughMemory& e)
    {
        cout << "catch(const NoEnoughMemory& e)" << endl;
        cout << e.message() << endl;
        cout << e.location() << endl;
    }

    catch(const NullPointerException& e)
    {
        cout << "catch(const NullPointerException& e)" << endl;
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
    SeqLish<int>* l;
//    SmartPointer<int> *sp = new SmartPointer<int>();

//    delete  sp;
//    Test *test = new Test();
//    SmartPointer<Test> t1 = new Test();
 //   SmartPointer<Exception> e1 = new ArithmeticException();
    return 0;
}
