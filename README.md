# inttree (Interval Tree)

## Introduction

This is a simple yet efficient Interval Tree library written in C++ 11.
The Interval Tree structure is based on Red Black Tree.
No recursion is used during development.
The library has been thoroughly unit tested via [Catch2](https://github.com/catchorg/Catch2).

This project is a sub-project of another bigger one, so I'm just tailoring it to my own need -- no data slot in tree, no C++ template, just plain-old int closed intervals -- but more features may be added in the future.

## How to build

First you need [CMake](https://cmake.org).
To build tests you need Catch2 (mentioned above).
When I built tests I encountered some problem with the [`brew`](https://brew.sh)-installed Catch2.
That's why I cloned it to `lib/Catch2` (not included in this repo) and there's the line `add_subdirectory(lib/Catch2)` in `CMakeLists.txt`.
If you don't have problem with Catch2, remove that line.
If you don't want to build tests, remove the last three lines of `CMakeLists.txt`.
Then, the usual routine,

```bash
mkdir build
cd build
cmake ..
make
```

## How to use

An example (exerpted from `demo.cpp`):

```cpp
// build a tree
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
```

See `inttree.h` for detail.

## Similar projects

- [intervaltree](https://github.com/ekg/intervaltree)
- [interval-tree](https://github.com/5cript/interval-tree)
- [intervaltree](https://github.com/chaimleib/intervaltree)
