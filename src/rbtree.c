#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *newnode;
  newnode->color = RBTREE_BLACK;
  newnode->key = NULL;
  newnode->parent = NULL;
  newnode->left = NULL;
  newnode->right = NULL;
  p->nil = newnode;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void left_rotate(rbtree *t, const key_t key) {
  //
}

node_t *rbtree_insert_fixup(rbtree *t, node_t *key) {
  // insert fixup logic
  while (key->parent->color == RBTREE_RED) {
    //부모가 red일 때만 추가 연산을 한다.
    if (key->parent == key->parent->parent->left) {
      //부모가 왼쪽자식일 경우
      node_t *uncle = key->parent->parent->right;
      //삼촌 노드 참조
      if (uncle->color == RBTREE_RED) {
        //삼촌도 레드면 => 조부모를 red로, 부모와 삼촌을 black으로 조정
        key->parent->color = RBTREE_BLACK;
        //부모를 블랙으로
        uncle->color = RBTREE_BLACK;
        //삼촌 색 블랙으로
        key->parent->parent->color = RBTREE_RED;
        //조부모 레드로
        key = key->parent->parent;
        //포인터를 조부모로 옮김
      } else if (key == key->parent->right) {
        /*
         삼촌이 블랙이면 => 두 경우로 나눠서 연산
         1) key와 key의 부모와 key의 조부모가 linear한 경우
          => key의 부모를 중심으로 반대 방향으로 회전시키고, 부모와 조부모의
         노드 색을 switch 2) key와 key의 부모와 key의 조부모가 triangle인 경우
          => (1) key가 한 층 올라가도록 key를 중심으로 회전하고, 포인터를 내려간
         부모로 옮긴다. (2) 위 1을 수행하면 linear인 경우와 같은 상태가 되므로
         1만 구현하고 while에 돌아가도록 함
        */
        key = key->parent;
        left_rotate(t, key);
        key->parent.color = RBTREE_BLACK;
        key->parent->parent.color = RBTREE_RED;
        right_rotate(t, key->parent->parent);
      } else {
        // left - right 반전해서 하기~~~~~
      }
    }
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y = t->nil;
  node_t *x = t->root;
  node_t *keynode;
  keynode->key = key;
  while (x != t->nil) {
    y = x;
    if (key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  keynode->parent = y;
  if (y == t->nil) {
    t->root = keynode;
  } else if (key < y->key) {
    y->left = keynode;
  } else {
    y->right = keynode;
  }
  keynode->left = t->nil;
  keynode->right = t->nil;
  keynode->color = RBTREE_RED;
  rbtree_insert_fixup(t, keynode);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
