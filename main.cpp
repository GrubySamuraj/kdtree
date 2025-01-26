#include "kdtree.hpp"
#include <iostream>
#include <sstream>
#include <vector>

int main()
{
    size_t dimensions = 2;
    KdTree tree(dimensions);

    tree.input();

    std::cout << "Enter the target point (" << dimensions << " dimensions): ";
    std::string line;
    std::getline(std::cin, line);

    std::istringstream iss(line);
    std::vector<int> target;
    int value;

    while (iss >> value)
    {
        target.push_back(value);
    }

    if (target.size() != dimensions)
    {
        std::cerr << "Invalid input. Target point must have " << dimensions << " dimensions.\n";
        return 1;
    }

    std::vector<int> nearest = tree.findNearest(target);

    tree.printTree();

    std::cout << "Nearest point to {";
    for (size_t i = 0; i < target.size(); ++i)
    {
        std::cout << target[i];
        if (i < target.size() - 1)
            std::cout << ", ";
    }
    std::cout << "} is {";

    for (size_t i = 0; i < nearest.size(); ++i)
    {
        std::cout << nearest[i];
        if (i < nearest.size() - 1)
            std::cout << ", ";
    }
    std::cout << "}\n";

    return 0;
}
