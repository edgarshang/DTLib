#ifndef SEQLISH_H
#define SEQLISH_H

#include "list.h"
#include "exception.h"



namespace DTLib {

template <typename T>
class SeqLish : public List<T>
{
protected:
    T* m_array;    // 顺序存储空间
    int m_length;  // 当前线性表长度
public:

    bool insert(const T& e)
    {
        return insert(m_length, e);
    }

    bool insert(int i, const T& e)
    {
        bool ret = ((0 <= i) && (i <= m_length));

        ret = ret && (m_length < capacity());
        if(ret)
        {
            for (int p = m_length - 1; p >= i; p--)
            {
                m_array[p+1] = m_array[p];
            }

            m_array[i] = e;

            m_length++;
        }

        return ret;
    }
    bool remove(int i)
    {
        bool ret = ((0 < i) && (i < m_length));

        if( ret )
        {
            for (int p = i; p < m_length - 1; p++)
            {
                m_array[p] = m_array[p+1];
            }

            m_length--;
        }

        return ret;
    }

    bool set(int i, const T& e)
    {
        bool ret = (( 0 <= i) && ( i < m_length));

        if( ret )
        {
            m_array[i] = e;
        }

        return ret;
    }

    bool get(int i, T& e) const
    {
         bool ret = (( 0 <= i) && ( i < m_length));

         if( ret )
         {
             e = m_array[i];
         }

         return ret;
    }
    int length() const
    {
        return m_length;
    }

    void clear()
    {
        m_length = 0;
    }

    // 顺序存储线性表的数组访问方式
    T& operator[] (int i)
    {
        if((0<= i) && (i < m_length))
        {
            return m_array[i];
        }else
        {
            THROW_EXCEPTION(IndexOutOfBoundException, "Parameter i is invalid ...");
        }
    }
    T operator[] (int i) const
    {
        return (const_cast<SeqLish<T>&>(*this))[i];
    }
    // 顺序存储空间的容量
    virtual int capacity() const = 0;
};
}




#endif // SEQLISH_H
