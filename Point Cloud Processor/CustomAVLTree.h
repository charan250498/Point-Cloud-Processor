#pragma once
struct LinkedListNode {
	double x;
	double y;
	double z;
	struct LinkedListNode *next;
};
struct CustomAVLTreeNode {
	int height;
	double coordinate_value;
	struct LinkedListNode *head_node;
	struct CustomAVLTreeNode *left;
	struct CustomAVLTreeNode *right;
};
class CustomAVLTree
{
public:
	struct CustomAVLTreeNode *root;
public:
	CustomAVLTree();
	~CustomAVLTree();

	int Max(int a, int b);
	int height(struct CustomAVLTreeNode *Node);
	static struct CustomAVLTreeNode* newNode(double coordinate_value, double x, double y, double z);
	struct CustomAVLTreeNode* rightRotate(struct CustomAVLTreeNode *y);
	struct CustomAVLTreeNode* leftRotate(struct CustomAVLTreeNode *x);
	int getBalance(struct CustomAVLTreeNode *N);
	struct CustomAVLTreeNode* insert(struct CustomAVLTreeNode* node, double coordinate_value, double x, double y, double z);

	void preOrder(struct CustomAVLTreeNode *root);
};

