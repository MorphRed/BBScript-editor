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
    buffers.emplace_back();
}

PieceTable::PieceTable() : buffers({})
{
    root = nullptr;
    buffers.emplace_back();
}

int PieceTable::getColor(Node*& node)
{
    if (node == nullptr)
        return BLACK;

    return node->color;
}

void PieceTable::setColor(Node*& node, const Color color)
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

void PieceTable::insertBST(Node*& target, const int pos, Node*& insert)
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
        if (target->index == insert->index && target->start + target->length == buffers[target->index].size() && pos == target->left_subtree_length + target->length)
        {
            target->length += insert->length;
            free(insert);
            insert = nullptr;
            return;
        }
        insertBST(target->right, pos - target->left_subtree_length - target->length, insert);
        target->right->parent = target;
    }
    else
    {
        split_buffer = new Node(target->index, target->start + pos, target->length + target->start - pos);
        target->length = pos;
        insertBST(target->right, 0, insert);
        target->right->parent = target;
    }
}

void PieceTable::insertValue(const int pos, const int start, const int length)
{
    auto node = new Node(APPEND, start, length);
    // printf("insert %d[%d, %d]\n", node->index, node->start, node->length);
    // printTree(root, 0);
    insertBST(root, pos, node);
    fixInsertRBTree(node);
    if (split_buffer != nullptr)
    {
        insertBST(root, pos + node->length, split_buffer);
        fixInsertRBTree(split_buffer);
        split_buffer = nullptr;
    }
    // printTree(root, 0);
}

void PieceTable::rotateLeft(Node*& node)
{
    // printf("left rotation %d, %d[%d, %d]\n", node->left_subtree_length, node->index, node->start, node->length);
    // printTree(root, 0);
    Node* right_child = node->right;
    node->right = right_child->left;

    right_child->parent = node->parent;

    if (node->parent == nullptr)
        root = right_child;
    else if (node == node->parent->left)
        node->parent->left = right_child;
    else
        node->parent->right = right_child;

    right_child->left = node;
    right_child->left_subtree_length = node->left_subtree_length + node->length;
    if (node->right != nullptr)
    {
        node->right->parent = node;
        right_child->left_subtree_length += node->right->length;
    }
    node->parent = right_child;
    // printTree(root, 0);
}

void PieceTable::rotateRight(Node*& node)
{
    // printf("right rotation %d, %d[%d, %d]\n", node->left_subtree_length, node->index, node->start, node->length);
    // printTree(root, 0);
    Node* left_child = node->left;
    node->left = left_child->right;

    if (node->left != nullptr)
        node->left->parent = node;

    left_child->parent = node->parent;

    if (node->parent == nullptr)
        root = left_child;
    else if (node == node->parent->left)
        node->parent->left = left_child;
    else
        node->parent->right = left_child;

    left_child->right = node;
    node->left_subtree_length -= left_child->left_subtree_length + left_child->length;
    node->parent = left_child;
    // printTree(root, 0);
}

void PieceTable::fixInsertRBTree(Node*& node)
{
    if (node == nullptr)
        return;
    
    Node* parent = nullptr;
    Node* grandparent = nullptr;
    while (node != root && getColor(node) == RED && getColor(node->parent) == RED)
    {
        parent = node->parent;
        grandparent = parent->parent;
        if (parent == grandparent->left)
        {
            if (Node* uncle = grandparent->right; getColor(uncle) == RED)
            {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                node = grandparent;
            }
            else
            {
                if (node == parent->right)
                {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }
                rotateRight(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
            }
        }
        else
        {
            if (Node* uncle = grandparent->left; getColor(uncle) == RED)
            {
                setColor(uncle, BLACK);
                setColor(parent, BLACK);
                setColor(grandparent, RED);
                node = grandparent;
            }
            else
            {
                if (node == parent->left)
                {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }
                rotateLeft(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
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

Node* PieceTable::deleteBST(Node*& node, int pos, int length)
{
    // TODO
    return nullptr;
    // if (node == nullptr)
    //     return node;
    //
    // int root_pos = 0;
    // for (auto tmp = node; tmp != nullptr; tmp = tmp->left)
    // {
    //     root_pos += tmp->length;
    // }
    // if (pos < root_pos)
    //     return deleteBST(node->left, pos);
    //
    // if (pos > root_pos)
    //     return deleteBST(node->right, pos);
    //
    // if (node->left == nullptr || node->right == nullptr)
    //     return node;
    //
    // Node* temp = minValueNode(node->right);
    // node->data = temp->data;
    // return deleteBST(node->right, temp->data);
}

void PieceTable::deleteValue(int pos, int length)
{
    // TODO
    return;
    Node* node = deleteBST(root, pos, length);
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
void PieceTable::appendToBuffer(Node* node, const std::vector<Command>& commands)
{
    for (const auto& command : commands)
        buffers.at(node->index).push_back(command);
    node->length += static_cast<int>(commands.size());
}

// Like vim insert: at the start of the given pos
void PieceTable::insert(const int pos, const std::vector<Command>& commands)
{
    insertValue(pos, static_cast<int>(buffers.at(APPEND).size()), static_cast<int>(commands.size()));
    for (const auto& command : commands)
        buffers.at(APPEND).emplace_back(command);
}

std::vector<Command*> PieceTable::getCommand(int pos, int length)
{
    std::vector<Command*> output;
    auto node = getNode(root, pos);
    pos += node->start;
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

int rec[200];

void printTree(const Node* curr, const int depth)
{
    if(curr==nullptr)return;
    printf("\t");
    for(int i=0;i<depth;i++)
        if(i==depth-1)
            printf("%s\u2014\u2014\u2014", rec[depth-1]?"\u0371":"\u221F");
        else
            printf("%s   ",rec[i]?"\u23B8":"  ");
    printf("%d, %d[%d, %d]\n", curr->left_subtree_length, curr->index, curr->start, curr->length);
    rec[depth]=1;
    printTree(curr->right,depth+1);
    rec[depth]=0;
    printTree(curr->left,depth+1);
}