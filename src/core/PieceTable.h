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
    Color color;
    Node *left, *right, *parent;
    int left_subtree_length = 0;

    Node(BufferIndex, int start, int length);
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
    static void setColor(Node*&, Color);
    static Node* getNode(Node* target, int& pos);
    static Node* getNextNode(Node* target);
    static Node* minValueNode(Node*&);
    static Node* maxValueNode(Node*&);
    void insertBST(Node*&, int pos, Node*&);
    static Node* deleteBST(Node*&, int pos, int length);
    static int getBlackHeight(Node*);
    void insertValue(int pos, int start, int length);
    void deleteValue(int pos, int length);

public:
    void appendToBuffer(Node* node, const std::vector<Command>& commands);
    void insert(int pos, const std::vector<Command>& commands);
    std::vector<Command*> getCommand(int pos, int length);
    explicit PieceTable(const std::vector<Command>& commands);
    PieceTable();
};

void printTree(const Node* curr, int depth);


#endif //RED_BLACK_TREE_RBTREE_H
