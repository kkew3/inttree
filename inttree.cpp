//
//  inttree.cpp
//  inttree
//
//  Created by Kaiwen on 5/29/22.
//

#include "inttree.h"

namespace inttree {

template <typename Scalar, typename Payload>
IntTree<Scalar, Payload>::~IntTree() {
    clear();
    root = nullptr;
    delete NIL;
    NIL = nullptr;
}

template <typename Scalar, typename Payload>
IntTree<Scalar, Payload>::IntTree(const IntTree<Scalar, Payload> &other) {
    root = other.clone();
    if (RBNode<Scalar, Payload>::is_nil(root)) {
        NIL = root;
    } else {
        NIL = root->par;
    }
}

template <typename Scalar, typename Payload>
IntTree<Scalar, Payload>::IntTree(IntTree<Scalar, Payload> &&other) {
    if (RBNode<Scalar, Payload>::is_nil(other.root)) {
        // nothing to move
        NIL = IntTree<Scalar, Payload>::TreeNode::make_nil();
        root = NIL;
    } else {
        root = other.root;
        NIL = root->par;
        other.NIL = RBNode<Scalar, Payload>::make_nil();
        other.root = other.NIL;
    }
}

template <typename Scalar, typename Payload>
IntTree<Scalar, Payload> &IntTree<Scalar, Payload>::operator=(const IntTree<Scalar, Payload> &other) {
    RBNode<Scalar, Payload> *new_root = other.clone();
    RBNode<Scalar, Payload> *new_nil;
    if (RBNode<Scalar, Payload>::is_nil(new_root)) {
        new_nil = new_root;
    } else {
        new_nil = new_root->par;
    }
    clear();
    delete NIL;
    root = new_root;
    NIL = new_nil;
    return *this;
}

template <typename Scalar, typename Payload>
RBNode<Scalar, Payload> *IntTree<Scalar, Payload>::clone() const {
    RBNode<Scalar, Payload> *nil_copied = RBNode<Scalar, Payload>::make_nil();
    if (RBNode<Scalar, Payload>::is_nil(root)) {
        return nil_copied;
    }

    RBNode<Scalar, Payload> *root_copied = new RBNode<Scalar, Payload>(root->intvl, root->color);
    root_copied->max = root->max;
    root_copied->par = nil_copied;
    root_copied->left = nil_copied;
    root_copied->right = nil_copied;
    std::vector<RBNode<Scalar, Payload> *> stack { root };
    std::vector<RBNode<Scalar, Payload> *> stack_copied { root_copied };
    while (!stack.empty()) {
        RBNode<Scalar, Payload> *curr = stack.back();
        RBNode<Scalar, Payload> *curr_copied = stack_copied.back();
        stack.pop_back();
        stack_copied.pop_back();

        if (!RBNode<Scalar, Payload>::is_nil(curr->right)) {
            stack.push_back(curr->right);
            curr_copied->right = new RBNode<Scalar, Payload>(curr->right->intvl, curr->right->color);
            curr_copied->right->max = curr->right->max;
            curr_copied->right->par = curr_copied;
            curr_copied->right->left = nil_copied;
            curr_copied->right->right = nil_copied;
            stack_copied.push_back(curr_copied->right);
        }

        if (!RBNode<Scalar, Payload>::is_nil(curr->left)) {
            stack.push_back(curr->left);
            curr_copied->left = new RBNode<Scalar, Payload>(curr->left->intvl, curr->left->color);
            curr_copied->left->max = curr->left->max;
            curr_copied->left->par = curr_copied;
            curr_copied->left->left = nil_copied;
            curr_copied->left->right = nil_copied;
            stack_copied.push_back(curr_copied->left);
        }
    }
    return root_copied;
}

template <typename Scalar, typename Payload>
void IntTree<Scalar, Payload>::clear() {
    std::vector<RBNode<Scalar, Payload> *> stack;
    RBNode<Scalar, Payload> *prev = nullptr;
    while (!RBNode<Scalar, Payload>::is_nil(root) || !stack.empty()) {
        if (!RBNode<Scalar, Payload>::is_nil(root)) {
            stack.push_back(root);
            root = root->left;
        } else {
            root = stack.back();
            if (RBNode<Scalar, Payload>::is_nil(root->right)
                || root->right == prev) {
                prev = root;
                delete root;
                root = NIL;
                stack.pop_back();
            } else {
                root = root->right;
            }
        }
    }
    root = NIL;
}

template <typename Scalar, typename Payload>
bool IntTree<Scalar, Payload>::eq(const IntTree<Scalar, Payload> &other) const {
    if (RBNode<Scalar, Payload>::is_nil(root)
        && RBNode<Scalar, Payload>::is_nil(other.root)) {
        return true;
    }
    if (RBNode<Scalar, Payload>::is_nil(root)
        || RBNode<Scalar, Payload>::is_nil(other.root)) {
        return false;
    }

    std::vector<RBNode<Scalar, Payload> *> stack { root };
    std::vector<RBNode<Scalar, Payload> *> stack_other { other.root };
    while (!stack.empty()) {
        RBNode<Scalar, Payload> *curr = stack.back();
        RBNode<Scalar, Payload> *curr_other = stack_other.back();
        stack.pop_back();
        stack_other.pop_back();

        if (curr->intvl != curr_other->intvl || curr->max != curr_other->max) {
            return false;
        }

        if (!RBNode<Scalar, Payload>::is_nil(curr->right)
            && !RBNode<Scalar, Payload>::is_nil(curr_other->right)) {
            stack.push_back(curr->right);
            stack_other.push_back(curr_other->right);
        } else if (!RBNode<Scalar, Payload>::is_nil(curr->right)
                   || !RBNode<Scalar, Payload>::is_nil(curr_other->right)) {
            return false;
        }

        if (!RBNode<Scalar, Payload>::is_nil(curr->left)
            && !RBNode<Scalar, Payload>::is_nil(curr_other->left)) {
            stack.push_back(curr->left);
            stack_other.push_back(curr_other->left);
        } else if (!RBNode<Scalar, Payload>::is_nil(curr->left)
                   || !RBNode<Scalar, Payload>::is_nil(curr_other->left)) {
            return false;
        }
    }
    return true;
}

template <typename Scalar, typename Payload>
void IntTree<Scalar, Payload>::left_rotate(RBNode<Scalar, Payload> *x) {
    RBNode<Scalar, Payload> *y = x->right;
    x->right = y->left;
    if (!RBNode<Scalar, Payload>::is_nil(y->left)) {
        y->left->par = x;
    }
    y->par = x->par;
    if (RBNode<Scalar, Payload>::is_nil(x->par)) {
        root = y;
    } else if (x == x->par->left) {
        x->par->left = y;
    } else {
        x->par->right = y;
    }
    y->left = x;
    x->par = y;

    y->max = x->max;
    update_max(x);
}

template <typename Scalar, typename Payload>
void IntTree<Scalar, Payload>::right_rotate(RBNode<Scalar, Payload> *y) {
    RBNode<Scalar, Payload> *x = y->left;
    y->left = x->right;
    if (!RBNode<Scalar, Payload>::is_nil(x->right)) {
        x->right->par = y;
    }
    x->par = y->par;
    if (RBNode<Scalar, Payload>::is_nil(y->par)) {
        root = x;
    } else if (y == y->par->left) {
        y->par->left = x;
    } else {
        y->par->right = x;
    }
    x->right = y;
    y->par = x;

    x->max = y->max;
    update_max(y);
}

template <typename Scalar, typename Payload>
void IntTree<Scalar, Payload>::transplant(RBNode<Scalar, Payload> *u, RBNode<Scalar, Payload> *v) {
    if (RBNode<Scalar, Payload>::is_nil(u->par)) {
        root = v;
    } else if (u == u->par->left) {
        u->par->left = v;
    } else {
        u->par->right = v;
    }
    v->par = u->par;
}

template <typename Scalar, typename Payload>
void IntTree<Scalar, Payload>::update_max(RBNode<Scalar, Payload> *z) {
    if (RBNode<Scalar, Payload>::is_nil(z->left)
        && RBNode<Scalar, Payload>::is_nil(z->right)) {
        z->max = z->intvl.second;
    } else if (RBNode<Scalar, Payload>::is_nil(z->left)) {
        z->max = std::max(z->intvl.second, z->right->max);
    } else if (RBNode<Scalar, Payload>::is_nil(z->right)) {
        z->max = std::max(z->intvl.second, z->left->max);
    } else {
		z->max = std::max(std::max(z->intvl.second, z->left->max), z->right->max);
    }
}

template <typename Scalar, typename Payload>
RBNode<Scalar, Payload> *IntTree<Scalar, Payload>::make_node(ClosedInterval<Scalar> i) const {
    RBNode<Scalar, Payload> *z = new RBNode<Scalar, Payload>(i, RBColor::red);
    z->left = NIL;
    z->right = NIL;
    z->par = NIL;
    return z;
}

template <typename Scalar, typename Payload>
RBNode<Scalar, Payload> *IntTree<Scalar, Payload>::make_node(ClosedInterval<Scalar> i, Payload data) const {
    RBNode<Scalar, Payload> *z = new RBNode<Scalar, Payload>(i, data, RBColor::red);
    z->left = NIL;
    z->right = NIL;
    z->par = NIL;
    return z;
}

template <typename Scalar, typename Payload>
RBNode<Scalar, Payload> *IntTree<Scalar, Payload>::make_node(Scalar first, Scalar second) const {
    RBNode<Scalar, Payload> *z = new RBNode<Scalar, Payload>(ClosedInterval<Scalar>(first, second), RBColor::red);
    z->left = NIL;
    z->right = NIL;
    z->par = NIL;
    return z;
}

template <typename Scalar, typename Payload>
RBNode<Scalar, Payload> *IntTree<Scalar, Payload>::make_node(Scalar first, Scalar second, Payload data) const {
    RBNode<Scalar, Payload> *z = new RBNode<Scalar, Payload>(ClosedInterval<Scalar>(first, second), data, RBColor::red);
    z->left = NIL;
    z->right = NIL;
    z->par = NIL;
    return z;
}

template <typename Scalar, typename Payload>
RBNode<Scalar, Payload> *IntTree<Scalar, Payload>::contains(const ClosedInterval<Scalar> &i) const {
    RBNode<Scalar, Payload> *z = root;
    while (!RBNode<Scalar, Payload>::is_nil(z)) {
        if (z->intvl == i) {
            return z;
        }
        if (z->key() < i.first) {
            z = z->left;
        } else {
            z = z->right;
        }
    }
    return nullptr;
}

template <typename Scalar, typename Payload>
RBNode<Scalar, Payload> *IntTree<Scalar, Payload>::minimum(RBNode<Scalar, Payload> *x) const {
    if (RBNode<Scalar, Payload>::is_nil(x) || !x) {
        return nullptr;
    }
    while (!RBNode<Scalar, Payload>::is_nil(x->left)) {
        x = x->left;
    }
    return x;
}

template <typename Scalar, typename Payload>
RBNode<Scalar, Payload> *IntTree<Scalar, Payload>::successor(RBNode<Scalar, Payload> *x) const {
    if (RBNode<Scalar, Payload>::is_nil(x) || !x) {
        return nullptr;
    }
    if (!RBNode<Scalar, Payload>::is_nil(x->right)) {
        return minimum(x->right);
    }
    RBNode<Scalar, Payload> *y = x->par;
    while (!RBNode<Scalar, Payload>::is_nil(y) && x == y->right) {
        x = y;
        y = y->par;
    }
    return RBNode<Scalar, Payload>::is_nil(y) ? nullptr : y;
}

template <typename Scalar, typename Payload>
void IntTree<Scalar, Payload>::insert(RBNode<Scalar, Payload> *z) {
    if (RBNode<Scalar, Payload>::is_nil(z) || !z) {
        return;
    }

    RBNode<Scalar, Payload> *y = NIL;
    RBNode<Scalar, Payload> *x = root;
    while (!RBNode<Scalar, Payload>::is_nil(x)) {
        y = x;
        if (z->key() < x->key()) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->par = y;
    if (RBNode<Scalar, Payload>::is_nil(y)) {
        root = z;
    } else if (z->key() < y->key()) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = NIL;
    z->right = NIL;
    z->color = RBColor::red;
    z->max = z->intvl.second;
    RBNode<Scalar, Payload> *u = z->par;
    while (!is_nil(u)) {
        update_max(u);
        u = u->par;
    }

    // fixup
    while (z->par->color == RBColor::red) {
        if (z->par == z->par->par->left) {
            y = z->par->par->right;
            if (y->color == RBColor::red) {
                z->par->color = RBColor::black;
                y->color = RBColor::black;
                z->par->par->color = RBColor::red;
                z = z->par->par;
            } else {
                if (z == z->par->right) {
                    z = z->par;
                    left_rotate(z);
                }
                z->par->color = RBColor::black;
                z->par->par->color = RBColor::red;
                right_rotate(z->par->par);
            }
        } else {
            y = z->par->par->left;
            if (y->color == RBColor::red) {
                z->par->color = RBColor::black;
                y->color = RBColor::black;
                z->par->par->color = RBColor::red;
                z = z->par->par;
            } else {
                if (z == z->par->left) {
                    z = z->par;
                    right_rotate(z);
                }
                z->par->color = RBColor::black;
                z->par->par->color = RBColor::red;
                left_rotate(z->par->par);
            }
        }
    }
    root->color = RBColor::black;
}

template <typename Scalar, typename Payload>
void IntTree<Scalar, Payload>::erase(RBNode<Scalar, Payload> *z) {
    if (RBNode<Scalar, Payload>::is_nil(z) || !z) {
        return;
    }

    RBNode<Scalar, Payload> *y = z, *x;
    RBColor y_orig_color = y->color;
    if (RBNode<Scalar, Payload>::is_nil(z->left)) {
        x = z->right;
        transplant(z, z->right);
    } else if (RBNode<Scalar, Payload>::is_nil(z->right)) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_orig_color = y->color;
        x = y->right;
        if (y->par == z) {
            x->par = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->par = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->par = y;
        y->color = z->color;
    }
    RBNode<Scalar, Payload> *u = x->par;
    while (!RBNode<Scalar, Payload>::is_nil(u)) {
        update_max(u);
        u = u->par;
    }
    delete z;

    if (y_orig_color == RBColor::black) {
        // fixup
        RBNode<Scalar, Payload> *w;
        while (x != root && x->color == RBColor::black) {
            if (x == x->par->left) {
                w = x->par->right;
                if (w->color == RBColor::red) {
                    w->color = RBColor::black;
                    x->par->color = RBColor::red;
                    left_rotate(x->par);
                    w = x->par->right;
                }
                if (w->left->color == RBColor::black
                        && w->right->color == RBColor::black) {
                    w->color = RBColor::red;
                    x = x->par;
                } else {
                    if (w->right->color == RBColor::black) {
                        w->left->color = RBColor::black;
                        w->color = RBColor::red;
                        right_rotate(w);
                        w = x->par->right;
                    }
                    w->color = x->par->color;
                    x->par->color = RBColor::black;
                    w->right->color = RBColor::black;
                    left_rotate(x->par);
                    x = root;
                }
            } else {
                w = x->par->left;
                if (w->color == RBColor::red) {
                    w->color = RBColor::black;
                    x->par->color = RBColor::red;
                    right_rotate(x->par);
                    w = x->par->left;
                }
                if (w->right->color == RBColor::black
                        && w->left->color == RBColor::black) {
                    w->color = RBColor::red;
                    x = x->par;
                } else {
                    if (w->left->color == RBColor::black) {
                        w->right->color = RBColor::black;
                        w->color = RBColor::red;
                        left_rotate(w);
                        w = x->par->left;
                    }
                    w->color = x->par->color;
                    x->par->color = RBColor::black;
                    w->left->color = RBColor::black;
                    right_rotate(x->par);
                    x = root;
                }
            }
        }
        x->color = RBColor::black;
    }
}

template <typename Scalar, typename Payload>
RBNode<Scalar, Payload> *IntTree<Scalar, Payload>::intsearch(const ClosedInterval<Scalar> &i) const {
    RBNode<Scalar, Payload> *x = root;
    while (!RBNode<Scalar, Payload>::is_nil(x) && !x->intvl.overlap_with(i)) {
        if (!RBNode<Scalar, Payload>::is_nil(x->left) && x->left->max >= i.first) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return RBNode<Scalar, Payload>::is_nil(x) ? nullptr : x;
}

// Thanks https://github.com/YimingCuiCuiCui/Introduction-to-Algorithms-Solutions/blob/master/C14-Augmenting-Data-Structures/14.3.md#exercises-143-4
template <typename Scalar, typename Payload>
std::vector<RBNode<Scalar, Payload> *> IntTree<Scalar, Payload>::intsearch_all(const ClosedInterval<Scalar> &i) const {
    std::vector<RBNode<Scalar, Payload> *> result;
    if (RBNode<Scalar, Payload>::is_nil(root)) {
        return result;
    }
    std::vector<RBNode<Scalar, Payload> *> stack { root };
    while (!stack.empty()) {
        RBNode<Scalar, Payload> *curr = stack.back();
        stack.pop_back();

        if (curr->intvl.overlap_with(i)) {
            result.push_back(curr);
        }

        if (!RBNode<Scalar, Payload>::is_nil(curr->right)
            && curr->intvl.first <= i.second
            && curr->right->max >= i.first) {
            stack.push_back(curr->right);
        }
        if (!RBNode<Scalar, Payload>::is_nil(curr->left)
            && curr->left->max >= i.first) {
            stack.push_back(curr->left);
        }
    }
    return result;
}

}
