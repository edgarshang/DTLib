#ifndef GTREE_H
#define GTREE_H

#include "treenode.h"
#include "tree.h"
#include "gtreenode.h"

namespace DTLib
{
template <typename T>
class GTree : public Tree<T>
{
public:
    bool insert(TreeNode<T>* node)
    {
        bool ret = true;

        return ret;
    }
    bool insert(const T& value, TreeNode<T>* parent)
    {
        bool ret = true;

        return ret;
    }
   SharedPointer< Tree<T>> remove(const T& value)
   {
       return nullptr;
   }
    SharedPointer< Tree<T>> remove(TreeNode<T>* node)
   {
       return nullptr;
   }
    GTreeNode<T>* find(const T& value)
   {
        return nullptr;
   }
    GTreeNode<T>* find(TreeNode<T>* node)
   {
        return nullptr;
   }
   GTreeNode<T>* root()
   {
       return dynamic_cast<GTreeNode<T>*>(this->m_root);
   }

   int degree()
   {
       return 0;
   }
     int count()
   {
         return 0;
   }
    int height()
   {
        return 0;
   }
   void clear()
   {
        this->m_root = nullptr;
   }

   ~GTree()
   {
       clear();
   }
};
}

#endif // GTREE_H
