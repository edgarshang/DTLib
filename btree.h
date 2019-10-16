#ifndef BTREE_H
#define BTREE_H

#include "treenode.h"
#include "tree.h"
#include "gtreenode.h"
#include "exception.h"
#include "linkqueue.h"

#include "btreenode.h"

namespace DTLib
{
template <typename T>
class BTree : public Tree<T>
{
protected:
    virtual BTreeNode<T>* find(BTreeNode<T>* node, const T& value)const
    {
        BTreeNode<T>* ret = nullptr;

        if( node != nullptr )
        {
            if( node->value == value )
            {
                ret = node;
            }else
            {
                if( ret == nullptr)
                {
                    ret = find(node->left, value);
                }

                if( ret == nullptr)
                {
                    ret = find(node->right, value);
                }
            }
        }

        return ret;
    }

    virtual BTreeNode<T>* find(BTreeNode<T>*node, BTreeNode<T>* obj)const
    {
        BTreeNode<T>* ret = nullptr;
        if( node == obj )
        {
            ret = node;
        }else
        {
            if(node != nullptr)
            {
                if( ret == nullptr)
                {
                    find(node->left, obj);
                }

                if( ret == nullptr )
                {
                    find(node->right, obj);
                }
            }
        }

        return ret;
    }

    virtual bool insert(BTreeNode<T>* n, BTreeNode<T>* np, BTNodePos pos)
    {
        bool ret = true;
        if(pos == ANY)
        {
            if( np->left == nullptr )
            {
                np->left = n;
            }else if( np->right == nullptr)
            {
                np->right = n;
            }else {
                ret = false;
            }
        }

        else if(pos == LEFT)
        {
            if(np->left == nullptr)
            {
                np->left = n;
            }else {
                ret = false;
            }
        }

        else if(pos == RIGHT)
        {
            if(np->right == nullptr)
            {
                np->right = n;
            }else {
                ret = false;
            }
        }
        else {
            ret = false;
        }
    }
public:
     bool insert(TreeNode<T>* node)
     {
         return insert(node, ANY);
     }

     virtual bool insert(TreeNode<T>* node, BTNodePos pos)
     {
         bool ret = true;

         if( node != nullptr)
         {
            if( this->m_root == nullptr)
            {
                node->parent = nullptr;
                this->m_root = node;
            }else {
                BTreeNode<T>* np = find(node->parent);

                if( np != nullptr)
                {
                    ret = insert(dynamic_cast<BTreeNode<T>*>(node), np, pos);
                }else {
                    THROW_EXCEPTION(InvalidParameterException, "Invalid parent tree node...");
                }
            }
         }else {
            THROW_EXCEPTION(InvalidParameterException, "parameter node can not be null");
        }
         return ret;
     }

     bool insert(const T& value, TreeNode<T>* parent)
     {
            return insert(value, parent, ANY);
     }

     bool insert(const T& value, TreeNode<T>* parent, BTNodePos pos)
     {
         int ret = true;

         BTreeNode<T>* node = BTreeNode<T>::NewNode();

         if(node == NULL)
         {
             THROW_EXCEPTION(NoEnoughMemoryException, "No memort to creat new node...");
         }else {
             node->value = value;
             node->parent = parent;

             ret = insert(node, pos);

             if( !ret )
             {
                 delete node;
             }
         }

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
     BTreeNode<T>* find(const T& value) const
     {
         return find(root(), value);
     }
     BTreeNode<T>* find(TreeNode<T>* node) const
     {
         return find(root(), dynamic_cast<BTreeNode<T>*>(node) );
     }
     BTreeNode<T>* root() const
     {
         return dynamic_cast<BTreeNode<T>*>(this->m_root);
     }
     int degree() const
     {
         return 0;
     }
     int count() const
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
     ~BTree()
    {
       clear();
    }
};
}

#endif // BTREE_H
