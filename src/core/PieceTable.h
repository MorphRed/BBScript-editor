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
    int left_subtree_length = 0;

    Node(int, int, int);
};

class PieceTable
{
protected:
    std::vector<std::vector<Command>> buffers;
    Node* root;
    Node* split_buffer = nullptr;

    void rotateLeft(Node*&);
    void rotateRight(Node*&);
    void fixInsertRBTree(Node*&);
    void fixDeleteRBTree(Node*&);
    static int getColor(Node*&);
    static void setColor(Node*&, int);
    static Node* minValueNode(Node*&);
    static Node* maxValueNode(Node*&);
    static void updateParentSubtreeLen(Node*& ptr, int length);
    void insertBST(Node*&, int pos, Node*&);
    static Node* deleteBST(Node*&, int);
    static int getBlackHeight(Node*);
    void insertValue(int pos, int buffer_index, int start, int length);
    void deleteValue(int);

public:
    void appendToBuffer(Node*& ptr, std::vector<Command>& commands);
    void insert(int line, std::vector<Command>& commands);
    explicit PieceTable(const std::vector<Command>& commands);
    PieceTable();
};


#endif //RED_BLACK_TREE_RBTREE_H
