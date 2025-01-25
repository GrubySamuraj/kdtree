#include "kdtree.hpp"
#include <iostream>

int main()
{
    KdTree tree(2);

    std::vector<std::vector<int>> points = {
        {2, 3},
        {5, 4},
        {9, 6},
        {4, 7},
        {8, 1},
        {7, 2}};

    tree.create(points);

    std::vector<int> target = {6, 5};
    std::vector<int> nearest = tree.findNearest(target);

    std::cout << "Nearest point to {6, 5}: {";
    for (size_t i = 0; i < nearest.size(); ++i)
    {
        std::cout << nearest[i];
        if (i < nearest.size() - 1)
            std::cout << ", ";
    }
    std::cout << "}" << std::endl;

    return 0;
}
