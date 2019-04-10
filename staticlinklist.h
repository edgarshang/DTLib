#ifndef STATICLINKLIST_H
#define STATICLINKLIST_H

#include "linklist.h"

namespace DTLib
{
template<typename T, int N>
class Staticlinklist : public LinkList<T>
{
protected:
    typedef typename LinkList<T>::Node Node;
    struct SNode : public Node
    {
        void* operator new(unsigned long long size, void* loc)
        {
            (void)size;
            return loc;
        }
    };

    unsigned char m_space[sizeof(Node) * N];
    int m_used[N];

    Node* create()
    {
        Node* ret = nullptr;

        for (int i = 0; i < N; i++)
        {
            if(!m_used[i])
            {
                ret = reinterpret_cast<Node*>(m_space) + i;
                ret = new(ret)SNode();
                m_used[i] = 1;
                break;
            }
        }
        return ret;

    }

    void destory(Node* pn)
    {
        SNode* space = reinterpret_cast<SNode*>(m_space);
        SNode* psn = dynamic_cast<SNode*>(pn);

        for (int i = 0; i < N; i++)
        {
            if(pn == (space + i))
            {
                m_used[i] = 0;
                psn->~SNode();
                break;
            }
        }
    }

public:
    Staticlinklist()
    {
        for (int i = 0; i < N; i++)
        {
            m_used[i] = 0;
        }
    }

    int capacity()
    {
        return N;
    }

    ~Staticlinklist()
    {
        this->clear();
    }
};

}

#endif // STATICLINKLIST_H
