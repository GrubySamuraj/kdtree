#ifndef KDTREE_HPP
#define KDTREE_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

class KdTree
{
public:
    KdTree(size_t dims);
    void create(const std::vector<std::vector<int>> &points);
    bool search(const std::vector<int> &point) const;
    void insert(const std::vector<int> &point);
    std::vector<int> findNearest(const std::vector<int> &point) const;
    void printTree() const;
    void input();

private:
    struct Node
    {
        std::vector<int> values;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node(const std::vector<int> &vals);
    };
    void insert(Node *node, const std::vector<int> &point, int depth);
    void findNearest(const Node *node, const std::vector<int> &target, int depth,
                     const Node *&bestNode, double &bestDistance) const;

    std::unique_ptr<Node> root;
    size_t dimensions;

    std::unique_ptr<Node> buildTree(std::vector<std::vector<int>> &points, int depth);
    bool searchTree(const Node *node, const std::vector<int> &point, int depth) const;
    void printTree(const Node *node, const std::string &prefix, bool isRight) const;
};

#endif // KDTREE_HPP
