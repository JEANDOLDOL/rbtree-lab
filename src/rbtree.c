#include "rbtree.h"

#include <stdlib.h>
#define SENTINEL

rbtree *new_rbtree(void)
{
  // tree 구조체 동적 할당
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));

  // nil 노드 생성 및 초기화
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK; // nil 노드는 항상 BLACK

  // tree의 nil과 root를 nil 노드로 설정 (tree가 빈 경우 root는 nil노드여야 한다.)
  t->nil = nil;
  t->root = nil;

  return t;
}

void delete_rbtree_recursion(rbtree *t, node_t *x)
{
  if (x == t->nil)
  {
    return;
  }

  delete_rbtree_recursion(t, x->left);
  delete_rbtree_recursion(t, x->right);

  free(x);
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory

  delete_rbtree_recursion(t, t->root);
  free(t->nil);
  free(t);
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  {
    t->root = v;
  }
  else if (u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rotate_left(rbtree *t, node_t *x)
{
  node_t *y;
  // y를 설정.
  y = x->right;
  // y의 왼쪽 서브 트리를 x의 오른쪽 서브트리로 옮긴다.
  x->right = y->left;
  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  // x 의 부모를 y로 연결한다.
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  // x를 y의 왼쪽으로 놓는다.
  y->left = x;
  x->parent = y;
}

void rotate_right(rbtree *t, node_t *y)
{
  node_t *x;
  // x를 설정
  x = y->left;
  // x의 오른쪽 서브트리를 y의 왼쪽 서브트리로 옮긴다.
  y->left = x->right;
  if (x->right != t->nil)
  {
    x->right->parent = y;
  }
  // y의 부모를 x로 연결한다.
  x->parent = y->parent;
  if (y->parent == t->nil)
  {
    t->root = x;
  }
  else if (y == y->parent->left)
  {
    y->parent->left = x;
  }
  else
  {
    y->parent->right = x;
  }
  // y를 x의 오른쪽으로 놓는다.
  x->right = y;
  y->parent = x;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  node_t *z = (node_t *)malloc(sizeof(node_t));
  z->key = key;
  node_t *y, *x;
  y = t->nil;
  x = t->root;
  while (x != t->nil)
  {
    y = x;
    if (z->key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  z->parent = y;
  if (y == t->nil)
  {
    t->root = z;
  }
  else if (z->key < y->key)
  {
    y->left = z;
  }
  else
  {
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t, z);
  return z;
}

void rbtree_insert_fixup(rbtree *t, node_t *z)
{
  while (z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED)
      {
        // Case 1 z의 엉클은 RED
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        // Case 2: z의 엉클은 BLACK, z는 오른쪽 자식
        if (z == z->parent->right)
        {
          z = z->parent;
          rotate_left(t, z);
        }
        // Case 3: z의 엉클은 BLACK, z는 왼쪽 자식
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rotate_right(t, z->parent->parent);
      }
    }
    else
    {
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        // Case 1 z의 엉클은 RED
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        // Case 2: z의 엉클은 BLACK, z는 오른쪽 자식
        if (z == z->parent->left)
        {
          z = z->parent;
          rotate_right(t, z);
        }
        // Case 3: z의 엉클은 BLACK, z는 왼쪽 자식
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rotate_left(t, z->parent->parent);
      }
    }
  }
  // 루트노드는 항상 검정
  t->root->color = RBTREE_BLACK;
}
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *x = t->root;
  while (x != t->nil)
  {
    if (x->key == key)
    {
      return x;
    }
    if (key < x->key)
    {
      x = x->left;
    }
    else if (key > x->key)
    {
      x = x->right;
    }
  }
  return NULL;
}
node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *x = t->root;
  while (x->left != t->nil)
  {
    x = x->left;
  }
  return x;
}

node_t *rbtree_min_node(const rbtree *t, node_t *x)
{
  // TODO: implement find
  while (x->left != t->nil)
  {
    x = x->left;
  }
  return x;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  node_t *x = t->root;
  while (x->right != t->nil)
  {
    x = x->right;
  }
  return x;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  node_t *y = p;
  node_t *x;
  color_t y_original_color = y->color;
  if (p->left == t->nil)
  {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else
  {
    y = rbtree_min_node(t, p->right);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == p)
    {
      x->parent = y;
    }
    else
    {
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  free(p);
  if (y_original_color == RBTREE_BLACK)
  {
    rbtree_erase_fixup(t, x);
  }
  return 0;
}

void rbtree_erase_fixup(rbtree *t, node_t *x)
{
  node_t *w;
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      w = x->parent->right;
      if (w->color == RBTREE_RED)
      { // case 1
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotate_left(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      { // case 2
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->right->color == RBTREE_BLACK)
        { // case 3
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_right(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color; // case 4
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        rotate_left(t, x->parent);
        x = t->root;
      }
    }
    else
    {
      w = x->parent->left;
      if (w->color == RBTREE_RED)
      { // case 1
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rotate_right(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      { // case 2
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        { // case 3
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rotate_left(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color; // case 4
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rotate_right(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  node_t *cur = t->root;
  size_t *index;
  size_t x = 0;
  index = &x;

  inorder(t, arr, n, index, cur);
  return 0;
}

void inorder(const rbtree *t, key_t *arr, const size_t n, size_t *index, node_t *cur)
{

  if (cur == t->nil)
  {
    return;
  }

  inorder(t, arr, n, index, cur->left);
  if (*index < n)
  {
    arr[*index] = cur->key;
    *index = *index + 1;
  }
  else
  {
    return;
  }
  inorder(t, arr, n, index, cur->right);
}
