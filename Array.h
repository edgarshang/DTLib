#ifndef ARRAY_H
#define ARRAY_H

#include "Object.h"
#include "Exception.h"
namespace DTLib
{
template  <typename T>
class Array:public Object
{
protected:
    T* m_array;
public:
    virtual bool set(int i, const T&e)
    {
        bool ret = ((0 <= i) &&(i < length()));

        if( ret )
        {
            m_array[i] = e;
        }

        return ret;
    }

    virtual bool get(int i, T& e)const
    {
        bool ret = ((0 <= i) &&(i < length()));
        if(ret)
        {
            e = m_array[i];
        }

        return ret;
    }

    T& operator[] (int i)  // O(1)
    {
        if((0 <= i) &&(i < length()))
        {
            return m_array[i];
        }else {
            THROW_EXCEPTION(IndexOutOfBoundException, "Para i is valid...");
        }
    }
    T operator[] (int i)const  //  O(1)
    {
        return (const_cast<Array<T>&>(*this))[i];
    }

    virtual int length() const = 0;
};
}

#endif // ARRAY_H
