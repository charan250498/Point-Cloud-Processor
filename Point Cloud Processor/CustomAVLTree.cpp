#include "stdafx.h"
#include "CustomAVLTree.h"
#include<stdlib.h>
#include<stdio.h>


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

struct CustomAVLTreeNode* CustomAVLTree::newNode(double coordinate_value, double x, double y, double z) {
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
	node->head_node->next = NULL;
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

struct CustomAVLTreeNode* CustomAVLTree::insert(struct CustomAVLTreeNode* node, double coordinate_value, double x, double y, double z) {
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
		while (temp->next != NULL) {
			temp = temp->next;
		}
		struct LinkedListNode *new_node = (struct LinkedListNode*)malloc(sizeof(struct LinkedListNode));
		new_node->x = x;
		new_node->y = y;
		new_node->z = z;
		new_node->next = NULL;
		temp->next = new_node;
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
			temp = temp->next;
		}
		preOrder(root->left);
		preOrder(root->right);
	}
}