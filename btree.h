#ifndef BTREE_H
#define BTREE_H

#include "treenode.h"
#include "tree.h"
#include "gtreenode.h"
#include "exception.h"
#include "linkqueue.h"

#include "btreenode.h"
#include "array.h"
#include "dynamicarray.h"

namespace DTLib
{

enum BTTraveral
{
    PreOrder,
    IndOrder,
    PostOrder
};

template <typename T>
class BTree : public Tree<T>
{
protected:
    LinkQueue<BTreeNode<T>*> m_queue;
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

    virtual void remove(BTreeNode<T>* node, BTree<T>*& ret)
    {
        ret = new BTree<T>();

        if( ret == nullptr )
        {
            THROW_EXCEPTION(NoEnoughMemoryException, "No Enough memory to create new tree...");
        }else {
            if( root() == node){
                this->m_root = nullptr;
            }else {
                BTreeNode<T>* parent = dynamic_cast<BTreeNode<T>*>(node->parent);

                if( parent->left == node )
                {
                    parent->left = nullptr;
                }else if (parent->right == node) {
                    parent->right = nullptr;

                }

                node->parent = nullptr;
            }

            ret->m_root = node;
        }
    }

    virtual void free(BTreeNode<T>* node)
    {
        if( node != nullptr)
        {
            free(node->left);
            free(node->right);

            if( node->flag())
            {
                delete node;
            }
        }
    }

    int count(BTreeNode<T>* node) const
    {
//        int ret = 0;
//        if(node != nullptr)
//        {
//            ret = count(node->left) + count(node->right) + 1;
//        }

//        return ret;
        return (node != nullptr) ? (count(node->left) + count(node->right) + 1) : (0);
    }

    int height(BTreeNode<T>* node) const
    {
        int ret = 0;
        if(node != nullptr)
        {
            int lh = height(node->left);
            int rh = height(node->right);

            ret = ((lh > rh) ? lh : rh) + 1;
        }
        return ret;
    }

    int degree(BTreeNode<T>* node) const
    {
        int ret = 0;
        // 因为二叉树的最大度为2，所以只要检测到有一个节点的子节点数为2，就可以判定这棵树的度为2
        if(node != nullptr)
        {
            BTreeNode<T>* chile[] = {node->left, node->right};
             ret = (!!node->left + !!node->right);

             for(int i = 0; (i<2)&&(ret < 2); i++)
             {
                 int d = degree(chile[i]);
                 if( ret < d )
                 {
                     ret = d;
                 }
             }

        }

        return ret;
    }

    void preOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node != nullptr)
        {
            queue.add(node);
            preOrderTraversal(node->left, queue);
            preOrderTraversal(node->right, queue);
        }
    }

    void inOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node != nullptr)
        {
            inOrderTraversal(node->left, queue);
            queue.add(node);
            inOrderTraversal(node->right, queue);
        }
    }

    void postOrderTraversal(BTreeNode<T>* node, LinkQueue<BTreeNode<T>*>& queue)
    {
        if(node != nullptr)
        {
            postOrderTraversal(node->left, queue);
            postOrderTraversal(node->right, queue);
            queue.add(node);
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
         BTree<T>* ret = nullptr;
         BTreeNode<T>* node = find(value);

         if(node == nullptr)
         {
             THROW_EXCEPTION(InvalidParameterException, "Can not find the tree via value...");

         }else {
             remove(node, ret);
             m_queue.clear();
         }

         return ret;
     }
     SharedPointer< Tree<T>> remove(TreeNode<T>* node)
     {
         BTree<T>* ret = nullptr;

         node = find(node);

         if(node == nullptr)
         {
             THROW_EXCEPTION(InvalidParameterException, "parameter node is invalid...");

         }else {
             remove(dynamic_cast<BTreeNode<T>*>(node), ret);
             m_queue.clear();
         }
         return ret;
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
         return degree(root());
     }
     int count() const
     {
         return count(root());
     }
     int height()
     {
         return height(root());
     }
     void clear()
     {
         free(root());
         m_queue.clear();
         this->m_root = nullptr;
     }

     bool begin()
     {
         bool ret = (root() != nullptr);
         if( ret )
         {
              m_queue.clear();
              m_queue.add(root());
         }

         return ret;
     }

     bool end()
     {
         return (m_queue.length() == 0);
     }

     bool next()
     {
         bool ret = (m_queue.length() > 0);
         if( ret )
         {
           BTreeNode<T>* node = m_queue.front();

           m_queue.remove();

           if(node->left != nullptr)
           {
               m_queue.add(node->left);
           }
           if(node->right != nullptr)
           {
               m_queue.add((node->right));
           }

         }

     }

     T current()
     {
         if(!end())
         {
             return m_queue.front()->value;
         }else {
              THROW_EXCEPTION(InvalidOperationException, "No value at current position...");
          }
     }

     SharedPointer<Array<T>> traversal( BTTraveral order)
     {
        DynamicArray<T>* ret  = nullptr;
        LinkQueue<BTreeNode<T>*> queue;
        switch (order) {
        case PreOrder:
            preOrderTraversal(root(), queue);
            break;
        case IndOrder:
            inOrderTraversal(root(), queue);
            break;
        case PostOrder:
            postOrderTraversal(root(), queue);
            break;
        default:
            THROW_EXCEPTION(InvalidParameterException,"Parameter order is invalid...");
            break;
        }

        ret = new DynamicArray<T>(queue);

        if( ret != nullptr )
        {
            for (int i = 0; i < ret->length(); ++i) {
                ret->set(i, queue.front()->value);
            }
        }else {
            THROW_EXCEPTION(NoEnoughMemoryException, "No memory to create return array...");
        }
        return  ret;
     }


     ~BTree()
    {
       clear();
    }
};
}

#endif // BTREE_H
