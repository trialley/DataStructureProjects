#pragma once
#ifndef WINNER_TREE_H
#define WINNER_TREE_H

#include <string.h>
#include <stdlib.h>

#ifndef RET_TRUE
#define RET_TRUE  0
#endif

#ifndef RET_FALSE
#define RET_FALSE -1
#endif

struct tree_node {
	int value;
	int outer_index;  // 记录来自于外部节点树组的哪个index
};

struct winner_tree {
	struct tree_node** inner_nodes;   // store the point which points to struct tree_node.
	struct tree_node** outer_nodes;
	int outer_size;
	int low_ext;  /* 最底层的外部节点数 */
	int offset;

};

static int winner_tree_init (struct winner_tree* tree, struct tree_node* arr[], int size);
static void winner_tree_destroy (struct winner_tree* tree);
static int winner_tree_replay (struct winner_tree* tree, int outer_index);
static int get_inner_index (struct winner_tree* tree, int outer_index);
static struct tree_node* get_winner (struct tree_node* n1, struct tree_node* n2);
static struct tree_node* get_partner_of_outer (struct winner_tree* tree, int outer_index);
static struct tree_node* get_partner_of_inner (struct winner_tree* tree, int inner_index);

static struct tree_node* get_winner (struct tree_node* n1, struct tree_node* n2) {
	if (!n1 && !n2)
		return NULL;

	if (n1 && !n2)
		return n1;

	if (!n1 && n2)
		return n2;

	if (n1->value >= n2->value)
		return n1;
	else
		return n2;
}

static int power_of_s (int s) {
	if (s < 0)
		return 1;
	else
		return 2 << s;
}

static struct tree_node* get_partner_of_inner (struct winner_tree* tree, int inner_index) {
	struct tree_node* partner_node = NULL;

	if (!tree || inner_index <= 1 || inner_index > tree->outer_size - 1)
		return NULL;

	if (inner_index == (tree->outer_size - 1)
		&& inner_index % 2 == 0) {
		partner_node = tree->outer_nodes[tree->low_ext];
	} else {
		if (inner_index % 2 == 0)
			partner_node = tree->inner_nodes[inner_index];
		else
			partner_node = tree->inner_nodes[inner_index - 2];
	}

	return partner_node;
}

static struct tree_node* get_partner_of_outer (struct winner_tree* tree, int outer_index) {
	struct tree_node* partner_node = NULL;
	int inner_i;

	if (!tree || outer_index < 1 || outer_index > tree->outer_size)
		return NULL;

	if (tree->outer_size % 2 == 0 || outer_index <= tree->low_ext) {
		if (outer_index % 2 == 0)
			partner_node = tree->outer_nodes[outer_index - 2];
		else
			partner_node = tree->outer_nodes[outer_index];
	} else {
		if (outer_index == tree->low_ext + 1) {
			inner_i = get_inner_index (tree, outer_index);
			partner_node = tree->inner_nodes[2 * inner_i - 1];
		} else {  // outer_index > tree->low_ext + 1
			if (outer_index % 2 == 0)
				partner_node = tree->outer_nodes[outer_index];
			else
				partner_node = tree->outer_nodes[outer_index - 2];
		}
	}

	return partner_node;
}

/*
 * @param 外部节点数组及其大小
 */
