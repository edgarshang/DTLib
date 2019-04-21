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

    mutable struct : public Object
    {
        char reserved[sizeof(T)];
        Node* next;
    }m_header;
    int m_length;
    int m_step;
    Node* m_current;

    Node* position(int i) const
    {
        Node* ret = reinterpret_cast<Node*>(&m_header);

        for (int p = 0; p < i ; p++)
        {
            ret = ret->next;
        }

        return  ret;
    }

    virtual Node* create()
    {
          return new Node();
    }

    virtual void destory(Node* pn)
    {
        delete pn;
    }
public:
    LinkList()
    {
        m_header.next = nullptr;
        m_length = 0;
        m_step = 1;
        m_current = nullptr;
    }

    int find(const T& e) const
    {
        int ret = -1;
        int i = 0;
        Node* node = m_header.next;

        while (node)
        {
            if(node->value == e)
            {
                ret = i;
                break;
            }else {
                node = node->next;
                i++;
            }
        }

        return ret;
    }

    bool insert(int i, const T& e)
    {
        bool ret = (( 0<=i ) && ( i <= m_length ));


        if(ret)
        {

           // std::cout << "insert" << std::endl;
            Node* node = create();

            if( node != nullptr )
            {

                Node *current = position(i);

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
            Node *current = position(i);
            std::cout << "before current->value = " << current->next->value << std::endl;;

            Node* toDel = current->next;
            current->next = toDel->next;

            std::cout << "toDel->value = " << toDel->value << std::endl;;
            std::cout << "current->value = " << current->next->value << std::endl;;

            destory(toDel);



            m_length--;
        }

        return  ret;
    }
    bool set(int i, const T& e)
    {
        bool ret = ((0 <= i )&& ( i < m_length));
        if( ret )
        {
            Node *current = position(i);

            current->next->value = e;
        }

        return  ret;
    }

    virtual T get(int i) const
    {
        T ret;
        if (get(i, ret))
        {
            return ret;
        }else {
            THROW_EXCEPTION(NoEnoughMemoryException, "abc");
        }
    }

    bool get(int i, T& e) const
    {
        bool ret = ((0 <= i )&& ( i < m_length));
        if( ret )
        {
          Node *current = position(i);

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

            destory(toDel);
        }

        m_length = 0;
    }

    virtual bool move(int i, int step = 1)
    {
        bool ret = ( 0 <= i ) && ( i < m_length ) && (step > 0);

        if( ret )
        {
            m_current = position(i)->next;
            m_step = step;
        }

        return ret;
    }

    virtual bool end()
    {
        return (m_current == nullptr);
    }

    virtual T current()
    {
        if(!end() )
        {
            return m_current->value;
        }else
        {
            THROW_EXCEPTION(InvalidOperationException, "No value at current position");
        }
    }

    virtual bool next()
    {
        int i = 0;

        while ((i < m_step)&& !end())
        {
            std::cout << "m_step = " << m_step << std::endl;
            m_current = m_current->next;
            i++;
        }

        return ( i == m_step );
    }

    ~LinkList()
    {
        clear();
    }

    // .....
};
}

#endif // LINKLIST_H
