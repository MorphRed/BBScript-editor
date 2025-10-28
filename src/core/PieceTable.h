//
// Red Black Tree Definition
//

#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H

#include "Command.h"

enum Color { RED, BLACK, DOUBLE_BLACK };

enum BufferIndex
{
    ORIGINAL,
    APPEND
};

struct Node
{
    BufferIndex index;
    int start, length;
    int color;
    Node *left, *right, *parent;
    int left_subtree_length = 0;

    Node(BufferIndex, int, int);
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
    static Node* getNode(Node* target, int& pos);
    static Node* getNextNode(Node* target);
    static Node* minValueNode(Node*&);
    static Node* maxValueNode(Node*&);
    static void updateParentSubtreeLen(const Node* ptr, int length);
    void insertBST(Node*&, int pos, Node*);
    static Node* deleteBST(Node*&, int);
    static int getBlackHeight(Node*);
    void insertValue(int pos, int start, int length);
    void deleteValue(int);

public:
    void appendToBuffer(Node* ptr, const std::vector<Command>& commands);
    void insert(int pos, const std::vector<Command>& commands);
    std::vector<Command*> getCommand(int pos, int length);
    void debug();
    explicit PieceTable(const std::vector<Command>& commands);
    PieceTable();
};


#endif //RED_BLACK_TREE_RBTREE_H
