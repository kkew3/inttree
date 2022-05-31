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


enum class rbcolor { black, red };

struct rbnode {
    ClosedInterval intvl;
    int max;
    rbnode *left;
    rbnode *right;
    rbnode *par;
    rbcolor color;

    rbnode(ClosedInterval intvl, rbcolor color):
            intvl(intvl), max(intvl.second), left(nullptr), right(nullptr),
            par(nullptr), color(color)
    {}

    rbnode(const rbnode &) = default;

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
    IntTree(): root(nullptr), NIL(nullptr) {
        NIL = make_nil();
        root = NIL;
    }
    IntTree(const IntTree &);
    IntTree(IntTree &&);
    ~IntTree();
    static inline rbnode *make_nil() {
        return new rbnode(make_interval(1, 0), rbcolor::black);
    }
    // Being static and not relying on the identity of NIL is important for
    // the implementation of operator= method
    static inline bool is_nil(rbnode *z) {
        return z->intvl.first > z->intvl.second;
    }
    IntTree &operator=(const IntTree &);
    rbnode *clone() const;
    inline bool empty() const { return is_nil(root); }
    void clear();
    bool eq(const IntTree &) const;

    rbnode *make_node(ClosedInterval) const;
    rbnode *contains(const ClosedInterval &) const;
    inline rbnode *minimum() const {
        return empty() ? nullptr : minimum(root);
    }
    /// if arg is NIL the behavior is undefined
    rbnode *minimum(rbnode *) const;
    /// if arg is NIL the behavior is undefined
    rbnode *successor(rbnode *) const;
    /// if arg is NIL the behavior is undefined
    void insert(rbnode *);
    /// if arg is NIL the behavior is undefined
    void erase(rbnode *);
    rbnode *intsearch(const ClosedInterval &) const;
    std::vector<rbnode *> intsearch_all(const ClosedInterval &) const;
private:
    void left_rotate(rbnode *);
    void right_rotate(rbnode *);
    void transplant(rbnode *, rbnode *);
    void update_max(rbnode *);

    /// an internal representation; should be exposed to outside as nullptr
    rbnode *NIL;
public:
    /// should be private, but left public for convenience of testing
    rbnode *root;
};

}


#endif /* _INTTREE_H_ */
