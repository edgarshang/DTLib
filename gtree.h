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
protected:
    GTreeNode<T>* find(GTreeNode<T>* node, const T& value) const
    {
        GTreeNode<T>* ret = nullptr;
        if(node != nullptr)
        {
            if(node->value == value)
            {
                return node;
            }else {
                for (node->child.move(0); !node->child.end() && (ret == nullptr); node->child.next())
                {
                    ret = find(node->child.current(), value);
                }
            }
        }

        return ret;
    }

    GTreeNode<T>* find(GTreeNode<T>* node, GTreeNode<T>* obj) const
    {
        GTreeNode<T>* ret = nullptr;

        if(node == obj)
        {
            return node;
        }else {
            if(node != nullptr)
            {
                for (node->child.move(0); !node->child.end() && (ret == nullptr); node->child.next())
                {
                        ret = find(node->child.current(), obj);
                }
            }
        }

        return ret;
    }
public:
    bool insert(TreeNode<T>* node)
    {
        bool ret = true;
       find(dynamic_cast<GTreeNode<T>*>(node), 0);
        return ret;
    }
    bool insert(const T& value, TreeNode<T>* parent)
    {
        bool ret = true;
         find(dynamic_cast<GTreeNode<T>*>(parent), value);

        return ret;
    }
   SharedPointer< Tree<T>> remove(const T& value)
   {
       find(nullptr, value);
       return nullptr;
   }
    SharedPointer< Tree<T>> remove(TreeNode<T>* node)
   {
         find(dynamic_cast<GTreeNode<T>*>(node), 0);
       return nullptr;
   }
    GTreeNode<T>* find(const T& value)
   {
        return find(root(), value);
   }
    GTreeNode<T>* find(TreeNode<T>* node)
   {
        return find(root(), dynamic_cast<GTreeNode<T>*>(node));
   }
   GTreeNode<T>* root()
   {
       return dynamic_cast<GTreeNode<T>*>(this->m_root);
   }

   int degree()
   {
       root();
       return 0;
   }
     int count()
   {
         root();
         return 0;
   }
    int height()
   {
        root();
        return 0;
   }
   void clear()
   {
        this->m_root = nullptr;
   }

   GTree()
   {

   }

   ~GTree()
   {
       clear();
   }
};
}

#endif // GTREE_H
