#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

typedef struct Pnode {

	Pnode * left;
	Pnode * right;
	Pnode * parent;

	int value;//노드의 value
	char color;//노드의 color

}Node;

typedef Node * NodePtr;

typedef struct RBTree {

	NodePtr root;
	NodePtr NIL;

	int count;//노드의 개수 count
	int blackcount;//black노드의 개수 count
	int blackheight;//blackheight 확안
	int insert;//insert된 노드의 개수 count
	int del;//delete된 노드의 개수를 count
	int miss;//miss된 노드의 개수 count

}Tree;

typedef Tree * TreePtr;

NodePtr NodeAlloc(TreePtr self, int data) {

	NodePtr Pnode = (NodePtr)malloc(sizeof(Node));
	Pnode->color = 'R';
	Pnode->value = data;
	Pnode->left = self->NIL;
	Pnode->right = self->NIL;
	Pnode->parent = self->NIL;
	return Pnode;
}

TreePtr TreeAlloc() {

	TreePtr PTree = (TreePtr)malloc(sizeof(Tree));
						
	PTree->count = 0;
	PTree->blackcount = 0;
	PTree->blackheight = 0;
	PTree->del = 0;
	PTree->insert = 0;
	PTree->miss = 0;

	PTree->NIL = (NodePtr)malloc(sizeof(Node));
	PTree->NIL->left = PTree->NIL;
	PTree->NIL->right = PTree->NIL;
	PTree->NIL->parent = PTree->NIL;
	PTree->NIL->color = 'B';
	PTree->NIL->value = 0;


	PTree->root = NodeAlloc(PTree, 0);
	PTree->root = PTree->NIL;
	PTree->root->parent = PTree->NIL;
	return PTree;
}

void Rotate_right(TreePtr self, NodePtr root)
{

	NodePtr y = root->left;
	root->left = y->right;

	if (y->right != self->NIL)
	{
		y->right->parent = root;
	}

	y->parent = root->parent;


	if (root->parent == self->NIL)
	{
		self->root = y;
	}

	else if (root == root->parent->left)
	{
		root->parent->left = y;
	}
	else
	{
		root->parent->right = y;
	}
	y->right = root;
	root->parent = y;
}

void Rotate_left(TreePtr self, NodePtr root)
{

	NodePtr y = root->right;

	root->right = y->left;

	if (y->left != self->NIL)
	{
		y->left->parent = root;
	}

	y->parent = root->parent;

	if (root->parent == self->NIL)
	{
		self->root = y;
	}

	else if (root == root->parent->left)
	{
		root->parent->left = y;
	}
	else
	{
		root->parent->right = y;
	}
	y->left = root;
	root->parent = y;
}


NodePtr rb_search(TreePtr self, NodePtr x, int data)
{
	if (x == self->NIL || x->value == data)
		return x;

	if (data < x->value)
		return rb_search(self, x->left, data);

	else
		return rb_search(self, x->right, data);
}



void rb_blackcount(TreePtr self, NodePtr x)
{
	if (x == self->NIL)
		return;
	if (x->color == 'B')
		self->blackcount++;

	rb_blackcount(self, x->left);

	rb_blackcount(self, x->right);
}


int rb_blackheight(TreePtr self, NodePtr x)
{
	int bh = 1;


	while (x->left != self->NIL && x->right != self->NIL)
	{
		if (x->color == 'B')
			bh++;
		x = x->right;
	}
	return bh;
}

void RBprint_inorder(TreePtr self, NodePtr x)
{
	if (x == self->NIL)
		return;

	RBprint_inorder(self, x->left);
	printf("%d", x->value);
	printf(" %c\n", x->color);
	RBprint_inorder(self, x->right);
}