static int winner_tree_init (struct winner_tree* tree, struct tree_node* arr[], int size) {
	int s, outer_i, inner_i, end_i;
	struct tree_node* partner_node = NULL;

	if (NULL == tree || NULL == arr || size < 2)
		return RET_FALSE;

	for (s = 0; 2 << s <= (size - 1); s++);

	tree->low_ext = 2 * (size - power_of_s (s - 1));
	tree->offset = power_of_s (s) - 1;
	tree->outer_size = size;

	tree->outer_nodes = (tree_node**)malloc (size * sizeof (struct tree_node*));
	if (NULL == tree->outer_nodes)
		return RET_FALSE;
	memset (tree->outer_nodes, 0, size * sizeof (struct tree_node*));
	for (outer_i = 0; outer_i < size; outer_i++)
		tree->outer_nodes[outer_i] = arr[outer_i];

	tree->inner_nodes = (tree_node**)malloc ((size - 1) * sizeof (struct tree_node*));
	if (NULL == tree->inner_nodes) {
		free (tree->outer_nodes);
		tree->outer_nodes = NULL;
		return RET_FALSE;
	}
	memset (tree->inner_nodes, 0, (size - 1) * sizeof (struct tree_node*));

	/* 遍历所有外部节点 */
	for (outer_i = 1; outer_i <= tree->outer_size;) {
		inner_i = get_inner_index (tree, outer_i);
		partner_node = get_partner_of_outer (tree, outer_i);
		tree->inner_nodes[inner_i - 1] = get_winner (tree->outer_nodes[outer_i - 1], partner_node);

		if (tree->low_ext < tree->outer_size && outer_i == tree->low_ext + 1)
			outer_i += 1;
		else
			outer_i += 2;
	}
	/* 遍历所有内部节点 */
	inner_i = tree->outer_size - 1;
	while (inner_i > 1) {
		partner_node = get_partner_of_inner (tree, inner_i);
		tree->inner_nodes[inner_i / 2 - 1] = get_winner (tree->inner_nodes[inner_i - 1], partner_node);
		if ((tree->outer_size - 1) % 2 == 0)
			inner_i -= 1;
		else
			inner_i -= 2;
	}

	return RET_TRUE;
}

static void winner_tree_destroy (struct winner_tree* tree) {
	if (tree) {
		if (tree->outer_nodes)
			free (tree->outer_nodes);

		if (tree->inner_nodes)
			free (tree->inner_nodes);
	}
}

/*
 * 有一个成员更新, 其对应的比赛(往上遍历)全部重赛.
 */
static int winner_tree_replay (struct winner_tree* tree, int outer_index) {
	int inner_i;
	struct tree_node* partner_node, * replayer;

	if (!tree || outer_index < 1 || outer_index > tree->outer_size)
		return RET_FALSE;

	replayer = tree->outer_nodes[outer_index - 1];
	inner_i = get_inner_index (tree, outer_index);

	/* 获取replayer的partner */
	partner_node = get_partner_of_outer (tree, outer_index);
	/* Get new winner */
	tree->inner_nodes[inner_i - 1] = get_winner (partner_node, replayer);

	while (inner_i > 1) {
		if (inner_i % 2 == 0) {  // left-child
			if (inner_i + 1 <= tree->outer_size - 1)
				partner_node = tree->inner_nodes[inner_i];
			else
				partner_node = tree->outer_nodes[tree->low_ext];
		} else  // right-child
			partner_node = tree->inner_nodes[inner_i - 2];

		tree->inner_nodes[inner_i / 2 - 1] = get_winner (partner_node, tree->inner_nodes[inner_i - 1]);
		inner_i /= 2;
	}

	return RET_TRUE;
}

static int get_inner_index (struct winner_tree* tree, int outer_index) {
	int inner_index = -1;

	if (NULL == tree || outer_index < 1)
		return -1;

	if (outer_index <= tree->low_ext) {
		inner_index = (outer_index + tree->offset) / 2;
	} else {
		inner_index = (outer_index - tree->low_ext + tree->outer_size - 1) / 2;
	}

	return inner_index;
}

static void print_winner_tree (struct winner_tree* tree) {
	int i;

	printf ("Outer nodes: ");
	for (i = 1; i <= tree->outer_size; i++) {
		if (tree->outer_nodes[i - 1])
			printf ("%d ", tree->outer_nodes[i - 1]->value);
	}
	printf ("\n");

	printf ("Inner nodes: ");
	for (i = 1; i <= tree->outer_size - 1; i++) {
		if (tree->inner_nodes[i - 1])
			printf ("%d ", tree->inner_nodes[i - 1]->value);
	}

	printf ("\n");
}

#endif