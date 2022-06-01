//
//  test.cpp
//  inttree
//
//  Created by Kaiwen on 5/30/22.
//

#include "inttree.hpp"
#include <catch2/catch_test_macros.hpp>
#include <set>
#include <vector>

using namespace inttree;

TEST_CASE("IntTree trivial insert")
{
	IntTree<int, int> t;
	RBNode<int, int>* node;
	ClosedInterval<int> i;
	REQUIRE(t.is_nil(t.root));
	REQUIRE(t.root->color == RBColor::black);

	i = ClosedInterval<int>(9, 16);
	t.insert(t.make_node(i));
	REQUIRE_FALSE(t.root == nullptr);
	REQUIRE_FALSE(t.is_nil(t.root));
	REQUIRE(t.root->intvl == i);
	REQUIRE(t.is_nil(t.root->left));
	REQUIRE(t.is_nil(t.root->right));
	REQUIRE(t.is_nil(t.root->par));
	REQUIRE(t.root->color == RBColor::black);
	REQUIRE(t.root->max == 16);
}

TEST_CASE("IntTree test")
{
	std::vector<ClosedInterval<int>> intvls {
		ClosedInterval<int>(17, 19),
		ClosedInterval<int>(15, 23),
		ClosedInterval<int>(25, 30),
		ClosedInterval<int>(8, 9),
		ClosedInterval<int>(0, 3),
		ClosedInterval<int>(19, 20),
		ClosedInterval<int>(26, 26),
		ClosedInterval<int>(6, 10),
		ClosedInterval<int>(16, 21),
		ClosedInterval<int>(5, 8)
	};

	IntTree<int, int> t;
	RBNode<int, int>* node;
	ClosedInterval<int> i;
	REQUIRE(t.is_nil(t.root));
	REQUIRE(t.root->color == RBColor::black);

	for (auto itr = intvls.begin(); itr != intvls.end(); ++itr) {
		t.insert(t.make_node(*itr));
	}

	std::set<ClosedInterval<int>> iset, refiset;

	// for the right answer see
	// https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
	SECTION("insert")
	{
		node = t.root;
		i = ClosedInterval<int>(17, 19);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(t.is_nil(node->par));
		REQUIRE(node->color == RBColor::black);

		node = t.root->left;
		i = ClosedInterval<int>(8, 9);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::red);

		node = t.root->right;
		i = ClosedInterval<int>(25, 30);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(node->color == RBColor::black);

		node = t.root->left->left;
		i = ClosedInterval<int>(5, 8);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::black);

		node = t.root->left->right;
		i = ClosedInterval<int>(15, 23);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::black);
		REQUIRE(t.is_nil(node->left));

		node = t.root->right->left;
		i = ClosedInterval<int>(19, 20);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 20);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t.is_nil(node->left));
		REQUIRE(t.is_nil(node->right));

		node = t.root->right->right;
		i = ClosedInterval<int>(26, 26);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 26);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t.is_nil(node->left));
		REQUIRE(t.is_nil(node->right));

		node = t.root->left->left->left;
		i = ClosedInterval<int>(0, 3);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 3);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t.is_nil(node->left));
		REQUIRE(t.is_nil(node->right));

		node = t.root->left->left->right;
		i = ClosedInterval<int>(6, 10);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t.is_nil(node->left));
		REQUIRE(t.is_nil(node->right));

		node = t.root->left->right->right;
		i = ClosedInterval<int>(16, 21);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 21);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t.is_nil(node->left));
		REQUIRE(t.is_nil(node->right));
	}

	SECTION("intsearch(16, 21)")
	{
		i = ClosedInterval<int>(16, 21);
		refiset.insert(ClosedInterval<int>(19, 20));
		refiset.insert(ClosedInterval<int>(17, 19));
		refiset.insert(ClosedInterval<int>(16, 21));
		refiset.insert(ClosedInterval<int>(15, 23));

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
			auto iset_vec = t.intsearch_all(i);
			for (auto itr = iset_vec.begin(); itr != iset_vec.end(); ++itr) {
				iset.insert((*itr)->intvl);
			}
			REQUIRE(iset == refiset);
		}
	}

	SECTION("intsearch(11, 13)")
	{
		i = ClosedInterval<int>(11, 13);

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
			auto iset_vec = t.intsearch_all(i);
			for (auto itr = iset_vec.begin(); itr != iset_vec.end(); ++itr) {
				iset.insert((*itr)->intvl);
			}
			REQUIRE(iset == refiset);
		}
	}

	SECTION("intsearch(3, 4)")
	{
		i = ClosedInterval<int>(3, 4);
		refiset.insert(ClosedInterval<int>(0, 3));

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
			auto iset_vec = t.intsearch_all(i);
			for (auto itr = iset_vec.begin(); itr != iset_vec.end(); ++itr) {
				iset.insert((*itr)->intvl);
			}
			REQUIRE(iset == refiset);
		}
	}

	SECTION("intsearch(23, 26)")
	{
		i = ClosedInterval<int>(23, 26);
		refiset.insert(ClosedInterval<int>(15, 23));
		refiset.insert(ClosedInterval<int>(25, 30));
		refiset.insert(ClosedInterval<int>(26, 26));

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
			auto iset_vec = t.intsearch_all(i);
			for (auto itr = iset_vec.begin(); itr != iset_vec.end(); ++itr) {
				iset.insert((*itr)->intvl);
			}
			REQUIRE(iset == refiset);
		}
	}

	SECTION("intsearch(9, 16) & intsearch(23, 25)")
	{
		i = ClosedInterval<int>(9, 16);
		refiset.insert(ClosedInterval<int>(8, 9));
		refiset.insert(ClosedInterval<int>(6, 10));
		refiset.insert(ClosedInterval<int>(15, 23));
		refiset.insert(ClosedInterval<int>(16, 21));
		while ((node = t.intsearch(i))) {
			iset.insert(node->intvl);
			t.erase(node);
		}
		REQUIRE(iset == refiset);

		i = ClosedInterval<int>(23, 25);
		refiset.clear();
		refiset.insert(ClosedInterval<int>(25, 30));
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
		IntTree<int, int> t2(t);
		t.clear();
		REQUIRE_FALSE(t2.empty());

		node = t2.root;
		i = ClosedInterval<int>(17, 19);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(t2.is_nil(node->par));
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left;
		i = ClosedInterval<int>(8, 9);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::red);

		node = t2.root->right;
		i = ClosedInterval<int>(25, 30);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->left;
		i = ClosedInterval<int>(5, 8);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->right;
		i = ClosedInterval<int>(15, 23);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::black);
		REQUIRE(t2.is_nil(node->left));

		node = t2.root->right->left;
		i = ClosedInterval<int>(19, 20);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 20);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->right->right;
		i = ClosedInterval<int>(26, 26);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 26);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->left->left->left;
		i = ClosedInterval<int>(0, 3);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 3);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->left->left->right;
		i = ClosedInterval<int>(6, 10);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->left->right->right;
		i = ClosedInterval<int>(16, 21);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 21);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));
	}

	SECTION("operator=")
	{
		IntTree<int, int> t2 = t;
		t.clear();
		REQUIRE_FALSE(t2.empty());

		node = t2.root;
		i = ClosedInterval<int>(17, 19);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(t2.is_nil(node->par));
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left;
		i = ClosedInterval<int>(8, 9);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::red);

		node = t2.root->right;
		i = ClosedInterval<int>(25, 30);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->left;
		i = ClosedInterval<int>(5, 8);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->right;
		i = ClosedInterval<int>(15, 23);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::black);
		REQUIRE(t2.is_nil(node->left));

		node = t2.root->right->left;
		i = ClosedInterval<int>(19, 20);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 20);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->right->right;
		i = ClosedInterval<int>(26, 26);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 26);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->left->left->left;
		i = ClosedInterval<int>(0, 3);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 3);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->left->left->right;
		i = ClosedInterval<int>(6, 10);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->left->right->right;
		i = ClosedInterval<int>(16, 21);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 21);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));
	}

	SECTION("move constructor")
	{
		IntTree<int, int> t2(std::move(t));
		REQUIRE(t.empty());
		REQUIRE_FALSE(t2.empty());

		node = t2.root;
		i = ClosedInterval<int>(17, 19);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(t2.is_nil(node->par));
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left;
		i = ClosedInterval<int>(8, 9);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::red);

		node = t2.root->right;
		i = ClosedInterval<int>(25, 30);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 30);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->left;
		i = ClosedInterval<int>(5, 8);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::black);

		node = t2.root->left->right;
		i = ClosedInterval<int>(15, 23);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 23);
		REQUIRE(node->color == RBColor::black);
		REQUIRE(t2.is_nil(node->left));

		node = t2.root->right->left;
		i = ClosedInterval<int>(19, 20);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 20);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->right->right;
		i = ClosedInterval<int>(26, 26);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 26);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->left->left->left;
		i = ClosedInterval<int>(0, 3);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 3);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->left->left->right;
		i = ClosedInterval<int>(6, 10);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 10);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));

		node = t2.root->left->right->right;
		i = ClosedInterval<int>(16, 21);
		REQUIRE_FALSE(node == nullptr);
		REQUIRE_FALSE(t2.is_nil(node));
		REQUIRE(node->intvl == i);
		REQUIRE(node->max == 21);
		REQUIRE(node->color == RBColor::red);
		REQUIRE(t2.is_nil(node->left));
		REQUIRE(t2.is_nil(node->right));
	}

	SECTION("eq")
	{
		IntTree<int, int> t2(t);
		REQUIRE(t.eq(t2));
	}

	SECTION("operator==")
	{
		IntTree<int, int> t2(t);
		REQUIRE(t == t2);
	}
}
