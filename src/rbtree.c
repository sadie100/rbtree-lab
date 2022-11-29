#include "rbtree.h"

#include <stdbool.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *newnode = (node_t *)calloc(1, sizeof(node_t));
  newnode->color = RBTREE_BLACK;
  p->nil = newnode;
  p->root = newnode;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

//왼쪽으로 회전
// key가 위에서 왼쪽으로 내려가는 노드
void left_rotate(rbtree *t, node_t *key) {
  node_t *new_parent = key->right;
  node_t *new_son = new_parent->left;
  // 1. 조부모관계 정리
  if (key->parent->left == key) {
    // 만약 key가 왼쪽 자식이었다면
    new_parent->parent = key->parent;
    key->parent->left = new_parent;
  } else {
    new_parent->parent = key->parent;
    key->parent->right = new_parent;
  }
  // 2. 부모관계 정리
  key->parent = new_parent;
  new_parent->left = key;
  // 3. new_parent의 왼쪽 자식이었던 애 관계 정리
  key->right = new_son;
  new_son->parent = key;
}

//오른쪽으로 회전
// key가 위에서 오른쪽으로 내려가는 노드
void right_rotate(rbtree *t, node_t *key) {
  node_t *new_parent = key->left;
  node_t *new_son = new_parent->right;
  // 1. 조부모관계 정리
  if (key->parent->left == key) {
    // 만약 key가 왼쪽 자식이었다면
    new_parent->parent = key->parent;
    key->parent->left = new_parent;
  } else {
    new_parent->parent = key->parent;
    key->parent->right = new_parent;
  }
  // 2. 부모관계 정리
  key->parent = new_parent;
  new_parent->right = key;
  // 3. new_parent의 왼쪽 자식이었던 애 관계 정리
  key->left = new_son;
  new_son->parent = key;
}

void rbtree_insert_fixup(rbtree *t, node_t *key) {
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
         노드 색을 switch
         2) key와 key의 부모와 key의 조부모가 triangle인 경우
          => (1) key가 한 층 올라가도록 key를 중심으로 회전하고, 포인터를 내려간
         부모로 옮긴다.
         (2) linear인 경우와 같게 되었으므로, 해당 경우의 연산을 수행한다.
         (key의 부모를 중심으로 반대 방향으로 회전시키고, 부모와 조부모의
         노드 색을 switch)
        */
        // 현재는 triangle인 경우
        // 1번 수행
        key = key->parent;
        left_rotate(t, key);
        // 2번 수행
        right_rotate(t, key->parent->parent);
        key->parent->color = RBTREE_BLACK;
        key->parent->parent->color = RBTREE_RED;
      } else {
        // linear인 경우
        right_rotate(t, key->parent->parent);
        key->parent->color = RBTREE_BLACK;
        key->parent->parent->color = RBTREE_RED;
      }
    } else {
      //부모가 오른쪽 자식일 경우
      node_t *uncle = key->parent->parent->left;
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
      } else if (key == key->parent->left) {
        /*
         삼촌이 블랙이면 => 두 경우로 나눠서 연산
         1) key와 key의 부모와 key의 조부모가 linear한 경우
          => key의 부모를 중심으로 반대 방향으로 회전시키고, 부모와 조부모의
         노드 색을 switch
         2) key와 key의 부모와 key의 조부모가 triangle인 경우
          => (1) key가 한 층 올라가도록 key를 중심으로 회전하고, 포인터를 내려간
         부모로 옮긴다.
         (2) linear인 경우와 같게 되었으므로, 해당 경우의 연산을 수행한다.
         (key의 부모를 중심으로 반대 방향으로 회전시키고, 부모와 조부모의
         노드 색을 switch)
        */
        // 현재는 triangle인 경우
        // 1번 수행
        key = key->parent;
        right_rotate(t, key);
        // 2번 수행
        left_rotate(t, key->parent->parent);
        key->parent->color = RBTREE_BLACK;
        key->parent->parent->color = RBTREE_RED;
      } else {
        // linear인 경우
        left_rotate(t, key->parent->parent);
        key->parent->color = RBTREE_BLACK;
        key->parent->parent->color = RBTREE_RED;
      }
    }
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *y = t->nil;
  node_t *x = t->root;
  node_t *keynode = (node_t *)calloc(1, sizeof(node_t));
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
  node_t *now = t->root;
  node_t *result = (node_t *)calloc(1, sizeof(node_t));
  while (true) {
    if (now == t->nil) {
      result = NULL;
      break;
    }
    if (now->key == key) {
      result = now;
      break;
    }
    if (now->key > key) {
      now = now->left;
    } else {
      now = now->right;
    }
  }
  return result;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *now = (node_t *)calloc(1, sizeof(node_t));

  while (true) {
    if (now->left == t->nil) {
      break;
    } else {
      now = now->left;
    }
  }
  return now;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *now = (node_t *)calloc(1, sizeof(node_t));

  while (true) {
    if (now->right == t->nil) {
      break;
    } else {
      now = now->right;
    }
  }
  return now;
}

