#ifndef DYNAMICLIST_H
#define DYNAMICLIST_H

#include "Seqlish.h"
#include "Exception.h"
namespace DTLib
{
template <typename T>
class Dynamiclish : public SeqLish<T>
{
protected:
    int m_capacity;
public:
    Dynamiclish(int capacity)   // 申请空间
    {
        this->m_array = new T[capacity];

        if(this->m_array != nullptr)
        {
            this->m_length = 0;
            this->m_capacity = capacity;
        }else
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "no memory to create object....");
        }
    }

    int capacity()const
    {
        return m_capacity;
    }

    // 重新设置顺序储存空间的大小
    void resize(int capacity)
    {
        if( capacity != m_capacity )
        {
            T* array = new T[capacity];

            if( array != nullptr )
            {
                int length = (this->m_length < capacity ? this->m_length : capacity);


                for (int i = 0; i < length; i++)
                {
                    array[i] = this->m_array[i];
                }

                T* temp = this->m_array;

                this->m_array = array;
                this->m_length = length;
                this->m_capacity = capacity;

                delete[] temp;
            }
            else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memory to resize Dynamiclic list ...");
            }
        }
    }
    ~Dynamiclish()
    {
        delete [] this->m_array;
    }
};
}

#endif // DYNAMICLIST_H
