#include "stdafx.h"
#include "CustomAVLTree.h"
#include<stdlib.h>
#include<stdio.h>

struct LinkedListNodeForHighlighting* head;

CustomAVLTree::CustomAVLTree()
{
	this->root = NULL;
}


CustomAVLTree::~CustomAVLTree()
{
}

int CustomAVLTree::Max(int a, int b) {
	return a < b ? b : a;
}

int CustomAVLTree::height(struct CustomAVLTreeNode *Node) {
	if (Node == NULL) {
		return 0;
	}
	return Node->height;
}

struct CustomAVLTreeNode* CustomAVLTree::newNode(float coordinate_value, float x, float y, float z) {
	struct CustomAVLTreeNode* node = (struct CustomAVLTreeNode*)malloc(sizeof(struct CustomAVLTreeNode));
	struct LinkedListNode* linked_list_node = (struct LinkedListNode*)malloc(sizeof(struct LinkedListNode));
	node->coordinate_value = coordinate_value;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	node->head_node = linked_list_node;
	node->head_node->x = x;
	node->head_node->y = y;
	node->head_node->z = z;
	node->head_node->right_next = NULL;
	node->head_node->left_next = NULL;
	return(node);
}

struct CustomAVLTreeNode* CustomAVLTree::rightRotate(struct CustomAVLTreeNode *y) {
	struct CustomAVLTreeNode *x = y->left;
	struct CustomAVLTreeNode *temp = x->right;

	x->right = y;
	y->left = temp;

	y->height = Max(height(y->left), height(y->right)) + 1;
	x->height = Max(height(x->left), height(x->right)) + 1;

	return x;
}

struct CustomAVLTreeNode* CustomAVLTree::leftRotate(struct CustomAVLTreeNode *x) {
	struct CustomAVLTreeNode *y = x->right;
	struct CustomAVLTreeNode *temp = y->left;

	y->left = x;
	x->right = temp;

	x->height = Max(height(x->left), height(x->right)) + 1;
	y->height = Max(height(y->left), height(y->right)) + 1;

	return y;
}

int CustomAVLTree::getBalance(struct CustomAVLTreeNode *N) {
	if (N == NULL) {
		return 0;
	}
	return height(N->left) - height(N->right);
}

struct CustomAVLTreeNode* CustomAVLTree::insert(struct CustomAVLTreeNode* node, float coordinate_value, float x, float y, float z) {
	if (node == NULL) {
		return(newNode(coordinate_value, x, y, z));
	}
	if (coordinate_value < node->coordinate_value) {
		node->left = insert(node->left, coordinate_value, x, y, z);
	}
	else if (coordinate_value > node->coordinate_value) {
		node->right = insert(node->right, coordinate_value, x, y, z);
	}
	else {
		//equal coordinate values will come here
		struct LinkedListNode *temp = node->head_node;
		while (temp->right_next != NULL) {
			temp = temp->right_next;
		}
		struct LinkedListNode *new_node = (struct LinkedListNode*)malloc(sizeof(struct LinkedListNode));
		new_node->x = x;
		new_node->y = y;
		new_node->z = z;
		new_node->right_next = NULL;
		new_node->left_next = temp;
		temp->right_next = new_node;
		return node;
	}

	node->height = 1 + Max(height(node->left), height(node->right));

	int balance = getBalance(node);

	if (balance > 1 && coordinate_value < node->left->coordinate_value) {
		return rightRotate(node);
	}
	if (balance < -1 && coordinate_value > node->right->coordinate_value) {
		return leftRotate(node);
	}
	if (balance > 1 && coordinate_value > node->left->coordinate_value)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}
	if (balance < -1 && coordinate_value < node->right->coordinate_value)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

void CustomAVLTree::preOrder(struct CustomAVLTreeNode* root) {
	if (root != NULL) {
		struct LinkedListNode* temp = root->head_node;
		printf("CustomAVLTreeNode :%f\n", root->coordinate_value);
		printf("LinkedListNodes are as follows: \n");
		while (temp != NULL) {
			printf("Node : %f %f %f\n", temp->x, temp->y, temp->z);
			temp = temp->right_next;
		}
		preOrder(root->left);
		preOrder(root->right);
	}
}