void transplant(rbtree *t, node_t *origin, node_t *changing) {
  // origin의 parent관계에 changing을 이식하기
  if (origin->parent == t->nil) {
    t->root = changing;
  } else if (origin == origin->parent->left) {
    origin->parent->left = changing;
  } else {
    origin->parent->right = changing;
  }

  changing->parent = origin->parent;
}

void erase_fixup(rbtree *t, node_t *x) {
  // erase rbtree 추가연산
  // x는 새로 들어온 자식 노드, 혹은 nil
  while (x != t->root && x->color == RBTREE_BLACK) {
    node_t *w = (node_t *)calloc(1, sizeof(node_t));
    if (x == x->parent->left) {
      // x가 왼쪽 자식일 경우
      w = x->parent->right;
      if (w->color == RBTREE_RED) {
        // w가 red일 경우. case 1
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      } else if (w->left->color == RBTREE_BLACK &&
                 w->right->color == RBTREE_BLACK) {
        // w가 black이고 w의 자식들이 모두 black. case 2
        w->color = RBTREE_RED;
        // x->parent->color = RBTREE_Black; 이거 없어도 되나?
        x = x->parent;
      } else if (w->right->color == RBTREE_BLACK) {
        // w의 왼쪽 자식이 red. case 3
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        right_rotate(t, w);
        w = x->parent->right;
      } else if (w->left->color == RBTREE_BLACK) {
        // w의 오른쪽 자식이 red. case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    } else {
      // 방향 바꾼거... x가 오른쪽 자식일 경우
      w = x->parent->left;
      if (w->color == RBTREE_RED) {
        // case 1
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      } else if (w->right->color == RBTREE_BLACK &&
                 w->left->color == RBTREE_BLACK) {
        // w의 자식들이 모두 black. case 2
        w->color = RBTREE_RED;
        x = x->parent;
      } else if (w->left->color == RBTREE_BLACK) {
        // w의 왼쪽 자식이 red. case 3
        w->right->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        left_rotate(t, w);
        w = x->parent->left;
      } else if (w->right->color == RBTREE_BLACK) {
        // w의 오른쪽 자식이 red. case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // p는 실제로 지우는 노드
  // x는 자식노드
  node_t *y = p;
  color_t y_origin_color = y->color;
  node_t *x = (node_t *)calloc(1, sizeof(node_t));
  if (p->left == t->nil) {
    x = p->right;
    transplant(t, p, p->right);
  } else if (p->right == t->nil) {
    x = p->left;
    transplant(t, p, p->left);
  } else {
    y = p->right;
    while (true) {
      if (p->left == t->nil) {
        break;
      }
      y = p->left;
    }
    y_origin_color = y->color;
    x = y->right;
    if (y->parent == p) {
      x->parent = y;
    } else {
      transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
    if (y_origin_color == RBTREE_BLACK) {
      erase_fixup(t, x);
    }
  }
  return 0;
}

void inorder_tree_walk(const rbtree *t, key_t *arr, node_t *x, int *idx,
                       size_t n) {
  if (x == t->nil) {
    return;
  }
  inorder_tree_walk(t, arr, x->left, idx, n);

  if (*idx < n) {
    arr[*idx] = x->key;
  }
  inorder_tree_walk(t, arr, x->right, idx, n);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *now = t->root;
  int *idx = 0;
  inorder_tree_walk(t, arr, now, idx, n);

  return 0;
}