int Insert_Fixup(TreePtr self, NodePtr target)
{
	if (target == self->root || target->parent == self->root)
	{
		self->root->color = 'B';
		return 0;
	}


	while (target != self->NIL && target != self->root && target->parent->color == 'R')
	{

		if (target->parent == target->parent->parent->left)
		{
			NodePtr uncle = target->parent->parent->right;
			if (uncle->color == 'R')
			{
				target->parent->color = 'B';
				uncle->color = 'B';
				target->parent->parent->color = 'R';
				target = target->parent->parent;
			}

			else
			{
				if (target == target->parent->right)
				{
					target = target->parent;
					Rotate_left(self, target);
				}

				target->parent->color = 'B';
				target->parent->parent->color = 'R';
				Rotate_right(self, target->parent->parent);
			}
		}

		else
		{
			NodePtr uncle = target->parent->parent->left;

			if (uncle->color == 'R')
			{
				target->parent->color = 'B';
				uncle->color = 'B';
				target->parent->parent->color = 'R';
				target = target->parent->parent;
			}

			else
			{
				if (target == target->parent->left)
				{
					target = target->parent;
					Rotate_right(self, target);
				}
				target->parent->color = 'B';
				target->parent->parent->color = 'R';
				Rotate_left(self, target->parent->parent);
			}
		}
	}

	self->root->color = 'B';
  return 0;
}


int rb_insert(TreePtr self, int data)
{
	NodePtr a = self->NIL;
	NodePtr b = self->root;
	NodePtr newNode = NodeAlloc(self, data);

	if (self->count == 0)
	{
		self->root = newNode;
		self->count++;
		return 0;
	}


	while (b != self->NIL)
	{
		a = b;
		if (newNode->value < b->value)
		{
			b = b->left;
		}
		else
		{
			b = b->right;
		}
	}

	newNode->parent = a;

	if (newNode->value < a->value)
	{
		a->left = newNode;
	}
	else
	{
		a->right = newNode;
	}


	newNode->left = self->NIL;
	newNode->right = self->NIL;
	newNode->color = 'R';

	Insert_Fixup(self, newNode);
	self->count++;
	return 0;
}



void Transplant(TreePtr self, NodePtr a, NodePtr b)
{
	if (a->parent == self->NIL)
		self->root = b;

	else if (a == a->parent->left)
		a->parent->left = b;

	else
		a->parent->right = b;

	b->parent = a->parent;
}

NodePtr Minimum(TreePtr self, NodePtr x)
{
	while (x->left != self->NIL) {
		x = x->left;
	}
	return x;
}


void Delete_fixup(TreePtr self, NodePtr x) {

	while (x != self->root && x->color == 'B') {

		if (x == x->parent->left) {

			NodePtr b = x->parent->right;
			if (b->color == 'R')
			{
				b->color = 'B';
				x->parent->color = 'R';
				Rotate_left(self, x->parent);
				b = x->parent->right;
			}

			if (b->left->color == 'B' && b->right->color == 'B')
			{
				b->color = 'R';
				x = x->parent;
			}

			else if (b->right->color == 'B')
			{
				b->left->color = 'B';
				b->color = 'R';
				Rotate_right(self, b);
				b = x->parent->right;
			}

			b->color = x->parent->color;
			x->parent->color = 'B';
			b->right->color = 'B';
			Rotate_left(self, x->parent);
			x = self->root;
		}


		else {

			NodePtr b = x->parent->left;
			if (b->color == 'R') {
				b->color = 'B';
				x->parent->color = 'R';
				Rotate_left(self, x->parent);
				b = x->parent->left;
			}

			if (b->right->color == 'B' && b->left->color == 'B') {
				b->color = 'R';
				x = x->parent;
			}

			else if (b->left->color == 'B') {
				b->right->color = 'B';
				b->color = 'R';
				Rotate_left(self, b);
				b = x->parent->left;
			}

			b->color = x->parent->color;
			x->parent->color = 'B';
			b->left->color = 'B';
			Rotate_right(self, x->parent);
			x = self->root;
		}
	}

	x->color = 'B';
}

