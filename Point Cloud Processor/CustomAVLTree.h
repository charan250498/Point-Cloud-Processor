#pragma once
struct LinkedListNode {
	float x;
	float y;
	float z;
	struct LinkedListNode *right_next;
	struct LinkedListNode *left_next;
};
struct CustomAVLTreeNode {
	int height;
	float coordinate_value;
	struct LinkedListNode *head_node;
	struct CustomAVLTreeNode *left = NULL;
	struct CustomAVLTreeNode *right = NULL;
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
	static struct CustomAVLTreeNode* newNode(float coordinate_value, float x, float y, float z);
	struct CustomAVLTreeNode* rightRotate(struct CustomAVLTreeNode *y);
	struct CustomAVLTreeNode* leftRotate(struct CustomAVLTreeNode *x);
	int getBalance(struct CustomAVLTreeNode *N);
	struct CustomAVLTreeNode* insert(struct CustomAVLTreeNode* node, float coordinate_value, float x, float y, float z);
	struct LinkedListNode* searchPoint(struct CustomAVLTreeNode* root, float x, float y, float z);

	void preOrder(struct CustomAVLTreeNode *root);
};

