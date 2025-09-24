//
// Red Black Tree Definition
//

#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H

enum Color { RED, BLACK, DOUBLE_BLACK };

struct Node
{
    int buffer_index, start, length;
    int color;
    Node *left, *right, *parent;

    Node(int, int, int);
};

class RBTree
{
protected:
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

public:
    RBTree();
    void insertValue(int pos, int buffer_index, int start, int length);
    void deleteValue(int);
};


#endif //RED_BLACK_TREE_RBTREE_H
