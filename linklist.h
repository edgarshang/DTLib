#ifndef LINKLIST_H
#define LINKLIST_H

#include "list.h"
#include "exception.h"
#include <iostream>



namespace DTLib
{

template < typename T >
class LinkList : public List<T>
{
protected:
    struct Node : public Object
    {
        T value;
        Node* next;
    };

    mutable Node m_header;
    int m_length;

public:
    LinkList()
    {
        m_header.next = nullptr;
        m_length = 0;
    }



    bool insert(int i, const T& e)
    {
        bool ret = (( 0<=i ) && ( i <= m_length ));


        if(ret)
        {

           // std::cout << "insert" << std::endl;
            Node* node = new Node();

            if( node != nullptr )
            {

                Node *current = &m_header;


                for (int p = 0; p < i; p++)
                {
                    current = current->next;
                }

                node->value = e;
                node->next = current->next;
                current->next = node;

                 m_length++;
            }else
            {
                THROW_EXCEPTION(NoEnoughMemoryException, "No memmory to insert");
            }
        }

        return ret;
    }

    bool insert(const T& e)
    {
       return insert(m_length, e);

      //  std::cout << "length() = " << length() << std::endl;
    }
    bool remove(int i)
    {
        bool ret = ((0 <= i )&& ( i < m_length));
        if( ret )
        {
            Node* current = &m_header;

            for (int p = 0; p < i; ++p)
            {
                current = current->next;
            }

            Node* toDel = current->next;
            current->next = toDel->next;

            delete toDel;

            m_length--;
        }

        return  ret;
    }
    bool set(int i, const T& e)
    {
        bool ret = ((0 <= i )&& ( i < m_length));
        if( ret )
        {
            Node* current = &m_header;

            for (int p = 0; p < i; ++p)
            {
                current = current->next;
            }

            current->next->value = e;
        }

        return  ret;
    }
    bool get(int i, T& e) const
    {
        bool ret = ((0 <= i )&& ( i < m_length));
        if( ret )
        {
            Node* current = &m_header;

            for (int p = 0; p < i; ++p)
            {
                current = current->next;
            }

            e = current->next->value;
        }

        return ret;
    }
    int length() const
    {
        return m_length;
    }

    void clear()
    {
        while (m_header.next)
        {
            Node* toDel = m_header.next;

            m_header.next = toDel->next;

            delete  toDel;
        }

        m_length = 0;
    }

    ~LinkList()
    {
        clear();
    }

    // .....
};
}

#endif // LINKLIST_H
