//
// Red Black Tree Definition
//

#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H

#include "Command.h"

enum Color { RED, BLACK, DOUBLE_BLACK };

struct Node
{
    int buffer_index, start, length;
    int color;
    Node *left, *right, *parent;

    Node(int, int, int);
};

class PieceTable
{
protected:
    class Buffer
    {
    public:
        std::vector<Command> commands;
    };
    std::vector<Buffer> buffers;
    Node* root;
    
    void rotateLeft(Node*&);
    void rotateRight(Node*&);
    void fixInsertRBTree(Node*&);
    void fixDeleteRBTree(Node*&);
    int getColor(Node*&);
    void setColor(Node*&, int);
    Node* minValueNode(Node*&);
    Node* maxValueNode(Node*&);
    Node* insertBST(Node*&, int pos, Node*&);
    Node* deleteBST(Node*&, int);
    int getBlackHeight(Node*);
    void insertValue(int pos, int buffer_index, int start, int length);
    void deleteValue(int);

public:
    PieceTable();
};


#endif //RED_BLACK_TREE_RBTREE_H
