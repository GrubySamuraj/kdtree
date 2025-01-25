#include "kdtree.hpp"

KdTree::Node::Node(const std::vector<int> &vals) : values(vals), left(nullptr), right(nullptr) {}

KdTree::KdTree(size_t dims) : root(nullptr), dimensions(dims) {}

void KdTree::create(const std::vector<std::vector<int>> &points)
{
    if (points.empty())
        return;

    for (const auto &point : points)
    {
        if (point.size() != dimensions)
        {
            throw std::invalid_argument("All points must have the same number of dimensions.");
        }
    }

    std::vector<std::vector<int>> pointsCopy = points;
    root = buildTree(pointsCopy, 0);
}

std::unique_ptr<KdTree::Node> KdTree::buildTree(std::vector<std::vector<int>> &points, int depth)
{
    if (points.empty())
        return nullptr;

    int axis = depth % dimensions;

    std::sort(points.begin(), points.end(), [axis](const std::vector<int> &a, const std::vector<int> &b)
              { return a[axis] < b[axis]; });

    size_t medianIndex = points.size() / 2;
    auto medianPoint = points[medianIndex];

    auto node = std::make_unique<Node>(medianPoint);

    std::vector<std::vector<int>> leftPoints(points.begin(), points.begin() + medianIndex);
    std::vector<std::vector<int>> rightPoints(points.begin() + medianIndex + 1, points.end());

    node->left = buildTree(leftPoints, depth + 1);
    node->right = buildTree(rightPoints, depth + 1);

    return node;
}

bool KdTree::search(const std::vector<int> &point) const
{
    if (point.size() != dimensions)
    {
        throw std::invalid_argument("Point dimensionality must match the tree dimensionality.");
    }

    return searchTree(root.get(), point, 0);
}

bool KdTree::searchTree(const Node *node, const std::vector<int> &point, int depth) const
{
    if (!node)
        return false;

    if (node->values == point)
        return true;

    int axis = depth % dimensions;

    if (point[axis] < node->values[axis])
    {
        return searchTree(node->left.get(), point, depth + 1);
    }
    else
    {
        return searchTree(node->right.get(), point, depth + 1);
    }
}

void KdTree::printTree() const
{
    if (!root)
    {
        std::cout << "Tree is empty.\n";
        return;
    }
    printTree(root.get(), "", true);
}

void KdTree::printTree(const Node *node, const std::string &prefix, bool isRight) const
{
    if (!node)
        return;

    std::cout << prefix;

    if (isRight)
    {
        std::cout << "└── ";
    }
    else
    {
        std::cout << "├── ";
    }

    std::cout << "[";
    for (size_t i = 0; i < node->values.size(); ++i)
    {
        std::cout << node->values[i];
        if (i < node->values.size() - 1)
            std::cout << ", ";
    }
    std::cout << "]\n";

    printTree(node->left.get(), prefix + (isRight ? "    " : "│   "), false);
    printTree(node->right.get(), prefix + (isRight ? "    " : "│   "), true);
}

void KdTree::insert(const std::vector<int> &point)
{
    if (point.size() != dimensions)
    {
        throw std::invalid_argument("Point dimensionality must match the tree dimensionality.");
    }

    if (!root)
    {
        root = std::make_unique<Node>(point);
        return;
    }

    insert(root.get(), point, 0);
}

void KdTree::insert(Node *node, const std::vector<int> &point, int depth)
{
    int axis = depth % dimensions;

    if (point[axis] < node->values[axis])
    {
        if (node->left)
        {
            insert(node->left.get(), point, depth + 1);
        }
        else
        {
            node->left = std::make_unique<Node>(point);
        }
    }
    else
    {
        if (node->right)
        {
            insert(node->right.get(), point, depth + 1);
        }
        else
        {
            node->right = std::make_unique<Node>(point);
        }
    }
}
std::vector<int> KdTree::findNearest(const std::vector<int> &point) const
{
    if (point.size() != dimensions)
    {
        throw std::invalid_argument("Point dimensionality must match the tree dimensionality.");
    }

    const Node *bestNode = nullptr;
    double bestDistance = std::numeric_limits<double>::max();

    findNearest(root.get(), point, 0, bestNode, bestDistance);

    if (bestNode)
        return bestNode->values;

    return {};
}

void KdTree::findNearest(const Node *node, const std::vector<int> &target, int depth,
                         const Node *&bestNode, double &bestDistance) const
{
    if (!node)
        return;

    double distance = 0.0;
    for (size_t i = 0; i < dimensions; ++i)
    {
        distance += std::pow(node->values[i] - target[i], 2);
    }
    distance = std::sqrt(distance);

    if (distance < bestDistance)
    {
        bestDistance = distance;
        bestNode = node;
    }

    int axis = depth % dimensions;
    Node *first = nullptr;
    Node *second = nullptr;

    if (target[axis] < node->values[axis])
    {
        first = node->left.get();
        second = node->right.get();
    }
    else
    {
        first = node->right.get();
        second = node->left.get();
    }

    findNearest(first, target, depth + 1, bestNode, bestDistance);

    if (std::abs(target[axis] - node->values[axis]) < bestDistance)
    {
        findNearest(second, target, depth + 1, bestNode, bestDistance);
    }
}