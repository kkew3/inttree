//
//  inttree.cpp
//  inttree
//
//  Created by Kaiwen on 5/29/22.
//

#include "inttree.h"

using namespace inttree;

IntTree::~IntTree() {
    clear();
    root = nullptr;
    delete NIL;
    NIL = nullptr;
}

IntTree::IntTree(const IntTree &other) {
    root = other.clone();
    if (is_nil(root)) {
        NIL = root;
    } else {
        NIL = root->par;
    }
}

IntTree::IntTree(IntTree &&other) {
    if (is_nil(other.root)) {
        // nothing to move
        NIL = make_nil();
        root = NIL;
    } else {
        root = other.root;
        NIL = root->par;
        other.NIL = make_nil();
        other.root = other.NIL;
    }
}

IntTree &IntTree::operator=(const IntTree &other) {
    rbnode *new_root = other.clone();
    rbnode *new_nil;
    if (is_nil(new_root)) {
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

rbnode *IntTree::clone() const {
    rbnode *nil_copied = make_nil();
    if (is_nil(root)) {
        return nil_copied;
    }

    rbnode *root_copied = new rbnode(root->intvl, root->color);
    root_copied->max = root->max;
    root_copied->par = nil_copied;
    root_copied->left = nil_copied;
    root_copied->right = nil_copied;
    std::vector<rbnode *> stack { root };
    std::vector<rbnode *> stack_copied { root_copied };
    while (!stack.empty()) {
        rbnode *curr = stack.back();
        rbnode *curr_copied = stack_copied.back();
        stack.pop_back();
        stack_copied.pop_back();

        if (!is_nil(curr->right)) {
            stack.push_back(curr->right);
            curr_copied->right = new rbnode(curr->right->intvl, curr->right->color);
            curr_copied->right->max = curr->right->max;
            curr_copied->right->par = curr_copied;
            curr_copied->right->left = nil_copied;
            curr_copied->right->right = nil_copied;
            stack_copied.push_back(curr_copied->right);
        }

        if (!is_nil(curr->left)) {
            stack.push_back(curr->left);
            curr_copied->left = new rbnode(curr->left->intvl, curr->left->color);
            curr_copied->left->max = curr->left->max;
            curr_copied->left->par = curr_copied;
            curr_copied->left->left = nil_copied;
            curr_copied->left->right = nil_copied;
            stack_copied.push_back(curr_copied->left);
        }
    }
    return root_copied;
}

void IntTree::clear() {
    std::vector<rbnode *> stack;
    rbnode *prev = nullptr;
    while (!is_nil(root) || !stack.empty()) {
        if (!is_nil(root)) {
            stack.push_back(root);
            root = root->left;
        } else {
            root = stack.back();
            if (is_nil(root->right) || root->right == prev) {
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

bool IntTree::eq(const IntTree &other) const {
    if (is_nil(root) && is_nil(other.root)) {
        return true;
    }
    if (is_nil(root) || is_nil(other.root)) {
        return false;
    }

    std::vector<rbnode *> stack { root };
    std::vector<rbnode *> stack_other { other.root };
    while (!stack.empty()) {
        rbnode *curr = stack.back();
        rbnode *curr_other = stack_other.back();
        stack.pop_back();
        stack_other.pop_back();

        if (curr->intvl != curr_other->intvl || curr->max != curr_other->max) {
            return false;
        }

        if (!is_nil(curr->right) && !is_nil(curr_other->right)) {
            stack.push_back(curr->right);
            stack_other.push_back(curr_other->right);
        } else if (!is_nil(curr->right) || !is_nil(curr_other->right)) {
            return false;
        }

        if (!is_nil(curr->left) && !is_nil(curr_other->left)) {
            stack.push_back(curr->left);
            stack_other.push_back(curr_other->left);
        } else if (!is_nil(curr->left) || !is_nil(curr_other->left)) {
            return false;
        }
    }
    return true;
}

void IntTree::left_rotate(rbnode *x) {
    rbnode *y = x->right;
    x->right = y->left;
    if (!is_nil(y->left)) {
        y->left->par = x;
    }
    y->par = x->par;
    if (is_nil(x->par)) {
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

void IntTree::right_rotate(rbnode *y) {
    rbnode *x = y->left;
    y->left = x->right;
    if (!is_nil(x->right)) {
        x->right->par = y;
    }
    x->par = y->par;
    if (is_nil(y->par)) {
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

void IntTree::transplant(rbnode *u, rbnode *v) {
    if (is_nil(u->par)) {
        root = v;
    } else if (u == u->par->left) {
        u->par->left = v;
    } else {
        u->par->right = v;
    }
    v->par = u->par;
}

void IntTree::update_max(rbnode *z) {
    if (is_nil(z->left) && is_nil(z->right)) {
        z->max = z->intvl.second;
    } else if (is_nil(z->left)) {
        z->max = std::max(z->intvl.second, z->right->max);
    } else if (is_nil(z->right)) {
        z->max = std::max(z->intvl.second, z->left->max);
    } else {
		z->max = std::max(std::max(z->intvl.second, z->left->max), z->right->max);
    }
}

rbnode *IntTree::make_node(ClosedInterval i) const {
    rbnode *z = new rbnode(i, rbcolor::red);
    z->left = NIL;
    z->right = NIL;
    z->par = NIL;
    return z;
}

rbnode *IntTree::contains(const ClosedInterval &i) const {
    rbnode *z = root;
    while (!is_nil(z)) {
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

rbnode *IntTree::minimum(rbnode *x) const {
    while (!is_nil(x->left)) {
        x = x->left;
    }
    return x;
}

rbnode *IntTree::successor(rbnode *x) const {
    if (!is_nil(x->right)) {
        return minimum(x->right);
    }
    rbnode *y = x->par;
    while (!is_nil(y) && x == y->right) {
        x = y;
        y = y->par;
    }
    return is_nil(y) ? nullptr : y;
}

void IntTree::insert(rbnode *z) {
    rbnode *y = NIL;
    rbnode *x = root;
    while (!is_nil(x)) {
        y = x;
        if (z->key() < x->key()) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->par = y;
    if (is_nil(y)) {
        root = z;
    } else if (z->key() < y->key()) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = NIL;
    z->right = NIL;
    z->color = rbcolor::red;
    z->max = z->intvl.second;
    rbnode *u = z->par;
    while (!is_nil(u)) {
        update_max(u);
        u = u->par;
    }

    // fixup
    while (z->par->color == rbcolor::red) {
        if (z->par == z->par->par->left) {
            y = z->par->par->right;
            if (y->color == rbcolor::red) {
                z->par->color = rbcolor::black;
                y->color = rbcolor::black;
                z->par->par->color = rbcolor::red;
                z = z->par->par;
            } else {
                if (z == z->par->right) {
                    z = z->par;
                    left_rotate(z);
                }
                z->par->color = rbcolor::black;
                z->par->par->color = rbcolor::red;
                right_rotate(z->par->par);
            }
        } else {
            y = z->par->par->left;
            if (y->color == rbcolor::red) {
                z->par->color = rbcolor::black;
                y->color = rbcolor::black;
                z->par->par->color = rbcolor::red;
                z = z->par->par;
            } else {
                if (z == z->par->left) {
                    z = z->par;
                    right_rotate(z);
                }
                z->par->color = rbcolor::black;
                z->par->par->color = rbcolor::red;
                left_rotate(z->par->par);
            }
        }
    }
    root->color = rbcolor::black;
}

void IntTree::erase(rbnode *z) {
    rbnode *y = z, *x;
    rbcolor y_orig_color = y->color;
    if (is_nil(z->left)) {
        x = z->right;
        transplant(z, z->right);
    } else if (is_nil(z->right)) {
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
    rbnode *u = x->par;
    while (!is_nil(u)) {
        update_max(u);
        u = u->par;
    }
    delete z;

    if (y_orig_color == rbcolor::black) {
        // fixup
        rbnode *w;
        while (x != root && x->color == rbcolor::black) {
            if (x == x->par->left) {
                w = x->par->right;
                if (w->color == rbcolor::red) {
                    w->color = rbcolor::black;
                    x->par->color = rbcolor::red;
                    left_rotate(x->par);
                    w = x->par->right;
                }
                if (w->left->color == rbcolor::black
                        && w->right->color == rbcolor::black) {
                    w->color = rbcolor::red;
                    x = x->par;
                } else {
                    if (w->right->color == rbcolor::black) {
                        w->left->color = rbcolor::black;
                        w->color = rbcolor::red;
                        right_rotate(w);
                        w = x->par->right;
                    }
                    w->color = x->par->color;
                    x->par->color = rbcolor::black;
                    w->right->color = rbcolor::black;
                    left_rotate(x->par);
                    x = root;
                }
            } else {
                w = x->par->left;
                if (w->color == rbcolor::red) {
                    w->color = rbcolor::black;
                    x->par->color = rbcolor::red;
                    right_rotate(x->par);
                    w = x->par->left;
                }
                if (w->right->color == rbcolor::black
                        && w->left->color == rbcolor::black) {
                    w->color = rbcolor::red;
                    x = x->par;
                } else {
                    if (w->left->color == rbcolor::black) {
                        w->right->color = rbcolor::black;
                        w->color = rbcolor::red;
                        left_rotate(w);
                        w = x->par->left;
                    }
                    w->color = x->par->color;
                    x->par->color = rbcolor::black;
                    w->left->color = rbcolor::black;
                    right_rotate(x->par);
                    x = root;
                }
            }
        }
        x->color = rbcolor::black;
    }
}

rbnode *IntTree::intsearch(const ClosedInterval &i) const {
    rbnode *x = root;
    while (!is_nil(x) && !closed_interval_overlap(x->intvl, i)) {
        if (!is_nil(x->left) && x->left->max >= i.first) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    return is_nil(x) ? nullptr : x;
}

// Thanks https://github.com/YimingCuiCuiCui/Introduction-to-Algorithms-Solutions/blob/master/C14-Augmenting-Data-Structures/14.3.md#exercises-143-4
std::vector<rbnode *> IntTree::intsearch_all(const ClosedInterval &i) const {
    std::vector<rbnode *> result;
    if (is_nil(root)) {
        return result;
    }
    std::vector<rbnode *> stack { root };
    while (!stack.empty()) {
        rbnode *curr = stack.back();
        stack.pop_back();

        if (closed_interval_overlap(curr->intvl, i)) {
            result.push_back(curr);
        }

        if (!is_nil(curr->right) && curr->intvl.first <= i.second
            && curr->right->max >= i.first) {
            stack.push_back(curr->right);
        }
        if (!is_nil(curr->left) && curr->left->max >= i.first) {
            stack.push_back(curr->left);
        }
    }
    return result;
}