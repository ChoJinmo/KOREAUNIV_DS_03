#include <stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS

#define RED 0
#define BLACK 1

typedef struct NODE {
	int clr, key;
	NODE *left, *right, *p;
};

struct ROOT {
	NODE* nil, *r;

};

void create_nilnode(ROOT *r)
{
	r->nil = (NODE*)malloc(sizeof(NODE));
	(r->nil)->clr = BLACK;
	r->r = r->nil;
}

void left_rotate(ROOT *r, NODE *x)
{
	NODE *temp;

	temp = x->right;
	x->right = temp->left;

	if (temp->left != r->nil)
	{
		(temp->left)->p = x;
	}

	temp->p = x->p;

	if (x->p == r->nil)
	{
		r->r = temp;
	}
	else if (x == (x->p)->left)
	{
		(x->p)->left = temp;
	}
	else
	{
		(x->p)->right = temp;
	}

	temp->left = x;
	x->p = temp;
}

void right_rotate(ROOT *r, NODE *y)
{
	NODE *temp;

	temp = y->left;
	y->left = temp->right;

	if (temp->right != r->nil)
	{
		(temp->right)->p = y;
	}

	temp->p = y->p;

	if (y->p == r->nil)
	{
		r->r = temp;
	}
	else if (y == (y->p)->left)
	{
		(y->p)->left = temp;
	}
	else
	{
		(y->p)->right = temp;
	}

	temp->right = y;
	y->p = temp;
}

void insert_fixup(ROOT *r, NODE *x);
void insert_node(ROOT *r, int key)
{
	NODE *n = (NODE *)malloc(sizeof(NODE));
	NODE *temp = r->r;
	NODE *p = r->nil;

	n->left = r->nil;
	n->right = r->nil;
	n->clr = RED;
	n->key = key;

	while (temp != r->nil)
	{
		p = temp;

		if (key > temp->key)
		{
			temp = temp->right;
		}
		else
		{
			temp = temp->left;
		}
	}

	n->p = p; 

	if (p == r->nil)
	{
		r->r = n;
	}
	else if (key > p->key)
	{
		p->right = n;
	}
	else
	{
		p->left = n;
	}

	insert_fixup(r, n);
}

void insert_fixup(ROOT *r, NODE *x)
{
	NODE *u = NULL;

	while ((x->p)->clr == RED)
	{
		if (x->p == (x->p->p)->left)
		{
			u = (x->p->p)->right;

			if (u->clr == RED)
			{
				(x->p)->clr = BLACK;
				u->clr = BLACK;

				(x->p->p)->clr = RED;

				x = (x->p->p);
			}
			else
			{
				if (x == (x->p)->right)
				{
					x = (x->p);

					left_rotate(r, x);
				}

				(x->p)->clr = BLACK;
				(x->p->p)->clr = RED;

				right_rotate(r, (x->p->p));
			}
		}
		else
		{
			u = (x->p->p)->left;

			if (u->clr == RED)
			{
				(x->p)->clr = BLACK;
				u->clr = BLACK;

				(x->p->p)->clr = RED;

				x = (x->p->p);
			}
			else
			{
				if (x == (x->p)->left)
				{
					x = (x->p);

					right_rotate(r, x);
				}

				(x->p)->clr = BLACK;
				(x->p->p)->clr = RED;

				left_rotate(r, (x->p->p));
			}
		}
	}

	(r->r)->clr = BLACK;
}

void tree_transplant(ROOT *r, NODE *t, NODE *c)
{
	if (t->p == r->nil)
	{
		r->r = c;
	}
	else if (t == (t->p)->left)
	{
		(t->p)->left = c;
	}
	else
	{
		(t->p)->right = c;
	}

	c->p = t->p;
}

int del, miss;

