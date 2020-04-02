#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MaxHeap.h"
#include "WinnerTree.h"

static int is_array_empty (struct tree_node** arr, int size) {
	int i;
	if (arr) {
		for (i = 0; i < size; i++) {
			if (arr[i])
				return 0;
		}
	}
	return 1;
}

static int is_array_sorted (int* arr, int size, int sorted_nums) {
	int i;

	for (i = 0; i < size - 1; i++) {
		if (arr[i] < arr[i + 1]) {
			printf ("Result is not sorted properly!\n");
			return RET_FALSE;
		}
	}

	if (size != sorted_nums) {
		printf ("ERROR: get %d results, not equal %d.\n", sorted_nums, size);
		return RET_FALSE;
	} else {
		printf ("Result is sorted!\n");
		return RET_TRUE;
	}
}

int main (int argc, char* argv[]) {
	int total_nums, heap_size;
	int* arr, ret, i, j, max, arr_size, outer_index, result;
	struct max_heap** heaps;
	struct winner_tree* winner;
	struct tree_node** nodes;

	// Verify the result
	int* sorted_arr;

	if (argc != 3) {
		printf ("Usage: ./Sort 10 100\n");
		return -1;
	}

	total_nums = atoi (argv[2]);
	heap_size = atoi (argv[1]);

	if (heap_size < 2) {
		printf ("Usage: The 3th parameter should be larger than 1. Now it's %d.\n", heap_size);
		return -1;
	}

	if (total_nums <= heap_size) {
		printf ("Usage: The 2nd parameter should be larger than the 3th parameter.\n");
		return -1;
	}

	if (total_nums % heap_size != 0) {
		printf ("Usage: The 2nd parameter should be devided by the 3th parameter.\n");
		return -1;
	}

	sorted_arr = (int * )malloc (total_nums * sizeof (int));
	memset (sorted_arr, 0, total_nums * sizeof (int));

	/*
	 * 生成原始数据, 一行 heap_size 个元素,依此打印出来
	 */
	arr = (int*)malloc (total_nums * sizeof (int));
	memset (arr, 0, total_nums * sizeof (int));
	for (i = 1; i <= total_nums; i++)
		*(arr + i - 1) = rand () % 1000;
	printf ("Input Array: \n");
	for (i = 1; i <= total_nums; i++) {
		if (i % heap_size == 1)
			printf ("%3d - %3d: ", i, i + heap_size - 1);

		printf ("%3d ", *(arr + i - 1));

		if (i % heap_size == 0)
			printf ("\n");
	}
	printf ("\n\n");

	arr_size = total_nums / heap_size;

	/*
	 * 每heap_size个元素组成一个最大堆, 共申请 (total_nums/heap_size) 个struct max_heap.
	 */
	heaps = (max_heap**)malloc (arr_size * sizeof (struct max_heap*));
	memset (heaps, 0, sizeof (struct max_heap*));
	for (i = 0; i < arr_size; i++) {
		heaps[i] = (max_heap*)malloc (sizeof (struct max_heap));
		memset (heaps[i], 0, sizeof (struct max_heap));
	}
	for (i = 1; i <= total_nums; i += heap_size) {
		printf ("   Input: ");
		for (j = i; j < i + heap_size; j++)
			printf ("%3d ", arr[j - 1]);
		printf ("\n");

		// 初始化最大堆
		max_heap_init (heaps[i / heap_size], arr + i - 1, heap_size);
		printf ("heaps[%d]: ", i / heap_size);
		print_max_heap (heaps[i / heap_size]);

#if 0
		// 依此弹出最大值
		printf ("  Sorted: ");
		while (heaps[i / heap_size]->size) {
			max_heap_pop (heaps[i / heap_size], &ret);
			printf ("%3d ", ret);
		}
#endif
		printf ("\n");
	}

	/*
	 * 构建一个最大赢者树, heap_size 个外部节点
	 */
	 // 初始化最大赢者树的外部节点
	nodes = (tree_node**)malloc (arr_size * sizeof (struct tree_node*));
	memset (nodes, 0, arr_size * sizeof (struct tree_node*));
	for (i = 0; i < arr_size; i++) {
		nodes[i] =(tree_node*) malloc (sizeof (struct tree_node));
		memset (nodes[i], 0, sizeof (struct tree_node));
	}
	// 给外部节点赋值
	for (i = 0; i < arr_size; i++) {
		ret = 0;
		max_heap_pop (heaps[i], &ret);
		nodes[i]->value = ret;
		nodes[i]->outer_index = i + 1;
	}

	// 初始化最大赢者树
	winner = (winner_tree*)malloc (sizeof (struct winner_tree));
	memset (winner, 0, sizeof (struct winner_tree));
	winner_tree_init (winner, nodes, arr_size);
	print_winner_tree (winner);

	// Output winner and Replay
	printf ("\nAfter Sorted: \n");
	i = 1;
	while (1) {
		if (i % heap_size == 1)
			printf ("%3d - %3d: ", i, i + heap_size - 1);

		if (winner->inner_nodes[0]) {
			max = winner->inner_nodes[0]->value;
			outer_index = winner->inner_nodes[0]->outer_index;
			printf ("%3d[%2d] ", max, outer_index - 1);

			sorted_arr[i - 1] = max;
		} else {
			printf ("No max!\n");
			break;
		}

		// Replay 
		ret = 0;
		if (RET_FALSE == max_heap_pop (heaps[outer_index - 1], &ret)) {
			//printf("\nPop fail on heaps[%d]\n", outer_index);
			winner->outer_nodes[outer_index - 1] = NULL;

			if (is_array_empty (winner->outer_nodes, arr_size))
				break;

		} else {
			winner->outer_nodes[outer_index - 1]->value = ret;
			winner->outer_nodes[outer_index - 1]->outer_index = outer_index;
		}
		winner_tree_replay (winner, outer_index);

		if (i % heap_size == 0)
			printf ("\n");

		i++;
	}
	printf ("\n\n");

	// Verify the result
	if (RET_FALSE == is_array_sorted (sorted_arr, total_nums, i))
		result = -1;
	else
		result = 0;

	/*
	 * Free
	 */
	for (i = 0; i < arr_size; i++) {
		free (nodes[i]);
		nodes[i] = NULL;
	}
	free (nodes);
	nodes = NULL;

	winner_tree_destroy (winner);
	free (winner);
	winner = NULL;

	for (i = 0; i < total_nums / heap_size; i++) {
		free (heaps[i]);
		heaps[i] = NULL;
	}
	free (heaps);
	heaps = NULL;

	free (arr);
	arr = NULL;

	free (sorted_arr);
	sorted_arr = NULL;

	return result;
}