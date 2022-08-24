#pragma once

#include <stdio.h>
#include "flat.h"
enum Axis
{
    AXIS_X,
    AXIS_Y,
    AXIS_Z
};

template <class T>
struct BspTreeNode
{
    int depth = 0;
    Axis axis;
    T splitter;
    std::vector<Flat<T>> flats;
    Point<T> min_point;
    Point<T> max_point;
    BspTreeNode* l_child = nullptr;
    BspTreeNode* r_child = nullptr;
    BspTreeNode() {}
};

template <class T>
class BspTree
{
public:
    BspTree();
    ~BspTree();

    void InitBspTree(std::vector<Flat<T>>& flats, Point<T> min_point, Point<T> max_point, int max_depth);
    void DeleteBspTree();
    void TraversBspTree() const;

private:
    void cutFlat(const Flat<T>& flat, Axis axis, const T& splitter, int& left_count, int& right_count, int& both_count);
    void splitSpace(BspTreeNode<T>* node, Axis axis, int depth);
    void traverse(BspTreeNode<T>* node) const;
    void deleteTree(BspTreeNode<T>* root);

private:
    BspTreeNode<T>* m_root;
    int m_max_depth;
};

template <class T>
BspTree<T>::BspTree() : m_root(nullptr), m_max_depth(5)
{

}

template <class T>
BspTree<T>::~BspTree()
{

}

template <class T>
void BspTree<T>::InitBspTree(std::vector<Flat<T>>& flats, Point<T> min_point, Point<T> max_point, int max_depth)
{

    m_max_depth = max_depth;
    m_root = new BspTreeNode<T>;
    m_root->depth = 1;
    m_root->flats = flats;
    m_root->max_point = max_point;
    m_root->min_point = min_point;
    splitSpace(m_root, AXIS_X, 1);
}

template <class T>
void BspTree<T>::deleteTree(BspTreeNode<T>* root)
{
    if (root)
    {
        deleteTree(root->l_child);
        deleteTree(root->r_child);
        delete root;
        root = nullptr;
    }
}

template <class T>
void BspTree<T>::cutFlat(const Flat<T>& flat, Axis axis, const T& splitter, int& left_count, int& right_count, int& both_count)
{
    T p[FLAT_POINT] = { 0 };
    switch (axis)
    {
    case AXIS_X:
        p[AXIS_X] = flat.point[AXIS_X]._x;
        p[AXIS_Y] = flat.point[AXIS_Y]._x;
        p[AXIS_Z] = flat.point[AXIS_Z]._x;
        break;
    case AXIS_Y:
        p[AXIS_X] = flat.point[AXIS_X]._y;
        p[AXIS_Y] = flat.point[AXIS_Y]._y;
        p[AXIS_Z] = flat.point[AXIS_Z]._y;
        break;
    case AXIS_Z:
        p[AXIS_X] = flat.point[AXIS_X]._z;
        p[AXIS_Y] = flat.point[AXIS_Y]._z;
        p[AXIS_Z] = flat.point[AXIS_Z]._z;
        break;
    default:
        printf("axis type error! axis = %d", axis);
        break;
    }

    for (int i = 0; i < FLAT_POINT; i++)
    {
        if (p[i] < splitter)
            left_count++;
        else if (p[i] > splitter)
            right_count++;
        else if (fabs(p[i] - splitter) < 1e-6)
            both_count++;
    }
}

template <class T>
void BspTree<T>::splitSpace(BspTreeNode<T>* node, Axis axis, int depth)
{
    if (!node) return;
    node->axis = axis;
    node->depth = depth;
    if (depth == m_max_depth) return;
    node->l_child = new BspTreeNode<T>;
    node->r_child = new BspTreeNode<T>;

    //先给子节点的包围盒赋值
    node->l_child->max_point = node->max_point;
    node->l_child->min_point = node->min_point;
    node->r_child->max_point = node->max_point;
    node->r_child->min_point = node->min_point;
    T x_len = node->max_point._x - node->min_point._x;
    T y_len = node->max_point._y - node->min_point._y;
    T z_len = node->max_point._z - node->min_point._z;
    //设置节点的划分点
    Axis m_axis = AXIS_X;
    if (y_len > x_len && y_len > z_len)
        m_axis = AXIS_Y;
    if (z_len > x_len && z_len > y_len)
        m_axis = AXIS_Z;

    switch (m_axis)
    {
    case AXIS_X:
        node->splitter = (node->max_point._x + node->min_point._x) / 2;
        node->l_child->max_point._x = node->splitter;
        node->r_child->min_point._x = node->splitter;
        break;
    case AXIS_Y:
        node->splitter = (node->max_point._y + node->min_point._y) / 2;
        node->l_child->max_point._y = node->splitter;
        node->r_child->min_point._y = node->splitter;
        break;
    case AXIS_Z:
        node->splitter = (node->max_point._z + node->min_point._z) / 2;
        node->l_child->max_point._z = node->splitter;
        node->r_child->min_point._z = node->splitter;
        break;
    default:
        printf("m_axis type error! m_axis = %d", m_axis);
        break;
    }

    for (int i = 0; i < node->flats.size(); i++)
    {
        int left_count = 0;
        int right_count = 0;
        int both_count = 0;
        cutFlat(node->flats[i], m_axis, node->splitter, left_count, right_count, both_count);
        if (left_count || both_count)
            node->l_child->flats.push_back(node->flats[i]);
        if (right_count || both_count)
            node->r_child->flats.push_back(node->flats[i]);
    }

    node->flats.clear();
    splitSpace(node->l_child, m_axis, depth + 1);
    splitSpace(node->r_child, m_axis, depth + 1);
}

template <class T>
void BspTree<T>::traverse(BspTreeNode<T>* node) const
{
    if (!node)
        return;
    traverse(node->l_child);
    //中序遍历
    printf("nodes count = %d, flats count = %zd", node->depth, node->flats.size());
    switch (node->axis)
    {
    case AXIS_X:
        std::cout << "axis x = " << node->splitter << std::endl;
        break;
    case AXIS_Y:
        std::cout << "axis y = " << node->splitter << std::endl;
        break;
    case AXIS_Z:
        std::cout << "axis z = " << node->splitter << std::endl;
        break;
    default:
        printf("node axis type error! axis = %d", node->axis);
        break;
    }
    traverse(node->r_child);
}

template <class T>
void BspTree<T>::TraversBspTree() const
{
    traverse(m_root);
}

template <class T>
void BspTree<T>::DeleteBspTree()
{
    deleteTree(m_root);
}