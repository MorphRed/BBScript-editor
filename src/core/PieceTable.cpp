//
// Red Black Tree Implementation
//
#include <bits/stdc++.h>
#include "PieceTable.h"

using namespace std;

Node::Node(const int buffer_index, const int start, const int length)
{
    this->buffer_index = buffer_index;
    this->start = start;
    this->length = length;
    color = RED;
    left = right = parent = nullptr;
}

PieceTable::PieceTable()
{
    root = nullptr;
}

int PieceTable::getColor(Node*& node)
{
    if (node == nullptr)
        return BLACK;

    return node->color;
}

void PieceTable::setColor(Node*& node, int color)
{
    if (node == nullptr)
        return;

    node->color = color;
}

Node* PieceTable::insertBST(Node*& root, const int pos, Node*& ptr)
{
    // this insert the node ptr inside the structure of root
    if (root == nullptr)
        return ptr;
    int root_pos = 0;
    for (auto tmp = root; tmp != nullptr; tmp = tmp->left)
    {
        root_pos += tmp->length;
    }
    if (pos < root_pos)
    {
        root->left = insertBST(root->left, pos, ptr);
        root->left->parent = root;
    }
    else if (pos > root_pos)
    {
        root->right = insertBST(root->right, pos, ptr);
        root->right->parent = root;
    }
    else
    {
        //todo split the node if insidey
    }

    return root;
}

void PieceTable::insertValue(const int pos, const int buffer_index, const int start, const int length)
{
    Node* node = new Node(buffer_index, start, length);
    root = insertBST(root, pos, node);
    fixInsertRBTree(node);
}

void PieceTable::rotateLeft(Node*& ptr)
{
    Node* right_child = ptr->right;
    ptr->right = right_child->left;

    if (ptr->right != nullptr)
        ptr->right->parent = ptr;

    right_child->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = right_child;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = right_child;
    else
        ptr->parent->right = right_child;

    right_child->left = ptr;
    ptr->parent = right_child;
}

void PieceTable::rotateRight(Node*& ptr)
{
    Node* left_child = ptr->left;
    ptr->left = left_child->right;

    if (ptr->left != nullptr)
        ptr->left->parent = ptr;

    left_child->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = left_child;
    else if (ptr == ptr->parent->left)
        ptr->parent->left = left_child;
    else
        ptr->parent->right = left_child;

    left_child->right = ptr;
    ptr->parent = left_child;
}

void PieceTable::fixInsertRBTree(Node*& ptr)
{
    Node* parent = nullptr;
    Node* grandparent = nullptr;
    while (ptr != root && getColor(ptr) == RED && getColor(ptr->parent) == RED)
    {
        parent = ptr->parent;
        grandparent = parent->parent;
        if (parent == grandparent->left)
        {
            Node* uncle = grandparent->right;
            if (getColor(uncle) == RED)
            {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                ptr = grandparent;
            }
            else
            {
                if (ptr == parent->right)
                {
                    rotateLeft(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                rotateRight(grandparent);
                swap(parent->color, grandparent->color);
                ptr = parent;
            }
        }
        else
        {
            Node* uncle = grandparent->left;
            if (getColor(uncle) == RED)
            {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                ptr = grandparent;
            }
            else
            {
                if (ptr == parent->left)
                {
                    rotateRight(parent);
                    ptr = parent;
                    parent = ptr->parent;
                }
                rotateLeft(grandparent);
                swap(parent->color, grandparent->color);
                ptr = parent;
            }
        }
    }
    setColor(root, BLACK);
}

void PieceTable::fixDeleteRBTree(Node*& node)
{
    if (node == nullptr)
        return;

    if (node == root)
    {
        root = nullptr;
        return;
    }

    if (getColor(node) == RED || getColor(node->left) == RED || getColor(node->right) == RED)
    {
        Node* child = node->left != nullptr ? node->left : node->right;

        if (node == node->parent->left)
        {
            node->parent->left = child;
            if (child != nullptr)
                child->parent = node->parent;
            setColor(child, BLACK);
            delete (node);
        }
        else
        {
            node->parent->right = child;
            if (child != nullptr)
                child->parent = node->parent;
            setColor(child, BLACK);
            delete (node);
        }
    }
    else
    {
        Node* sibling = nullptr;
        Node* parent = nullptr;
        Node* ptr = node;
        setColor(ptr, DOUBLE_BLACK);
        while (ptr != root && getColor(ptr) == DOUBLE_BLACK)
        {
            parent = ptr->parent;
            if (ptr == parent->left)
            {
                sibling = parent->right;
                if (getColor(sibling) == RED)
                {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateLeft(parent);
                }
                else
                {
                    if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK)
                    {
                        setColor(sibling, RED);
                        if (getColor(parent) == RED)
                            setColor(parent, BLACK);
                        else
                            setColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    }
                    else
                    {
                        if (getColor(sibling->right) == BLACK)
                        {
                            setColor(sibling->left, BLACK);
                            setColor(sibling, RED);
                            rotateRight(sibling);
                            sibling = parent->right;
                        }
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->right, BLACK);
                        rotateLeft(parent);
                        break;
                    }
                }
            }
            else
            {
                sibling = parent->left;
                if (getColor(sibling) == RED)
                {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rotateRight(parent);
                }
                else
                {
                    if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK)
                    {
                        setColor(sibling, RED);
                        if (getColor(parent) == RED)
                            setColor(parent, BLACK);
                        else
                            setColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    }
                    else
                    {
                        if (getColor(sibling->left) == BLACK)
                        {
                            setColor(sibling->right, BLACK);
                            setColor(sibling, RED);
                            rotateLeft(sibling);
                            sibling = parent->left;
                        }
                        setColor(sibling, parent->color);
                        setColor(parent, BLACK);
                        setColor(sibling->left, BLACK);
                        rotateRight(parent);
                        break;
                    }
                }
            }
        }
        if (node == node->parent->left)
            node->parent->left = nullptr;
        else
            node->parent->right = nullptr;
        delete(node);
        setColor(root, BLACK);
    }
}

Node* PieceTable::deleteBST(Node*& root, int pos)
{
    return nullptr;
    // if (root == nullptr)
    //     return root;
    //
    // int root_pos = 0;
    // for (auto tmp = root; tmp != nullptr; tmp = tmp->left)
    // {
    //     root_pos += tmp->length;
    // }
    // if (pos < root_pos)
    //     return deleteBST(root->left, pos);
    //
    // if (pos > root_pos)
    //     return deleteBST(root->right, pos);
    //
    // if (root->left == nullptr || root->right == nullptr)
    //     return root;
    //
    // Node* temp = minValueNode(root->right);
    // root->data = temp->data;
    // return deleteBST(root->right, temp->data);
}

void PieceTable::deleteValue(int data)
{
    Node* node = deleteBST(root, data);
    fixDeleteRBTree(node);
}

Node* PieceTable::minValueNode(Node*& node)
{
    Node* ptr = node;

    while (ptr->left != nullptr)
        ptr = ptr->left;

    return ptr;
}

Node* PieceTable::maxValueNode(Node*& node)
{
    Node* ptr = node;

    while (ptr->right != nullptr)
        ptr = ptr->right;

    return ptr;
}

int PieceTable::getBlackHeight(Node* node)
{
    int blackheight = 0;
    while (node != nullptr)
    {
        if (getColor(node) == BLACK)
            blackheight++;
        node = node->left;
    }
    return blackheight;
}

void PieceTable::insert(int line, Command command) 
{
    // todo
    // What i want to do is 
    // check the position of the line in the nodes
    // insert the command in the text buffer
    // update the node
}
