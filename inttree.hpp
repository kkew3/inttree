//
//  inttree.hpp
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
	Scalar first;
	Scalar second;

	ClosedInterval() { }
	ClosedInterval(Scalar first)
		: first(first)
		, second(first)
	{
	}
	ClosedInterval(Scalar first, Scalar second)
		: first(first)
		, second(second)
	{
	}
	ClosedInterval(const ClosedInterval<Scalar>& other) = default;
	~ClosedInterval() = default;

	inline bool overlap_with(const ClosedInterval<Scalar>& other) const
	{
		return !(second < other.first || other.second < first);
	}

	inline std::pair<Scalar, Scalar> as_pair() const
	{
		return std::make_pair(first, second);
	}
};

template <typename Scalar>
inline bool operator==(const ClosedInterval<Scalar>& i1, const ClosedInterval<Scalar>& i2)
{
	return i1.first == i2.first && i1.second == i2.second;
}

template <typename Scalar>
inline bool operator!=(const ClosedInterval<Scalar>& i1, const ClosedInterval<Scalar>& i2)
{
	return i1.first != i2.first || i1.second != i2.second;
}

template <typename Scalar>
inline bool operator<(const ClosedInterval<Scalar>& i1, const ClosedInterval<Scalar>& i2)
{
	if (i1.first < i2.first) {
		return true;
	}
	if (!(i1.first < i2.first)) {
		return false;
	}
	return i1.second < i2.second;
}

enum class RBColor { black,
	red };

/// Scalar must support `<` operator and be able to be default and copy constructed.
/// Payload must be able to be default constructed.
template <typename Scalar, typename Payload>
struct RBNode {
	ClosedInterval<Scalar> intvl;
	Scalar max;
	Payload data;
	RBNode* left;
	RBNode* right;
	RBNode* par;
	RBColor color;
	bool sentinel;

	/// fill with interval and payload data
	RBNode(ClosedInterval<Scalar> intvl, Payload data, RBColor color)
		: intvl(intvl)
		, max(intvl.second)
		, data(data)
		, left(nullptr)
		, right(nullptr)
		, par(nullptr)
		, color(color)
		, sentinel(false)
	{
	}
	/// leave payload data empty
	RBNode(ClosedInterval<Scalar> intvl, RBColor color)
		: intvl(intvl)
		, max(intvl.second)
		, left(nullptr)
		, right(nullptr)
		, par(nullptr)
		, color(color)
		, sentinel(false)
	{
	}
	RBNode(const RBNode<Scalar, Payload>&) = default;
	~RBNode() = default;

	static inline bool is_nil(const RBNode<Scalar, Payload>* node) { return node->sentinel; }
	static inline RBNode<Scalar, Payload>* make_nil()
	{
		return new RBNode;
	}

	inline int key() const { return intvl.first; }

private:
	RBNode()
		: left(nullptr)
		, right(nullptr)
		, par(nullptr)
		, color(RBColor::black)
		, sentinel(true)
	{
	}
};

/// not thread-safe.
/// Scalar must support `<` operator and be able to be default and copy constructed.
/// Payload must be able to be default constructed.
template <typename Scalar, typename Payload>
class IntTree {
public:
	using TreeClosedInterval = ClosedInterval<Scalar>;
	using TreeNode = RBNode<Scalar, Payload>;

	IntTree()
		: NIL(make_nil())
		, root(NIL)
	{
	}

	IntTree(const IntTree<Scalar, Payload>& other)
	{
		root = other.clone();
		if (is_nil(root)) {
			NIL = root;
		} else {
			NIL = root->par;
		}
	}

