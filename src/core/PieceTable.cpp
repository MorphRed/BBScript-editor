//
// Red Black Tree Implementation
//

#include "PieceTable.h"

#include <iostream>

Node::Node(const BufferIndex index, const int start, const int length)
{
    this->index = index;
    this->start = start;
    this->length = length;
    color = RED;
    left = right = parent = nullptr;
}

PieceTable::PieceTable(const std::vector<Command>& commands) : buffers({commands})
{
    root = new Node(ORIGINAL, 0, static_cast<int>(commands.size()));
    root->color = BLACK;
    buffers.push_back({});
}

PieceTable::PieceTable() : buffers({})
{
    root = nullptr;
    buffers.push_back({});
}

int PieceTable::getColor(Node*& node)
{
    if (node == nullptr)
        return BLACK;

    return node->color;
}

void PieceTable::setColor(Node*& node, const int color)
{
    if (node == nullptr)
        return;

    node->color = color;
}

Node* PieceTable::getNode(Node* target, int& pos)
{
    while (true)
    {
        if (pos < target->left_subtree_length)
        {
            target = target->left;
        }
        else if (pos >= target->left_subtree_length + target->length)
        {
            pos -= target->left_subtree_length;
            target = target->right;
        }
        else
            return target;
    }
}

Node* PieceTable::getNextNode(Node* target)
{
    if (target->right != nullptr)
    {
        target = target->right;
        while (target->left != nullptr)
        {
            target = target->left;
        }
        return target;
    }
    if (target->parent == nullptr)
        return nullptr;
    if (target->parent->right == target)
    {
        while (true)
        {
            if (target->parent == nullptr)
                return nullptr;
            if (target->parent->left == target)
                return target->parent;
            target = target->parent;
        }
    }
    return target->parent;
}

void PieceTable::insertBST(Node*& target, const int pos, Node* insert)
{
    if (target == nullptr)
    {
        target = insert;
        return;
    }

    if (pos <= target->left_subtree_length)
    {
        target->left_subtree_length += insert->length;
        insertBST(target->left, pos, insert);
        target->left->parent = target;
    }
    else if (pos >= target->left_subtree_length + target->length)
    {
        insertBST(target->right, pos - target->left_subtree_length, insert);
        target->right->parent = target;
    }
    else
    {
        split_buffer = new Node(target->index, pos, target->length - pos);
        target->length = pos - target->start;
        insertBST(target->right, 0, insert);
        target->right->parent = target;
    }
}

void PieceTable::insertValue(const int pos, const int start, const int length)
{
    auto node = new Node(APPEND, start, length);
    insertBST(root, pos, node);
    // TODO fix some sort of bug about left subtree length
    fixInsertRBTree(node);
    if (split_buffer != nullptr)
    {
        insertBST(root, pos + node->length, split_buffer);
        fixInsertRBTree(split_buffer);
        split_buffer = nullptr;
    }
}

void PieceTable::updateParentSubtreeLen(const Node* ptr, const int length)
{
    const auto parent = ptr->parent;
    if (parent == nullptr)
        return;
    if (parent->left == ptr)
        parent->left_subtree_length += length;
    updateParentSubtreeLen(parent, length);
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
    right_child->left_subtree_length = ptr->left_subtree_length + ptr->length;
    updateParentSubtreeLen(right_child, right_child->length);
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
    ptr->left_subtree_length = left_child->left_subtree_length + left_child->length;
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
            if (Node* uncle = grandparent->right; getColor(uncle) == RED)
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
                std::swap(parent->color, grandparent->color);
                ptr = parent;
            }
        }
        else
        {
            if (Node* uncle = grandparent->left; getColor(uncle) == RED)
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
                std::swap(parent->color, grandparent->color);
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
    // TODO
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
    // TODO
    return;
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
    int black_height = 0;
    while (node != nullptr)
    {
        if (getColor(node) == BLACK)
            black_height++;
        node = node->left;
    }
    return black_height;
}

// I haven't searched how to check for the end of the buffer so this function is never reached
void PieceTable::appendToBuffer(Node* ptr, const std::vector<Command>& commands)
{
    for (const auto& command : commands)
        buffers.at(ptr->index).push_back(command);
    ptr->length += static_cast<int>(commands.size());
}

// Like vim insert, at the start of the given pos
void PieceTable::insert(const int pos, const std::vector<Command>& commands)
{
    insertValue(pos, buffers.at(APPEND).size(), static_cast<int>(commands.size()));
    for (const auto& command : commands)
        buffers.at(APPEND).emplace_back(command);
}

std::vector<Command*> PieceTable::getCommand(int pos, int length)
{
    std::vector<Command*> output;
    auto node = getNode(root, pos);
    while (length > 0)
    {
        const int node_length = (node->length < length) ? node->length : length;
        for (const int node_end = node_length + pos; pos < node_end; pos++)
        {
            output.emplace_back(&buffers.at(node->index).at(pos));
        }
        length -= node_length;
        if (length > 0)
        {
            node = getNextNode(node);
            pos = node->start;
        }
    }
    return output;
}

void PieceTable::debug()
{
    Node* node = root;
    while (node->left != nullptr)
        node = node->left;
    do
    {
        std::cout << node->left_subtree_length << std::endl;
        node = getNextNode(node);
    } while (node != nullptr);
}