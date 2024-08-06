#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1,sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->root = nil;
  p->nil = nil;

  return p;
}

void delete_rbtree_recursion(rbtree *t, node_t *x){
  x = t->root;
  if(x == t->nil){
    return;
  }
  if(x->left != t->nil){
    delete_rbtree_recursion(t,x->left);
  }
  if(x->right != t->nil){
    delete_rbtree_recursion(t,x->right);
  }
  if(x->left == t->nil && x->right == t->nil){
    // x->parent->좌우를 nil 로 해줘야 하나?
    free(x);
    return;
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *x;
  x = t->root;
  delete_rbtree_recursion(t,x);
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil){
    t->root = v;
  }
  else if(u == u->parent->left){
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rotate_left(rbtree *t, node_t *x){
  node_t *y;
  // y를 설정.
  y = x->right;
  // y의 왼쪽 서브 트리를 x의 오른쪽 서브트리로 옮긴다.
  x->right = y->left;
  if(y->left != t->nil){
    y->left->parent = x;
  }
  //x 의 부모를 y로 연결한다.
  y->parent = x->parent;
  if(x->parent == t->nil){
    t->root = y;
  }
  else if( x == x->parent->left){
    x->parent->left = y;
  }
  else{
    x->parent->right = y;
  }
  // x를 y의 왼쪽으로 놓는다.
  y->left = x;
  x->parent = y;
}

void rotate_right(rbtree *t, node_t *y){
  node_t *x;
  // x를 설정
  x = y->left;
  // x의 오른쪽 서브트리를 y의 왼쪽 서브트리로 옮긴다.
  y->left = x->right;
  if(x->right->parent != t->nil){
    x->right->parent = y;
  }
  //y의 부모를 x로 연결한다.
  x->parent = y->parent;
  if(y->parent == t->nil){
    t->root = x;
  }
  else if(y == y->parent->left){
    y->parent->left = x;
  }
  else{
    y->parent->right = x;
  }
  // y를 x의 오른쪽으로 놓는다.
  x->right = y;
  y->parent = x;
}

node_t *rbtree_insert(rbtree *t,  const key_t key) {
  // TODO: implement insert
  node_t *z = (node_t *)malloc(sizeof(node_t));
  z->key = key;
  node_t *y, *x;
  y = t->nil;
  x = t->root;
  while (x != t->nil){
    y = x;
    if (z->key < x->key){
      x = x->left;
    }
    else{
      x = x->right;
    }
  }
  z->parent = y;
  if(y == t->nil){
    t->root = z;
  }
  else if(z->key < y->key){
    y->left = z;
  }
  else{
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t,z);
  return t->root;
}

void rbtree_insert_fixup(rbtree *t,node_t *z){
  // 부모의 색이 RED일 때
  while (z->parent->color == RBTREE_RED){
    if (z->parent == z->parent->parent->left){
      node_t *y = z->parent->parent->right;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else if(z == z->parent->right){
        z = z->parent;
        rotate_left(t,z);
      }
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      rotate_right(t,z->parent->parent);
    }
    else{
      node_t *y = z->parent->parent->left;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else if(z == z->parent->left){
        z = z->parent;
        rotate_right(t,z);
      }
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      rotate_left(t,z->parent->parent);
    }
  }
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t->root;
  while(x != t->nil){
    if(x->key == key){
      return x;
    }
    if(key < x->key){
      x = x->left;
    }
    else if(key > x->key){
      x = x->right;
    }
  }
  return NULL;
}
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *r = t->root;
  if (r == t->nil){
    r->key = 32767;
    return r;
  }
  while(r->left != t->nil){
    r = r->left;
  }
  return r;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *r;
  r = t->root;
  if (r == t->nil){
    r->key = 0;
    return r;
  }
  while (r->right != t->nil){
    r = r->right;
  }
  return r;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
///////////////MR.ko//////////////////////////////
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// new_rbtree should return rbtree struct with null root node
void test_init(void)
{
  rbtree *t = new_rbtree();
  assert(t != NULL);
  assert(t->nil != NULL);
  assert(t->root == t->nil);
  delete_rbtree(t);
}

// root node should have proper values and pointers
void test_insert_single(const key_t key)
{
  rbtree *t = new_rbtree();
  node_t *p = rbtree_insert(t, key);
  assert(p != NULL);
  assert(t->root == p);
  assert(p->key == key);
  // assert(p->color == RBTREE_BLACK);  // color of root node should be black

  assert(p->left == t->nil);
  assert(p->right == t->nil);
  assert(p->parent == t->nil);
  delete_rbtree(t);
}

// find should return the node with the key or NULL if no such node exists
void test_find_single(const key_t key, const key_t wrong_key)
{
  rbtree *t = new_rbtree();
  node_t *p = rbtree_insert(t, key);

  node_t *q = rbtree_find(t, key);
  assert(q != NULL);
  assert(q->key == key);
  assert(q == p);

  q = rbtree_find(t, wrong_key);
  assert(q == NULL);

  delete_rbtree(t);
}

// erase should delete root node
void test_erase_root(const key_t key)
{
  rbtree *t = new_rbtree();
  node_t *p = rbtree_insert(t, key);
  assert(p != NULL);
  assert(t->root == p);
  assert(p->key == key);

  rbtree_erase(t, p);
  assert(t->root == t->nil);

  delete_rbtree(t);
}

static void insert_arr(rbtree *t, const key_t *arr, const size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    rbtree_insert(t, arr[i]);
  }
}

static int comp(const void *p1, const void *p2)
{
  const key_t *e1 = (const key_t *)p1;
  const key_t *e2 = (const key_t *)p2;
  if (*e1 < *e2)
  {
    return -1;
  }
  else if (*e1 > *e2)
  {
    return 1;
  }
  else
  {
    return 0;
  }
};

// min/max should return the min/max value of the tree
void test_minmax(key_t *arr, const size_t n)
{
  // null array is not allowed
  assert(n > 0 && arr != NULL);

  rbtree *t = new_rbtree();
  assert(t != NULL);

  insert_arr(t, arr, n);
  assert(t->root != NULL);
#ifdef SENTINEL
  assert(t->root != t->nil);
#endif

  qsort((void *)arr, n, sizeof(key_t), comp);
  node_t *p = rbtree_min(t);
  assert(p != NULL);
  assert(p->key == arr[0]);

  node_t *q = rbtree_max(t);
  assert(q != NULL);
  assert(q->key == arr[n - 1]);

  rbtree_erase(t, p);
  p = rbtree_min(t);
  assert(p != NULL);
  assert(p->key == arr[1]);

  if (n >= 2)
  {
    rbtree_erase(t, q);
    q = rbtree_max(t);
    assert(q != NULL);
    assert(q->key == arr[n - 2]);
  }

  delete_rbtree(t);
}

void test_to_array(rbtree *t, const key_t *arr, const size_t n)
{
  assert(t != NULL);

  insert_arr(t, arr, n);
  qsort((void *)arr, n, sizeof(key_t), comp);

  key_t *res = calloc(n, sizeof(key_t));
  rbtree_to_array(t, res, n);
  for (int i = 0; i < n; i++)
  {
    assert(arr[i] == res[i]);
  }
  free(res);
}

void test_multi_instance()
{
  rbtree *t1 = new_rbtree();
  assert(t1 != NULL);
  rbtree *t2 = new_rbtree();
  assert(t2 != NULL);

  key_t arr1[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n1 = sizeof(arr1) / sizeof(arr1[0]);
  insert_arr(t1, arr1, n1);
  qsort((void *)arr1, n1, sizeof(key_t), comp);

  key_t arr2[] = {4, 8, 10, 5, 3};
  const size_t n2 = sizeof(arr2) / sizeof(arr2[0]);
  insert_arr(t2, arr2, n2);
  qsort((void *)arr2, n2, sizeof(key_t), comp);

  key_t *res1 = calloc(n1, sizeof(key_t));
  rbtree_to_array(t1, res1, n1);
  for (int i = 0; i < n1; i++)
  {
    assert(arr1[i] == res1[i]);
  }

  key_t *res2 = calloc(n2, sizeof(key_t));
  rbtree_to_array(t2, res2, n2);
  for (int i = 0; i < n2; i++)
  {
    assert(arr2[i] == res2[i]);
  }

  free(res2);
  free(res1);
  delete_rbtree(t2);
  delete_rbtree(t1);
}

// Search tree constraint
// The values of left subtree should be less than or equal to the current node
// The values of right subtree should be greater than or equal to the current
// node

static bool search_traverse(const node_t *p, key_t *min, key_t *max,
                            node_t *nil)
{
  if (p == nil)
  {
    return true;
  }

  *min = *max = p->key;

  key_t l_min, l_max, r_min, r_max;
  l_min = l_max = r_min = r_max = p->key;

  const bool lr = search_traverse(p->left, &l_min, &l_max, nil);
  if (!lr || l_max > p->key)
  {
    return false;
  }
  const bool rr = search_traverse(p->right, &r_min, &r_max, nil);
  if (!rr || r_min < p->key)
  {
    return false;
  }

  *min = l_min;
  *max = r_max;
  return true;
}

void test_search_constraint(const rbtree *t)
{
  assert(t != NULL);
  node_t *p = t->root;
  key_t min, max;
#ifdef SENTINEL
  node_t *nil = t->nil;
#else
  node_t *nil = NULL;
#endif
  assert(search_traverse(p, &min, &max, nil));
}

// Color constraint
// 1. Each node is either red or black. (by definition)
// 2. All NIL nodes are considered black.
// 3. A red node does not have a red child.
// 4. Every path from a given node to any of its descendant NIL nodes goes
// through the same number of black nodes.

bool touch_nil = false;
int max_black_depth = 0;

static void init_color_traverse(void)
{
  touch_nil = false;
  max_black_depth = 0;
}

static bool color_traverse(const node_t *p, const color_t parent_color,
                           const int black_depth, node_t *nil)
{
  if (p == nil)
  {
    if (!touch_nil)
    {
      touch_nil = true;
      max_black_depth = black_depth;
    }
    else if (black_depth != max_black_depth)
    {
      return false;
    }
    return true;
  }
  if (parent_color == RBTREE_RED && p->color == RBTREE_RED)
  {
    return false;
  }
  int next_depth = ((p->color == RBTREE_BLACK) ? 1 : 0) + black_depth;
  return color_traverse(p->left, p->color, next_depth, nil) &&
         color_traverse(p->right, p->color, next_depth, nil);
}

void test_color_constraint(const rbtree *t)
{
  assert(t != NULL);
  node_t *nil = t->nil;
  node_t *p = t->root;
  assert(p == nil || p->color == RBTREE_BLACK);

  init_color_traverse();
  assert(color_traverse(p, RBTREE_BLACK, 0, nil));
}

// rbtree should keep search tree and color constraints
void test_rb_constraints(const key_t arr[], const size_t n)
{
  rbtree *t = new_rbtree();
  assert(t != NULL);

  insert_arr(t, arr, n);
  assert(t->root != NULL);

  test_color_constraint(t);
  test_search_constraint(t);

  delete_rbtree(t);
}

// rbtree should manage distinct values
void test_distinct_values()
{
  const key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_rb_constraints(entries, n);
}

// rbtree should manage values with duplicate
void test_duplicate_values()
{
  const key_t entries[] = {10, 5, 5, 34, 6, 23, 12, 12, 6, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_rb_constraints(entries, n);
}

void test_minmax_suite()
{
  key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_minmax(entries, n);
}

void test_to_array_suite()
{
  rbtree *t = new_rbtree();
  assert(t != NULL);

  key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_to_array(t, entries, n);

  delete_rbtree(t);
}

void test_find_erase(rbtree *t, const key_t *arr, const size_t n)
{
  for (int i = 0; i < n; i++)
  {
    node_t *p = rbtree_insert(t, arr[i]);
    // printf("arr[%d] = %d\n", i, arr[i]);
    // printf("%d\n", p->key);
    assert(p != NULL);
  }

  for (int i = 0; i < n; i++)
  {
    node_t *p = rbtree_find(t, arr[i]);
    printf("arr[%d] = %d\n", i, arr[i]);
    assert(p != NULL);
    assert(p->key == arr[i]);
    rbtree_erase(t, p);
  }

  for (int i = 0; i < n; i++)
  {
    node_t *p = rbtree_find(t, arr[i]);
    assert(p == NULL);
  }

  for (int i = 0; i < n; i++)
  {
    node_t *p = rbtree_insert(t, arr[i]);
    assert(p != NULL);
    node_t *q = rbtree_find(t, arr[i]);
    assert(q != NULL);
    assert(q->key == arr[i]);
    assert(p == q);
    rbtree_erase(t, p);
    q = rbtree_find(t, arr[i]);
    assert(q == NULL);
  }
}

void test_find_erase_fixed()
{
  const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(arr) / sizeof(arr[0]);
  rbtree *t = new_rbtree();
  assert(t != NULL);

  test_find_erase(t, arr, n);

  delete_rbtree(t);
}

void test_find_erase_rand(const size_t n, const unsigned int seed)
{
  srand(seed);
  rbtree *t = new_rbtree();
  key_t *arr = calloc(n, sizeof(key_t));
  for (int i = 0; i < n; i++)
  {
    arr[i] = rand();
  }

  test_find_erase(t, arr, n);

  free(arr);
  delete_rbtree(t);
}

int main(void)
{
  test_init();
  printf("test_init end\n");
  test_insert_single(1024);
  printf("test_insert_single end\n");
  test_find_single(512, 1024);
  printf("test_find_single end\n");
  test_erase_root(128);
  printf("test_erase_root end\n");
  test_find_erase_fixed();
  printf("test_find_erase_fixed end\n");
  test_minmax_suite();
  printf("test_minmax_suite end\n");
  test_to_array_suite();
  printf("test_to_array_suite end\n");
  test_distinct_values();
  printf("test_distinct_values end\n");
  test_duplicate_values();
  printf("test_duplicate_values end\n");
  test_multi_instance();
  printf("test_multi_instance end\n");
  test_find_erase_rand(10000, 17);
  printf("Passed all tests!\n");
}