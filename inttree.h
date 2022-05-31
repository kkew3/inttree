//
//  inttree.h
//  inttree
//
//  Created by Kaiwen on 5/29/22.
//

#ifndef _INTTREE_H_
#define _INTTREE_H_

#include <utility>
#include <vector>

namespace inttree {

using ClosedInterval = std::pair<int, int>;

inline ClosedInterval make_interval(int a, int b) {
    return std::make_pair(a, b);
}

inline bool closed_interval_overlap(const ClosedInterval &i1,
                                    const ClosedInterval &i2) {
    return i1.second >= i2.first && i2.second >= i1.first;
}


enum class RBColor { black, red };

struct RBNode {
    ClosedInterval intvl;
    int max;
    RBNode *left;
    RBNode *right;
    RBNode *par;
    RBColor color;

    RBNode(ClosedInterval intvl, RBColor color):
            intvl(intvl), max(intvl.second), left(nullptr), right(nullptr),
            par(nullptr), color(color)
    {}

    RBNode(const RBNode &) = default;

    inline int key() const { return intvl.first; }
};

/// not thread-safe
class IntTree {
    friend inline bool operator==(const IntTree &t1, const IntTree &t2) {
        return t1.eq(t2);
    }
    friend inline bool operator!=(const IntTree &t1, const IntTree &t2) {
        return !t1.eq(t2);
    }
public:
    IntTree(): NIL(nullptr), root(nullptr) {
        NIL = make_nil();
        root = NIL;
    }
    IntTree(const IntTree &);
    IntTree(IntTree &&);
    ~IntTree();
    static inline RBNode *make_nil() {
        return new RBNode(make_interval(1, 0), RBColor::black);
    }
    // Being static and not relying on the identity of NIL is important for
    // the implementation of operator= method
    static inline bool is_nil(RBNode *z) {
        return z->intvl.first > z->intvl.second;
    }
    IntTree &operator=(const IntTree &);
    RBNode *clone() const;
    inline bool empty() const { return is_nil(root); }
    void clear();
    bool eq(const IntTree &) const;

    RBNode *make_node(ClosedInterval) const;
    RBNode *contains(const ClosedInterval &) const;
    inline RBNode *minimum() const {
        return empty() ? nullptr : minimum(root);
    }
    /// if arg is NIL the behavior is undefined
    RBNode *minimum(RBNode *) const;
    /// if arg is NIL the behavior is undefined
    RBNode *successor(RBNode *) const;
    /// if arg is NIL the behavior is undefined
    void insert(RBNode *);
    /// if arg is NIL the behavior is undefined
    void erase(RBNode *);
    RBNode *intsearch(const ClosedInterval &) const;
    std::vector<RBNode *> intsearch_all(const ClosedInterval &) const;
private:
    void left_rotate(RBNode *);
    void right_rotate(RBNode *);
    void transplant(RBNode *, RBNode *);
    void update_max(RBNode *);

    /// an internal representation; should be exposed to outside as nullptr
    RBNode *NIL;
public:
    /// should be private, but left public for convenience of testing
    RBNode *root;
};

}


#endif /* _INTTREE_H_ */