void delete_fixup(ROOT *r, NODE *x);
void delete_node(ROOT *r, int key)
{
	NODE *target = r->r; // the node that we want to remove.
	NODE *temp = NULL; // the node that is moved or removed.
	NODE *x = NULL; // temp's original position
	char t_clr;

	while (target != r->nil)
	{
		if (target->key > key)
		{
			target = target->left;
		}
		else if (target->key == key) {
			break;
		}
		else
		{
			target = target->right;
		}
	}
	if (target == r->nil) {
		miss++;
		return;
	}
	else {
		del++;
	}
	t_clr = target->clr;

	if (target->left == r->nil)
	{
		x = target->right;
		tree_transplant(r, target, target->right);
	}
	else if (target->right == r->nil)
	{
		x = target->left;
		tree_transplant(r, target, target->left);
	}
	else
	{
		temp = target->right;

		while (temp->left != r->nil)
		{
			temp = temp->left;
		}

		t_clr = temp->clr;
		x = temp->right;

		tree_transplant(r, temp, temp->right);
		temp->right = target->right;
		(temp->right)->p = temp;

		tree_transplant(r, target, temp);
		temp->left = target->left;
		(temp->left)->p = temp;
		temp->clr = target->clr;
	}

	if (t_clr == BLACK)
	{
		delete_fixup(r, x);
	}
	if (target == r->r) {
		r->r = r->nil;
	}

	free(target);
}

void delete_fixup(ROOT *r, NODE *x)
{
	NODE *s = NULL; // sibling node.

	while ((x != r->r) && (x->clr == BLACK))
	{
		if (x == (x->p)->left)
		{
			s = (x->p)->right;

			if (s->clr == RED)
			{
				s->clr = BLACK;
				(x->p)->clr = RED;
				left_rotate(r, x->p);
				s = (x->p)->right;
			}

			if ((s->left)->clr == BLACK && (s->right)->clr == BLACK)
			{
				s->clr = RED;
				x = x->p;
			}
			else if ((s->left)->clr == RED && (s->right)->clr == BLACK)
			{
				s->clr = RED;
				(s->left)->clr = BLACK;
				right_rotate(r, s);
				s = (x->p)->right;
			}

			if ((s->right)->clr == RED)
			{
				s->clr = (x->p)->clr;
				(s->right)->clr = BLACK;
				(x->p)->clr = BLACK;
				left_rotate(r, x->p);

				x = r->r;
			}

		}
		else
		{
			s = (x->p)->left;

			if (s->clr == RED)
			{
				s->clr = BLACK;
				(x->p)->clr = RED;
				right_rotate(r, x->p);
				s = (x->p)->left;
			}

			if ((s->left)->clr == BLACK && (s->right)->clr == BLACK)
			{
				s->clr = RED;
				x = x->p;
			}
			else if ((s->right)->clr == RED && (s->left)->clr == BLACK)
			{
				s->clr = RED;
				(s->right)->clr = BLACK;
				left_rotate(r, s);
				// update x's sibling
				s = (x->p)->left;
			}

			// case 4 : x's sibling s is black, s's left child is red.
			if ((s->left)->clr == RED)
			{
				s->clr = (x->p)->clr;
				(s->left)->clr = BLACK;
				(x->p)->clr = BLACK;
				right_rotate(r, x->p);

				x = r->r;
			}
		}
	}

	x->clr = BLACK;
}

int total = 0, nb = 0;
void inorder(ROOT* rbt, NODE *n, int p)
{
	if (n == rbt->nil) return;
	inorder(rbt, n->left, p);
	total++;
	if (n->clr == BLACK && (n->left != rbt->nil || n->right != rbt->nil)) {
		nb++;
	}
	if (p == 1) {
		printf("%d %c\n", n->key, n->clr==BLACK?'B':'R');
	}
	inorder(rbt, n->right, p);
}

int main()
{
	char fn[100];
	printf("filename = ");
	scanf("%s", fn);
	FILE *in = fopen(fn, "r");
	int num, insert = 0;
	ROOT *rbt = (ROOT*)malloc(sizeof(ROOT));
	create_nilnode(rbt);
	while (!feof(in)) {
		fscanf(in, "%d", &num);
		if (num == 0) break;
		if (num > 0) {
			insert++;
			insert_node(rbt, num);
		}
		else {
			delete_node(rbt, -num);
		}
	}
	fclose(in);
	
	inorder(rbt, rbt->r, 0);
	printf("filename=%s\n", fn);
	printf("total = %d\n", total);
	printf("insert = %d\n", insert);
	printf("deleted = %d\n", del);
	printf("miss = %d\n", miss);
	printf("nb = %d\n", nb);
	int bh = 0;
	NODE* n = rbt->r;
	while (n != rbt->nil) {
		if (n->clr == BLACK) {
			bh++;
		}
		n = n->right;
	}
	printf("bh = %d\n", bh);
	inorder(rbt, rbt->r, 1);
	return 0;
}
