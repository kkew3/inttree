//
//  demo.cpp
//  inttree
//
//  Created by Kaiwen on 5/31/22.
//

#include "inttree.h"
#include <iostream>
#include <vector>

int main() {
    inttree::IntTree tree;
    tree.insert(tree.make_node(inttree::make_interval(17, 19)));
    tree.insert(tree.make_node(inttree::make_interval(8, 9)));
    tree.insert(tree.make_node(inttree::make_interval(25, 30)));

    // traverse in in-order and print
    std::cout << "Traverse:\n";
    if (!tree.empty()) {
        inttree::rbnode *x = tree.minimum();
        do {
            std::cout << "[" << x->intvl.first << ", " << x->intvl.second << "]\n";
        } while ((x = tree.successor(x)));
    }

    // find overlapping intervals
    inttree::ClosedInterval i = inttree::make_interval(18, 26);
    // find any one of the overlapping interval
    inttree::rbnode *found = tree.intsearch(i);
    std::cout << "found one:\n";
    std::cout << "[" << found->intvl.first << ", " << found->intvl.second << "]\n";
    // find all overlapping interval
    std::vector<inttree::rbnode *> all_found = tree.intsearch_all(i);
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
