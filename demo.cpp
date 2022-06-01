//
//  demo.cpp
//  inttree
//
//  Created by Kaiwen on 5/31/22.
//

#include "inttree.hpp"
#include <iostream>
#include <vector>

template class inttree::ClosedInterval<int>;
template class inttree::RBNode<int, int>;
template class inttree::IntTree<int, int>;

int main() {
    inttree::IntTree<int, int> tree;
    tree.insert(tree.make_node(17, 19));
    tree.insert(tree.make_node(8, 9));
    tree.insert(tree.make_node(25, 30));

    // traverse in in-order and print
    std::cout << "Traverse:\n";
    if (!tree.empty()) {
        auto *x = tree.minimum();
        do {
            std::cout << "[" << x->intvl.first << ", " << x->intvl.second << "]\n";
        } while ((x = tree.successor(x)));
    }

    // find overlapping intervals
    auto i = inttree::ClosedInterval<int>(18, 26);
    // find any one of the overlapping interval
    auto *found = tree.intsearch(i);
    std::cout << "found one:\n";
    std::cout << "[" << found->intvl.first << ", " << found->intvl.second << "]\n";
    // find all overlapping interval
    auto all_found = tree.intsearch_all(i);
    std::cout << "found all:\n";
    for (auto it = all_found.begin(); it != all_found.end(); ++it) {
        std::cout << "[" << (*it)->intvl.first << ", "
                  << (*it)->intvl.second << "]\n";
    }

    // erase `found`
    tree.erase(found);

    // clear tree
    tree.clear();

    return 0;
}
