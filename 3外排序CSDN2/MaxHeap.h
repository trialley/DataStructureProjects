#pragma once
#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <cstring>
#include <cstdlib>
using namespace std;
#ifndef RET_TRUE
#define RET_TRUE  0
#endif

#ifndef RET_FALSE
#define RET_FALSE -1
#endif

/* 最大堆: 每次给指定大小的数组排序 */
struct max_heap {
	int* start;  // start address of array
	int size;
};

static void max_heap_init (struct max_heap* tree, int* arr, int size);
static int  max_heap_pop (struct max_heap* tree, int* ret);  /* 弹出最大元素至int *ret, 然后对剩下的元素重新排序 */
static void max_heap_sort (struct max_heap* tree, int parent_index, int left_index, int right_index);
static void max_heap_resort_subtree (struct max_heap* tree, int parent_index);
static void int_swap (int* a, int* b);
static void print_max_heap (struct max_heap* tree);

static void int_swap (int* a, int* b) {
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

static void max_heap_resort_subtree (struct max_heap* tree, int parent_index) {
	int left_index, right_index;

	if (NULL == tree || parent_index < 1 || parent_index >= tree->size)
		return;

	left_index = 2 * parent_index;
	right_index = 2 * parent_index + 1;

	if (left_index <= tree->size) {
		if (right_index > tree->size)
			right_index = -1;

		max_heap_sort (tree, parent_index, left_index, right_index);
	}
}

static void max_heap_sort (struct max_heap* tree, int parent_index, int left_index, int right_index) {
	int* parent, * left, * right;

	if (!tree || parent_index < 1 || left_index > tree->size)
		return;

	parent = tree->start + parent_index - 1;
	left = tree->start + left_index - 1;
	right = tree->start + right_index - 1;

	if (right_index > 1) {
		if (*left > * parent && *left >= *right) {
			int_swap (left, parent);

			max_heap_resort_subtree (tree, left_index);
		} else if (*right > * parent && *right >= *left) {
			int_swap (right, parent);

			max_heap_resort_subtree (tree, right_index);
		}
	} else {
		if (*left > * parent)
			int_swap (left, parent);
	}
}

static void max_heap_init (struct max_heap* tree, int* arr, int size) {
	int i;

	tree->start = arr;
	tree->size = size;

	i = size;
	if (i % 2 == 0) { // The last element is a left-child
		max_heap_sort (tree, i / 2, i, -1);  // -1 means no right-child
		i--;
	}
	while (i > 1) {
		max_heap_sort (tree, (int)(i / 2), i - 1, i);
		i -= 2;
	}
}

/*
 * 弹出最大元素至int *ret, 然后对剩下的元素重新排序
 */
static int max_heap_pop (struct max_heap* tree, int* ret) {
	if (tree->size < 1)
		return RET_FALSE;

	*ret = *(tree->start + 0);

	*(tree->start + 0) = *(tree->start + tree->size - 1);
	tree->size--;

	if (tree->size > 1)
		max_heap_resort_subtree (tree, 1);

	return RET_TRUE;
}

static void print_max_heap (struct max_heap* tree) {
	int i;

	for (i = 1; i <= tree->size; i++)
		printf ("%3d ", *(tree->start + i - 1));
	printf ("\n");
}

#endif