struct LinkedListNode* CustomAVLTree::searchPoint(struct CustomAVLTreeNode* root, float x, float y, float z) {
	struct LinkedListNode* searched_points = NULL;
	struct CustomAVLTreeNode* tree_node = root;
	int flag = 0;
	if ((x == 9999 && y == 9999) || (y == 9999 && z == 9999) || (z == 9999 && x == 9999)) {
		if (x != 9999) {
			while (tree_node != NULL && flag != 1) {
				if (tree_node->coordinate_value == x) {
					searched_points = tree_node->head_node;
					flag = 1;
					break;
				}
				else if (tree_node->coordinate_value < x) {
					tree_node = tree_node->right;
				}
				else {
					tree_node = tree_node->left;
				}
			}
			if (flag == 0) {
				return NULL;
			}
		}
		else if (y != 9999) {
			while (tree_node != NULL && flag != 1) {
				if (tree_node->coordinate_value == y) {
					searched_points = tree_node->head_node;
					flag = 1;
					break;
				}
				else if (tree_node->coordinate_value < y) {
					tree_node = tree_node->right;
				}
				else {
					tree_node = tree_node->left;
				}
			}
			if (flag == 0) {
				return NULL;
			}
		}
		else {
			while (tree_node != NULL && flag != 1) {
				if (tree_node->coordinate_value == z) {
					searched_points = tree_node->head_node;
						flag = 1;
					break;
				}
				else if (tree_node->coordinate_value < z) {
					tree_node = tree_node->right;
				}
				else {
					tree_node = tree_node->left;
				}
			}
			if (flag == 0) {
				return NULL;
			}
		}
	}
	else if (x == 9999 || y == 9999 || z == 9999) {
		searched_points = (struct LinkedListNode*)malloc(sizeof(struct LinkedListNode));
		struct LinkedListNode* temporary_node;
		if (x == 9999) {
			while (tree_node != NULL && flag != 1 && flag != -1) {
				if (tree_node->coordinate_value == z) {
					temporary_node = tree_node->head_node;
					while (temporary_node != NULL) {
						if (temporary_node->y == y) {
							searched_points->x = temporary_node->x;
							searched_points->y = temporary_node->y;
							searched_points->z = temporary_node->z;
							searched_points->right_next = NULL;
							searched_points->left_next = NULL;
							flag = 1;
							break;
						}
						else {
							temporary_node = temporary_node->right_next;
							if (temporary_node == NULL) {
								flag = -1;
								break;
							}
						}
					}
				}
				else if (tree_node->coordinate_value > z) {
					tree_node = tree_node->left;
				}
				else {
					tree_node = tree_node->right;
				}
			}
			if (flag == 0) {
				return NULL;
			}
			if (flag == -1) {
				return NULL;
			}
		}
		else if (y == 9999) {
			while (tree_node != NULL && flag != 1 && flag != -1) {
				if (tree_node->coordinate_value == x) {
					int loop_count = 0;
					temporary_node = tree_node->head_node;
					while (temporary_node != NULL) {
						if (temporary_node->z == z) {
							searched_points->x = temporary_node->x;
							searched_points->y = temporary_node->y;
							searched_points->z = temporary_node->z;
							searched_points->right_next = NULL;
							searched_points->left_next = NULL;
							flag = 1;
							break;
						}
						else {
							temporary_node = temporary_node->right_next;
							if (temporary_node == NULL) {
								flag = -1;
								break;
							}
						}
					}
				}
				else if (tree_node->coordinate_value > x) {
					tree_node = tree_node->left;
				}
				else {
					tree_node = tree_node->right;
				}
			}
			if (flag == 0) {
				return NULL;
			}
			if (flag == -1) {
				return NULL;
			}
		}
		else {
			while (tree_node != NULL && flag != 1 && flag != -1) {
				if (tree_node->coordinate_value == x) {
					temporary_node = tree_node->head_node;
					while (temporary_node != NULL) {
						if (temporary_node->y == y) {
							searched_points->x = temporary_node->x;
							searched_points->y = temporary_node->y;
							searched_points->z = temporary_node->z;
							searched_points->right_next = NULL;
							searched_points->left_next = NULL;
							flag = 1;
							break;
						}
						else {
							temporary_node = temporary_node->right_next;
							if (temporary_node == NULL) {
								flag = -1;
								break;
							}
						}
					}
				}
				else if (tree_node->coordinate_value > x) {
					tree_node = tree_node->left;
				}
				else {
					tree_node = tree_node->right;
				}
			}
			if (flag == 0) {
				return NULL;
			}
			if (flag == -1) {
				return NULL;
			}
		}
	}
	else {
		searched_points = (struct LinkedListNode*)malloc(sizeof(struct LinkedListNode));
		searched_points->x = x;
		searched_points->y = y;
		searched_points->z = z;
		searched_points->right_next = NULL;
		searched_points->left_next = NULL;
	}
	return searched_points;
}

struct LinkedListNodeForHighlighting* CustomAVLTree::inOrder(struct CustomAVLTreeNode *root) {
	if (root != NULL) {
		inOrder(root->left);
		struct LinkedListNodeForHighlighting* new_node = (struct LinkedListNodeForHighlighting*)malloc(sizeof(struct LinkedListNodeForHighlighting));
		if (head == NULL) {
			new_node->coordinate_value = root->coordinate_value;
			new_node->head_node = root->head_node;
			new_node->left_next = NULL;
			new_node->right_next = NULL;
			head = new_node;
		}
		else {
			struct LinkedListNodeForHighlighting* temp = head;
			while (temp->right_next != NULL) {
				temp = temp->right_next;
			}
			new_node->coordinate_value = root->coordinate_value;
			new_node->head_node = root->head_node;
			new_node->left_next = temp;
			temp->right_next = new_node;
			new_node->right_next = NULL;
		}
		inOrder(root->right);
	}
	return head;
}