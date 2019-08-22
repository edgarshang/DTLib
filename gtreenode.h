#ifndef GTREENODE_H
#define GTREENODE_H

#include "treenode.h"
#include "linklist.h"

namespace DTLib
{
template <typename T>
class GTreeNode : public TreeNode<T>
{
public:
    LinkList<GTreeNode<T>*> child;
    GTreeNode()
    {
        m_flag = false;
    }
protected:
    bool m_flag;

    void * operator new(long unsigned int size) throw()
    {
        return Object::operator new(size);
    }

    bool flag()
    {
        return m_flag;
    }

    static GTreeNode<T>* NewNode()
    {
        GTreeNode<T>* ret = new GTreeNode<T>();

        if(ret != nullptr)
        {
            ret->m_flag = true;
        }
    }
};
}

#endif // GTREENODE_H
