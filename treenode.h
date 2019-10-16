#ifndef TREENODE_H
#define TREENODE_H

#include "object.h"

namespace DTLib
{
template <typename T>
class TreeNode : public Object
{
protected:
    bool m_flag;
    TreeNode(TreeNode<T>&);
    TreeNode<T>& operator = (const TreeNode<T>&);

    void * operator new(long long unsigned int size) throw()
    {
        return Object::operator new(size);
    }
public:
    T value;
    TreeNode<T> *parent;

    TreeNode()
    {
         m_flag = false;
        parent = nullptr;

    }

    bool flag()
    {
        return m_flag;
    }

    virtual ~TreeNode() = 0;
};

template <typename T>
TreeNode<T>::~TreeNode()
{

}
}

#endif // TREENODE_H
