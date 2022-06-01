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

template <typename Scalar, typename Payload>
struct RBNode;

/// Scalar must support `<` and `==` operator and be able to be default and copy constructed.
template <typename Scalar>
struct ClosedInterval {
    template <typename _Scalar, typename _Payload>
    friend class RBNode;

    Scalar first;
    Scalar second;

    ClosedInterval() {}
    ClosedInterval(Scalar first): first(first), second(first) {}
    ClosedInterval(Scalar first, Scalar second): first(first), second(second) {}
    ClosedInterval(const ClosedInterval<Scalar> &other) = default;
    ~ClosedInterval() = default;

    inline bool overlap_with(const ClosedInterval<Scalar> &other) {
        return !(second < other.first || other.second < first);
    }
};

template <typename Scalar>
inline bool operator==(const ClosedInterval<Scalar> &i1, const ClosedInterval<Scalar> &i2) {
    return i1.first == i2.first && i1.second == i2.second;
}

template <typename Scalar>
inline bool operator!=(const ClosedInterval<Scalar> &i1, const ClosedInterval<Scalar> &i2) {
    return i1.first != i2.first || i1.second != i2.second;
}

template <typename Scalar>
inline bool operator<(const ClosedInterval<Scalar> &i1, const ClosedInterval<Scalar> &i2) {
    if (i1.first < i2.first) {
        return true;
    }
    if (!(i1.first < i2.first)) {
        return false;
    }
    return i1.second < i2.second;
}

enum class RBColor { black, red };

/// Scalar must support `<` operator and be able to be default and copy constructed.
/// Payload must be able to be default constructed.
template <typename Scalar, typename Payload>
struct RBNode {
    ClosedInterval<Scalar> intvl;
    Scalar max;
    Payload data;
    RBNode *left;
    RBNode *right;
    RBNode *par;
    RBColor color;
    bool sentinel;

    /// fill with interval and payload data
    RBNode(ClosedInterval<Scalar> intvl, Payload data, RBColor color):
            intvl(intvl), max(intvl.second), data(data),
            left(nullptr), right(nullptr), par(nullptr), color(color),
            sentinel(false)
    {}
    /// leave payload data empty
    RBNode(ClosedInterval<Scalar> intvl, RBColor color):
            intvl(intvl), max(intvl.second), left(nullptr), right(nullptr),
            par(nullptr), color(color), sentinel(false)
    {}
    RBNode(const RBNode<Scalar, Payload> &) = default;
    ~RBNode() = default;

    static inline bool is_nil(const RBNode<Scalar, Payload> *node) { return node->sentinel; }
    static inline RBNode<Scalar, Payload> *make_nil() {
        return new RBNode;
    }

    inline int key() const { return intvl.first; }
private:
    RBNode():
            left(nullptr), right(nullptr), par(nullptr), color(RBColor::black),
            sentinel(true)
    {}
};

/// not thread-safe.
/// Scalar must support `<` operator and be able to be default and copy constructed.
/// Payload must be able to be default constructed.
template <typename Scalar, typename Payload>
class IntTree {
public:
    using TreeClosedInterval = ClosedInterval<Scalar>;
    using TreeNode = RBNode<Scalar, Payload>;

    IntTree(): NIL(TreeNode::make_nil()), root(NIL) {}
    IntTree(const IntTree<Scalar, Payload> &);
    IntTree(IntTree<Scalar, Payload> &&);
    ~IntTree();

    static inline bool is_nil(TreeNode *x) { return TreeNode::is_nil(x); }
    static inline TreeNode *make_nil() { return TreeNode::make_nil(); }
    IntTree &operator=(const IntTree<Scalar, Payload> &);
    TreeNode *clone() const;
    inline bool empty() const { return TreeNode::is_nil(root); }
    void clear();
    bool eq(const IntTree<Scalar, Payload> &) const;

    TreeNode *make_node(TreeClosedInterval) const;
    TreeNode *make_node(TreeClosedInterval, Payload) const;
    TreeNode *make_node(Scalar, Scalar) const;
    TreeNode *make_node(Scalar, Scalar, Payload) const;
    TreeNode *contains(const TreeClosedInterval &) const;
    inline TreeNode *minimum() const {
        return empty() ? nullptr : minimum(root);
    }
    TreeNode *minimum(TreeNode *) const;
    TreeNode *successor(TreeNode *) const;
    /// if arg is NIL the behavior is undefined
    void insert(TreeNode *);
    /// if arg is NIL the behavior is undefined
    void erase(TreeNode *);
    TreeNode *intsearch(const TreeClosedInterval &) const;
    std::vector<TreeNode *> intsearch_all(const TreeClosedInterval &) const;
private:
    void left_rotate(TreeNode *);
    void right_rotate(TreeNode *);
    void transplant(TreeNode *, TreeNode *);
    void update_max(TreeNode *);

    // NIL must be declared before root (see default constructor)
    /// an internal representation; should be exposed to outside as nullptr
    TreeNode *NIL;
public:
    /// should be private, but left public for convenience of testing
    TreeNode *root;
};

template <typename Scalar, typename Payload>
inline bool operator==(const IntTree<Scalar, Payload> &t1, const IntTree<Scalar, Payload> &t2) {
    return t1.eq(t2);
}

template <typename Scalar, typename Payload>
inline bool operator!=(const IntTree<Scalar, Payload> &t1, const IntTree<Scalar, Payload> &t2) {
    return !t1.eq(t2);
}

}


#endif /* _INTTREE_H_ */