	IntTree(IntTree<Scalar, Payload>&& other)
	{
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

	~IntTree()
	{
		clear();
		root = nullptr;
		delete NIL;
		NIL = nullptr;
	}

	static inline bool is_nil(TreeNode* x) { return TreeNode::is_nil(x); }
	static inline TreeNode* make_nil() { return TreeNode::make_nil(); }

	IntTree& operator=(const IntTree<Scalar, Payload>& other)
	{
		TreeNode* new_root = other.clone();
		TreeNode* new_nil;
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

	TreeNode* clone() const
	{
		TreeNode* nil_copied = make_nil();
		if (is_nil(root)) {
			return nil_copied;
		}

		TreeNode* root_copied = new TreeNode(root->intvl, root->color);
		root_copied->max = root->max;
		root_copied->par = nil_copied;
		root_copied->left = nil_copied;
		root_copied->right = nil_copied;
		std::vector<TreeNode*> stack { root };
		std::vector<TreeNode*> stack_copied { root_copied };
		while (!stack.empty()) {
			TreeNode* curr = stack.back();
			TreeNode* curr_copied = stack_copied.back();
			stack.pop_back();
			stack_copied.pop_back();

			if (!is_nil(curr->right)) {
				stack.push_back(curr->right);
				curr_copied->right = new TreeNode(curr->right->intvl, curr->right->color);
				curr_copied->right->max = curr->right->max;
				curr_copied->right->par = curr_copied;
				curr_copied->right->left = nil_copied;
				curr_copied->right->right = nil_copied;
				stack_copied.push_back(curr_copied->right);
			}

			if (!is_nil(curr->left)) {
				stack.push_back(curr->left);
				curr_copied->left = new TreeNode(curr->left->intvl, curr->left->color);
				curr_copied->left->max = curr->left->max;
				curr_copied->left->par = curr_copied;
				curr_copied->left->left = nil_copied;
				curr_copied->left->right = nil_copied;
				stack_copied.push_back(curr_copied->left);
			}
		}
		return root_copied;
	}

	inline bool empty() const { return is_nil(root); }

	void clear()
	{
		std::vector<TreeNode*> stack;
		TreeNode* prev = nullptr;
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

	bool eq(const IntTree<Scalar, Payload>& other) const
	{
		if (is_nil(root) && is_nil(other.root)) {
			return true;
		}
		if (is_nil(root) || is_nil(other.root)) {
			return false;
		}

		std::vector<TreeNode*> stack { root };
		std::vector<TreeNode*> stack_other { other.root };
		while (!stack.empty()) {
			TreeNode* curr = stack.back();
			TreeNode* curr_other = stack_other.back();
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

	TreeNode* make_node(TreeClosedInterval i) const
	{
		TreeNode* z = new TreeNode(i, RBColor::red);
		z->left = NIL;
		z->right = NIL;
		z->par = NIL;
		return z;
	}

	TreeNode* make_node(TreeClosedInterval i, Payload data) const
	{
		TreeNode* z = new TreeNode(i, data, RBColor::red);
		z->left = NIL;
		z->right = NIL;
		z->par = NIL;
		return z;
	}

	TreeNode* make_node(Scalar first, Scalar second) const
	{
		TreeNode* z = new TreeNode(TreeClosedInterval(first, second), RBColor::red);
		z->left = NIL;
		z->right = NIL;
		z->par = NIL;
		return z;
	}

	TreeNode* make_node(Scalar first, Scalar second, Payload data) const
	{
		TreeNode* z = new TreeNode(TreeClosedInterval(first, second), data, RBColor::red);
		z->left = NIL;
		z->right = NIL;
		z->par = NIL;
		return z;
	}

	TreeNode* contains(const TreeClosedInterval& i) const
	{
		TreeNode* z = root;
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

	inline TreeNode* minimum() const
	{
		return empty() ? nullptr : minimum(root);
	}

	TreeNode* minimum(TreeNode* x) const
	{
		if (is_nil(x) || !x) {
			return nullptr;
		}
		while (!is_nil(x->left)) {
			x = x->left;
		}
		return x;
	}

	TreeNode* successor(TreeNode* x) const
	{
		if (is_nil(x) || !x) {
			return nullptr;
		}
		if (!is_nil(x->right)) {
			return minimum(x->right);
		}
		TreeNode* y = x->par;
		while (!is_nil(y) && x == y->right) {
			x = y;
			y = y->par;
		}
		return is_nil(y) ? nullptr : y;
	}

	void insert(TreeNode* z)
	{
		if (is_nil(z) || !z) {
			return;
		}

		TreeNode* y = NIL;
		TreeNode* x = root;
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
		z->color = RBColor::red;
		z->max = z->intvl.second;
		TreeNode* u = z->par;
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

	void erase(TreeNode* z)
	{
		if (is_nil(z) || !z) {
			return;
		}

		TreeNode *y = z, *x;
		RBColor y_orig_color = y->color;
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
		TreeNode* u = x->par;
		while (!is_nil(u)) {
			update_max(u);
			u = u->par;
		}
		delete z;

		if (y_orig_color == RBColor::black) {
			// fixup
			TreeNode* w;
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

	TreeNode* intsearch(const TreeClosedInterval& i) const
	{
		TreeNode* x = root;
		while (!is_nil(x) && !x->intvl.overlap_with(i)) {
			if (!is_nil(x->left) && x->left->max >= i.first) {
				x = x->left;
			} else {
				x = x->right;
			}
		}
		return is_nil(x) ? nullptr : x;
	}

	// Thanks https://github.com/YimingCuiCuiCui/Introduction-to-Algorithms-Solutions/blob/master/C14-Augmenting-Data-Structures/14.3.md#exercises-143-4
	std::vector<TreeNode*> intsearch_all(const TreeClosedInterval& i) const
	{
		std::vector<TreeNode*> result;
		if (is_nil(root)) {
			return result;
		}
		std::vector<TreeNode*> stack { root };
		while (!stack.empty()) {
			TreeNode* curr = stack.back();
			stack.pop_back();

			if (curr->intvl.overlap_with(i)) {
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

private:
	void left_rotate(TreeNode* x)
	{
		TreeNode* y = x->right;
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

	void right_rotate(TreeNode* y)
	{
		TreeNode* x = y->left;
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

	void transplant(TreeNode* u, TreeNode* v)
	{
		if (is_nil(u->par)) {
			root = v;
		} else if (u == u->par->left) {
			u->par->left = v;
		} else {
			u->par->right = v;
		}
		v->par = u->par;
	}

	void update_max(TreeNode* z)
	{
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

	// NIL must be declared before root (see default constructor)
	/// an internal representation; should be exposed to outside as nullptr
	TreeNode* NIL;

public:
	/// should be private, but left public for convenience of testing
	TreeNode* root;
};

template <typename Scalar, typename Payload>
inline bool operator==(const IntTree<Scalar, Payload>& t1, const IntTree<Scalar, Payload>& t2)
{
	return t1.eq(t2);
}

template <typename Scalar, typename Payload>
inline bool operator!=(const IntTree<Scalar, Payload>& t1, const IntTree<Scalar, Payload>& t2)
{
	return !t1.eq(t2);
}

}

#endif /* _INTTREE_H_ */
