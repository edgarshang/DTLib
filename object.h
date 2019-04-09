#ifndef OBJECT_H
#define OBJECT_H

namespace DTLib
{
class Object
{
public:
   void* operator new (unsigned long long size) throw();
   void operator delete (void* p);
   void* operator new[] (unsigned long long size) throw();
   bool operator == (const Object& object);
   bool operator != (const Object& object);
   void operator delete[] (void *p);
   virtual ~Object() = 0;
};
}


#endif // OBJECT_H