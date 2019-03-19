#include <iostream>
#include "Object.h"
#include <cstdlib>

using namespace std;

namespace DTLib {


void* Object::operator new(unsigned long long size) throw()
{
    cout << "Object::operator new size = " << size << endl;
    return malloc(size);
}

void Object::operator delete(void* p)
{
    cout << "Object::operator delete p = " << p << endl;
    free(p);
}

void* Object::operator new[] (unsigned long long size) throw()
{
    cout << "Object::operator new[]" << endl;
    return malloc(size);
}

void Object::operator delete[](void* p)
{

    cout << "Object::operator delete[]" << endl;
    free(p);
}

Object::~Object()
{

}
}