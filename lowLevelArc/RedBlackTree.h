#ifndef BACKEND_REDBLACKTREE_H
#define BACKEND_REDBLACKTREE_H

#include <cstring>
#include <iostream>
#include "order.h"
#include <queue>


namespace sdds {
    struct Node {
        double data{};
        Node *parent{};
        Node *left{};
        Node *right{};
        int color{};
        std::queue<Order> orderQueue;
    };

    typedef Node *NodePtr;

    class RedBlackTree {
    private:
        NodePtr root;
        NodePtr TNULL;

        void initializeNULLNode(NodePtr node, NodePtr parent);
        void preOrderHelper(NodePtr node);
        void inOrderHelper(NodePtr node);
        void postOrderHelper(NodePtr node);
        NodePtr searchTreeHelper(NodePtr node, double key);
        void deleteFix(NodePtr x);
        void rbTransplant(NodePtr u, NodePtr v);
        void deleteNodeHelper(NodePtr node, double key);
        void insertFix(NodePtr k);
        void printHelper(NodePtr root, std::string indent, bool last);

        void preorder();
        void postorder();


        NodePtr successor(NodePtr x);
        NodePtr predecessor(NodePtr x);
        void leftRotate(NodePtr x);
        void rightRotate(NodePtr x);

    public:
        RedBlackTree();
        NodePtr searchTree(double k);
        std::queue<Order>* insert(double key);
        NodePtr minimum(NodePtr node);
        NodePtr maximum(NodePtr node);
        Node * minValue();
        Node * maxValue();
        NodePtr getRoot();
        void inorder();
        void printTree();
        void deleteNode(double data);
    };

}

#endif //BACKEND_REDBLACKTREE_H
