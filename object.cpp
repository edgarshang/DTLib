#include <iostream>
#include "object.h"
#include <cstdlib>

using namespace std;

namespace DTLib {


void* Object::operator new(long long unsigned int size) throw()
{
   // cout << "Object::operator new size = " << size << endl;
    return malloc(size);
}

void Object::operator delete(void* p)
{
  //  cout << "Object::operator delete p = " << p << endl;
    free(p);
}

bool Object::operator==(const Object &object)
{
    return (this == &object);
}

bool Object::operator!=(const Object &object)
{
    return (this != &object);
}

void* Object::operator new[] (long long unsigned int size) throw()
{
   // cout << "Object::operator new[]" << endl;
    return malloc(size);
}

void Object::operator delete[](void* p)
{

  //  cout << "Object::operator delete[]" << endl;
    free(p);
}

Object::~Object()
{

}
}
