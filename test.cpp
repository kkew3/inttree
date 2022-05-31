//
//  test.cpp
//  inttree
//
//  Created by Kaiwen on 5/30/22.
//

#include "inttree.h"
#include <catch2/catch_test_macros.hpp>
#include <set>
#include <utility>
#include <vector>

using namespace inttree;

TEST_CASE("IntTree trivial insert")
{
	IntTree t;
	RBNode* node;
	ClosedInterval i;
	REQUIRE(IntTree::is_nil(t.root));
	REQUIRE(t.root->color == RBColor::black);

	i = make_interval(9, 16);
	t.insert(t.make_node(i));
	REQUIRE_FALSE(t.root == nullptr);
	REQUIRE_FALSE(IntTree::is_nil(t.root));
	REQUIRE(t.root->intvl == i);
	REQUIRE(IntTree::is_nil(t.root->left));
	REQUIRE(IntTree::is_nil(t.root->right));
	REQUIRE(IntTree::is_nil(t.root->par));
	REQUIRE(t.root->color == RBColor::black);
	REQUIRE(t.root->max == 16);
}

TEST_CASE("IntTree test")
{
	std::vector<ClosedInterval> intvls {
		make_interval(17, 19),
		make_interval(15, 23),
		make_interval(25, 30),
		make_interval(8, 9),
		make_interval(0, 3),
		make_interval(19, 20),
		make_interval(26, 26),
		make_interval(6, 10),
		make_interval(16, 21),
		make_interval(5, 8)
	};

	IntTree t;
	RBNode* node;
	ClosedInterval i;
	REQUIRE(IntTree::is_nil(t.root));
	REQUIRE(t.root->color == RBColor::black);

	for (auto i = intvls.begin(); i != intvls.end(); ++i) {
		t.insert(t.make_node(*i));
	}

	std::set<ClosedInterval> iset, refiset;

	// for the right answer see
	// https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
	SECTION("insert")
	{
		node = t.root;
		i = make_interval(17, 19);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(IntTree::is_nil(node->par));
		REQUIRE(node->color == RBColor::black);

		node = t.root->left;
		i = make_interval(8, 9);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::red);

		node = t.root->right;
		i = make_interval(25, 30);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(node->color == RBColor::black);

		node = t.root->left->left;
		i = make_interval(5, 8);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::black);

		node = t.root->left->right;
		i = make_interval(15, 23);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::black);
		REQUIRE(IntTree::is_nil(node->left));

		node = t.root->right->left;
		i = make_interval(19, 20);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 20);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t.root->right->right;
		i = make_interval(26, 26);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 26);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t.root->left->left->left;
		i = make_interval(0, 3);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 3);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t.root->left->left->right;
		i = make_interval(6, 10);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t.root->left->right->right;
		i = make_interval(16, 21);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 21);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));
	}

	SECTION("intsearch(16, 21)")
	{
		i = make_interval(16, 21);
		refiset.insert(make_interval(19, 20));
		refiset.insert(make_interval(17, 19));
		refiset.insert(make_interval(16, 21));
		refiset.insert(make_interval(15, 23));

		SECTION("intsearch(16, 21) one-by-one")
		{
			while ((node = t.intsearch(i))) {
				iset.insert(node->intvl);
				t.erase(node);
			}
			REQUIRE(iset == refiset);
		}

		SECTION("intsearch(16, 21) all")
		{
			std::vector<RBNode *> iset_vec = t.intsearch_all(i);
			for (auto itr = iset_vec.begin(); itr != iset_vec.end(); ++itr) {
				iset.insert((*itr)->intvl);
			}
			REQUIRE(iset == refiset);
		}
	}

	SECTION("intsearch(11, 13)")
	{
		i = make_interval(11, 13);
		
		SECTION("intsearch(11, 13) one-by-one")
		{
			while ((node = t.intsearch(i))) {
				iset.insert(node->intvl);
				t.erase(node);
			}
			REQUIRE(iset == refiset);
		}

		SECTION("intsearch(11, 13) all")
		{
			std::vector<RBNode *> iset_vec = t.intsearch_all(i);
			for (auto itr = iset_vec.begin(); itr != iset_vec.end(); ++itr) {
				iset.insert((*itr)->intvl);
			}
			REQUIRE(iset == refiset);
		}
	}

	SECTION("intsearch(3, 4)")
	{
		i = make_interval(3, 4);
		refiset.insert(make_interval(0, 3));

		SECTION("intsearch(3, 4) one-by-one")
		{
			while ((node = t.intsearch(i))) {
				iset.insert(node->intvl);
				t.erase(node);
			}
			REQUIRE(iset == refiset);
		}

		SECTION("intsearch(3, 4) all")
		{
			std::vector<RBNode *> iset_vec = t.intsearch_all(i);
			for (auto itr = iset_vec.begin(); itr != iset_vec.end(); ++itr) {
				iset.insert((*itr)->intvl);
			}
			REQUIRE(iset == refiset);
		}
	}

	SECTION("intsearch(23, 26)")
	{
		i = make_interval(23, 26);
		refiset.insert(make_interval(15, 23));
		refiset.insert(make_interval(25, 30));
		refiset.insert(make_interval(26, 26));

		SECTION("intsearch(23, 26) one-by-one")
		{
			while ((node = t.intsearch(i))) {
				iset.insert(node->intvl);
				t.erase(node);
			}
			REQUIRE(iset == refiset);
		}

		SECTION("intsearch(23, 26) all")
		{
			std::vector<RBNode *> iset_vec = t.intsearch_all(i);
			for (auto itr = iset_vec.begin(); itr != iset_vec.end(); ++itr) {
				iset.insert((*itr)->intvl);
			}
			REQUIRE(iset == refiset);
		}
	}

	SECTION("intsearch(9, 16) & intsearch(23, 25)")
	{
		i = make_interval(9, 16);
		refiset.insert(make_interval(8, 9));
		refiset.insert(make_interval(6, 10));
		refiset.insert(make_interval(15, 23));
		refiset.insert(make_interval(16, 21));
		while ((node = t.intsearch(i))) {
			iset.insert(node->intvl);
			t.erase(node);
		}
		REQUIRE(iset == refiset);

		i = make_interval(23, 25);
		refiset.clear();
		refiset.insert(make_interval(25, 30));
		iset.clear();
		while ((node = t.intsearch(i))) {
			iset.insert(node->intvl);
			t.erase(node);
		}
		REQUIRE(iset == refiset);
	}

	SECTION("erase all")
	{
		while ((node = t.minimum())) {
			t.erase(node);
		}
		REQUIRE(t.empty());
	}

	SECTION("clear")
	{
		t.clear();
		REQUIRE(t.empty());
	}

	SECTION("copy constructor")
	{
		IntTree t2(t);
		t.clear();
		REQUIRE_FALSE(t2.empty());

		node = t2.root;
		i = make_interval(17, 19);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(IntTree::is_nil(node->par));
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left;
		i = make_interval(8, 9);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::red);

		node = t2.root->right;
		i = make_interval(25, 30);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->left;
		i = make_interval(5, 8);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->right;
		i = make_interval(15, 23);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::black);
		REQUIRE(IntTree::is_nil(node->left));

		node = t2.root->right->left;
		i = make_interval(19, 20);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 20);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->right->right;
		i = make_interval(26, 26);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 26);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->left->left->left;
		i = make_interval(0, 3);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 3);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->left->left->right;
		i = make_interval(6, 10);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->left->right->right;
		i = make_interval(16, 21);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 21);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));
	}
	
	SECTION("operator=")
	{
		IntTree t2 = t;
		t.clear();
		REQUIRE_FALSE(t2.empty());

		node = t2.root;
		i = make_interval(17, 19);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(IntTree::is_nil(node->par));
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left;
		i = make_interval(8, 9);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::red);

		node = t2.root->right;
		i = make_interval(25, 30);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->left;
		i = make_interval(5, 8);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->right;
		i = make_interval(15, 23);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::black);
		REQUIRE(IntTree::is_nil(node->left));

		node = t2.root->right->left;
		i = make_interval(19, 20);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 20);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->right->right;
		i = make_interval(26, 26);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 26);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->left->left->left;
		i = make_interval(0, 3);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 3);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->left->left->right;
		i = make_interval(6, 10);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->left->right->right;
		i = make_interval(16, 21);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 21);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));
	}

	SECTION("move constructor")
	{
		IntTree t2(std::move(t));
		REQUIRE(t.empty());
		REQUIRE_FALSE(t2.empty());

		node = t2.root;
		i = make_interval(17, 19);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(IntTree::is_nil(node->par));
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left;
		i = make_interval(8, 9);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::red);

		node = t2.root->right;
		i = make_interval(25, 30);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->left;
		i = make_interval(5, 8);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->right;
		i = make_interval(15, 23);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::black);
		REQUIRE(IntTree::is_nil(node->left));

		node = t2.root->right->left;
		i = make_interval(19, 20);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 20);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->right->right;
		i = make_interval(26, 26);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 26);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->left->left->left;
		i = make_interval(0, 3);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 3);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->left->left->right;
		i = make_interval(6, 10);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));

		node = t2.root->left->right->right;
		i = make_interval(16, 21);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(IntTree::is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 21);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(IntTree::is_nil(node->left));
		REQUIRE(IntTree::is_nil(node->right));
	}

	SECTION("eq")
	{
		IntTree t2(t);
		REQUIRE(t.eq(t2));
	}

	SECTION("operator==")
	{
		IntTree t2(t);
		REQUIRE(t == t2);
	}
}