void rb_delete(TreePtr self, NodePtr target)
{
	NodePtr x;
	NodePtr y = target;
	char original_color = y->color;

								   
	if (target->left == self->NIL)
	{
		x = target->right;
		Transplant(self, target, target->right);
	}

	
	else if (target->right == self->NIL)
	{
		x = target->left;
		Transplant(self, target, target->left);
	}


	else
	{
		y = Minimum(self, target->right);
		original_color = y->color;
		x = y->right;

		if (y->parent == target)
		{
			x->parent = y;
		}
		else
		{
			Transplant(self, y, y->right);
			y->right = target->right;
			y->right->parent = y;
		}

		Transplant(self, target, y);
		y->left = target->left;
		y->left->parent = y;
		y->color = target->color;
	}
	
	if (original_color == 'B')
	{
		Delete_fixup(self, x);
	}

	self->count--;
}


NodePtr Maximum(TreePtr self, NodePtr x) {
	while (x->right != self->NIL) {
		x = x->right;
	}
	return x;
}

NodePtr Tree_Successor(TreePtr self, NodePtr x) {

	if (x->right != self->NIL) {
		return Minimum(self, x->right);
	}

	NodePtr y = x->parent;

	while (y != self->NIL && x == y->right) {
		x = y;
		y = y->parent;
	}
	return y;
}

NodePtr Tree_predeccessor(TreePtr self, NodePtr x) {

	if (x->left != self->NIL) {
		return Maximum(self, x->left);
	}
	NodePtr y = x->parent;
	while (y != self->NIL && x == y->left) {
		x = y;
		y = y->parent;
	}
	return y;
}

void search_operation(TreePtr self, int data) {
	FILE *fp3 = NULL;

	fp3 = fopen("output.txt", "a");


	if (data == 0)
		return;
	if (rb_search(self, self->root, data) != self->NIL) {
		if (Tree_predeccessor(self, rb_search(self, self->root, data)) != self->NIL) {
			fprintf(fp3, "%d  ", Tree_predeccessor(self, rb_search(self, self->root, data))->value);
		}
		else
			fprintf(fp3, "NIL  ");

		fprintf(fp3, "%d  ", data);

		if (Tree_Successor(self, rb_search(self, self->root, data)) != self->NIL) {
			fprintf(fp3, "%d\n", Tree_Successor(self, rb_search(self, self->root, data))->value);
		}
		else
			fprintf(fp3, "NIL\n");
	}

	else
	{
		rb_insert(self, data);

		if (Tree_predeccessor(self, rb_search(self, self->root, data)) != self->NIL) {
			fprintf(fp3, "%d  ", Tree_predeccessor(self, rb_search(self, self->root, data))->value);
		}
		else
			fprintf(fp3, "NIL  ");

		fprintf(fp3, "NIL  ");

		if (Tree_Successor(self, rb_search(self, self->root, data)) != self->NIL) {
			fprintf(fp3, "%d\n", Tree_Successor(self, rb_search(self, self->root, data))->value);
		}
		else
			fprintf(fp3, "NIL\n");

		rb_delete(self, rb_search(self, self->root, data));

	}
	fclose(fp3);

}

void input_operation(TreePtr self, int data)
{

	if (data>0)
	{
		rb_insert(self, data);
		self->insert++;
	}

	else if (data<0)
	{
		if (rb_search(self, self->root, (data*(-1))) == self->NIL)
		{
			self->miss++;
		}
		else
		{
			rb_delete(self, rb_search(self, self->root, (data*(-1))));
			self->del++;
		}
	}
	else
		return;

}


void main()
{
	TreePtr RBT = TreeAlloc();
	int data1, data2;
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;


	fopen_s(&fp1, "input.txt", "r");
	fscanf_s(fp1, "%d", &data1);
	while (data1 != 0)
	{
		input_operation(RBT, data1);
		fscanf_s(fp1, "%d", &data1);
	}
	input_operation(RBT, data1);
	fclose(fp1);


	fopen_s(&fp2, "search.txt", "r");
	fscanf_s(fp2, "%d", &data2);
	while (data2 != 0)
	{
		search_operation(RBT, data2);
		fscanf_s(fp2, "%d", &data2);
	}
	search_operation(RBT, data2);
	fclose(fp2);


	return;
}
