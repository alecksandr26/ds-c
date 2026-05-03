#include <stdio.h>
#include <except/assert.h>
#include <mem.h>
#include "../include/ds.h"

void test_capacity_growth(void)
{
  VEC(int) vec = VEC_INIT(vec, 2);

  assert(VEC_CAPACITY(vec) == 2);
  assert(VEC_SIZE(vec) == 0);

  VEC_PUSH_BACK(vec, 2);
  VEC_PUSH_BACK(vec, 3);
  assert(VEC_CAPACITY(vec) == 4);

  VEC_PUSH_BACK(vec, 4);
  VEC_PUSH_BACK(vec, 5);
  assert(VEC_CAPACITY(vec) == 8);

  int x = VEC_POP_BACK(vec);
  x = VEC_POP_BACK(vec);
  assert(x == 4);
  assert(VEC_CAPACITY(vec) == 4);

  VEC_DESTROY(vec);
}

void test_basic_func(void)
{
  VEC(int) vec;
  VEC_INIT(vec, 100);
  assert(VEC_CAPACITY(vec) == 100);
  assert(vec.buff != NULL);
  assert(vec.size == 0);

  VEC_PUSH_BACK(vec, 20);
  int b = 10;
  VEC_PUSH_BACK(vec, b);
  assert(vec.size == 2);

  int c = VEC_POP_BACK(vec);
  assert(c == 10);
  assert(20 == VEC_GET(vec, 0));

  VEC_INSERT(vec, 0, 1);
  VEC_INSERT(vec, 0, 2);
  VEC_INSERT(vec, 0, 3);
  VEC_INSERT(vec, 0, 4);

  assert(4 == VEC_GET(vec, 0));
  assert(3 == VEC_GET(vec, 1));
  assert(2 == VEC_GET(vec, 2));

  VEC_DESTROY(vec);
}

void test_iterator_vec(void)
{
  VEC(int) vec = VEC_INIT(vec);
  int arr[] = {10, 12, 13};

  VEC_PUSH_BACK(vec, arr[0]);
  VEC_PUSH_BACK(vec, arr[1]);
  VEC_PUSH_BACK(vec, arr[2]);

  ITERATOR(int) it = VEC_BEGIN(vec, it);
  assert(ITERATOR_GET_DATA(it) == 10);

  ITERATOR_INC(it);
  assert(ITERATOR_GET_DATA(it) == 12);

  ITERATOR_INC(it);
  assert(ITERATOR_GET_DATA(it) == 13);

  ITERATOR_INC(it);
  assert(ITERATOR_IS_NULL(it) == true);
  assert(it.index >= vec.size);

  ITERATOR(int) end = VEC_END(vec, end);
  assert(ITERATOR_GET_DATA(end) == 13);
  assert(ITERATOR_CMP(end, it) < 0);

  VEC_BEGIN(vec, it);
  while (!ITERATOR_IS_NULL(it)) {
    assert(ITERATOR_GET_DATA(it) == arr[it.index]);
    ITERATOR_INC(it);
  }

  VEC_BEGIN(vec, it);
  while (ITERATOR_CMP(end, it) >= 0) {
    assert(ITERATOR_GET_DATA(it) == arr[it.index]);
    ITERATOR_INC(it);
  }

  VEC_DESTROY(vec);
}

void test_new_methods(void)
{
  VEC(int) vec = VEC_INIT(vec);

  VEC_PUSH_BACK(vec, 10);
  VEC_PUSH_BACK(vec, 11);
  VEC_PUSH_BACK(vec, 12);
  VEC_PUSH_BACK(vec, 13);

  int front = VEC_POP_FRONT(vec);
  assert(front == 10);
  assert(VEC_GET(vec, 0) == 11);

  VEC_PUSH_FRONT(vec, 9);
  assert(VEC_GET(vec, 0) == 9);

  VEC_DESTROY(vec);
}

void test_replace(void)
{
  VEC(int) vec = VEC_INIT(vec);

  VEC_PUSH_BACK(vec, 1);
  VEC_PUSH_BACK(vec, 2);
  VEC_PUSH_BACK(vec, 3);

  VEC_REPLACE(vec, 0, 99);
  assert(VEC_GET(vec, 0) == 99);

  VEC_REPLACE(vec, 1, 88);
  assert(VEC_GET(vec, 1) == 88);

  VEC_REPLACE(vec, 2, 77);
  assert(VEC_GET(vec, 2) == 77);

  assert(VEC_SIZE(vec) == 3);

  VEC_DESTROY(vec);
}

void test_remove(void)
{
  VEC(int) vec = VEC_INIT(vec);

  VEC_PUSH_BACK(vec, 10);
  VEC_PUSH_BACK(vec, 20);
  VEC_PUSH_BACK(vec, 30);
  VEC_PUSH_BACK(vec, 40);

  VEC_REMOVE(vec, 1);
  assert(VEC_SIZE(vec) == 3);
  assert(VEC_GET(vec, 0) == 10);
  assert(VEC_GET(vec, 1) == 30);
  assert(VEC_GET(vec, 2) == 40);

  VEC_REMOVE(vec, 0);
  assert(VEC_SIZE(vec) == 2);
  assert(VEC_GET(vec, 0) == 30);

  VEC_REMOVE(vec, 1);
  assert(VEC_SIZE(vec) == 1);
  assert(VEC_GET(vec, 0) == 30);

  VEC_DESTROY(vec);
}

void test_iterator_dec(void)
{
  VEC(int) vec = VEC_INIT(vec);

  VEC_PUSH_BACK(vec, 1);
  VEC_PUSH_BACK(vec, 2);
  VEC_PUSH_BACK(vec, 3);

  ITERATOR(int) it = VEC_END(vec, it);

  assert(ITERATOR_GET_DATA(it) == 3);
  ITERATOR_DEC(it);
  assert(ITERATOR_GET_DATA(it) == 2);
  ITERATOR_DEC(it);
  assert(ITERATOR_GET_DATA(it) == 1);

  VEC_DESTROY(vec);
}

void test_iterator_move_at(void)
{
  VEC(int) vec = VEC_INIT(vec);

  VEC_PUSH_BACK(vec, 10);
  VEC_PUSH_BACK(vec, 20);
  VEC_PUSH_BACK(vec, 30);
  VEC_PUSH_BACK(vec, 40);
  VEC_PUSH_BACK(vec, 50);

  ITERATOR(int) it = VEC_BEGIN(vec, it);

  ITERATOR_MOVE_AT(it, 2);
  assert(ITERATOR_GET_DATA(it) == 30);
  assert(it.index == 2);

  ITERATOR_MOVE_AT(it, 0);
  assert(ITERATOR_GET_DATA(it) == 10);
  assert(it.index == 0);

  ITERATOR_MOVE_AT(it, 4);
  assert(ITERATOR_GET_DATA(it) == 50);
  assert(it.index == 4);

  VEC_DESTROY(vec);
}

void test_iterator_get_at(void)
{
  VEC(int) vec = VEC_INIT(vec);

  VEC_PUSH_BACK(vec, 100);
  VEC_PUSH_BACK(vec, 200);
  VEC_PUSH_BACK(vec, 300);

  ITERATOR(int) it = VEC_GET_ITERATOR_AT(vec, it, 1);
  assert(ITERATOR_GET_DATA(it) == 200);
  assert(it.index == 1);

  ITERATOR(int) it2 = VEC_GET_ITERATOR_AT(vec, it2, 0);
  assert(ITERATOR_GET_DATA(it2) == 100);

  ITERATOR(int) it3 = VEC_GET_ITERATOR_AT(vec, it3, 2);
  assert(ITERATOR_GET_DATA(it3) == 300);

  VEC_DESTROY(vec);
}

void test_iterator_replace(void)
{
  VEC(int) vec = VEC_INIT(vec);

  VEC_PUSH_BACK(vec, 1);
  VEC_PUSH_BACK(vec, 2);
  VEC_PUSH_BACK(vec, 3);

  ITERATOR(int) it = VEC_BEGIN(vec, it);
  ITERATOR_REPLACE(it, 99);
  assert(VEC_GET(vec, 0) == 99);

  ITERATOR_INC(it);
  ITERATOR_REPLACE(it, 88);
  assert(VEC_GET(vec, 1) == 88);

  assert(VEC_SIZE(vec) == 3);

  VEC_DESTROY(vec);
}

void test_iterator_remove(void)
{
  VEC(int) vec = VEC_INIT(vec);

  VEC_PUSH_BACK(vec, 10);
  VEC_PUSH_BACK(vec, 20);
  VEC_PUSH_BACK(vec, 30);

  ITERATOR(int) it = VEC_BEGIN(vec, it);
  ITERATOR_INC(it);

  ITERATOR_REMOVE(it);
  assert(VEC_SIZE(vec) == 2);
  assert(ITERATOR_GET_DATA(it) == 30);

  ITERATOR_REMOVE(it);
  assert(VEC_SIZE(vec) == 1);
  assert(ITERATOR_IS_NULL(it));

  VEC_DESTROY(vec);
}

void test_iterator_insert(void)
{
  VEC(int) vec = VEC_INIT(vec);

  VEC_PUSH_BACK(vec, 10);
  VEC_PUSH_BACK(vec, 30);

  ITERATOR(int) it = VEC_BEGIN(vec, it);
  ITERATOR_INC(it);

  ITERATOR_INSERT(it, 20);
  assert(VEC_SIZE(vec) == 3);
  assert(VEC_GET(vec, 0) == 10);
  assert(VEC_GET(vec, 1) == 20);
  assert(VEC_GET(vec, 2) == 30);

  VEC_DESTROY(vec);
}

void test_iterator_cmp(void)
{
  VEC(int) vec = VEC_INIT(vec);

  VEC_PUSH_BACK(vec, 1);
  VEC_PUSH_BACK(vec, 2);
  VEC_PUSH_BACK(vec, 3);

  ITERATOR(int) a = VEC_BEGIN(vec, a);
  ITERATOR(int) b = VEC_BEGIN(vec, b);

  assert(ITERATOR_CMP(a, b) == 0);

  ITERATOR_INC(b);
  assert(ITERATOR_CMP(a, b) < 0);
  assert(ITERATOR_CMP(b, a) > 0);

  ITERATOR_INC(b);
  ITERATOR_INC(b);
  assert(ITERATOR_IS_NULL(b));
  assert(ITERATOR_CMP(b, a) > 0);

  VEC_DESTROY(vec);
}

void test_iterator_full_traversal_reverse(void)
{
  VEC(int) vec = VEC_INIT(vec);
  int arr[] = {1, 2, 3, 4, 5};
  int n = sizeof(arr) / sizeof(arr[0]);

  for (int i = 0; i < n; i++)
    VEC_PUSH_BACK(vec, arr[i]);

  ITERATOR(int) it = VEC_END(vec, it);

  for (int i = n - 1; i >= 0; i--) {
    assert(ITERATOR_GET_DATA(it) == arr[i]);
    if (i > 0) ITERATOR_DEC(it);
  }

  VEC_DESTROY(vec);
}

void test_single_element(void)
{
  VEC(int) vec = VEC_INIT(vec);

  VEC_PUSH_BACK(vec, 42);

  ITERATOR(int) it  = VEC_BEGIN(vec, it);
  ITERATOR(int) end = VEC_END(vec, end);

  assert(ITERATOR_GET_DATA(it)  == 42);
  assert(ITERATOR_GET_DATA(end) == 42);
  assert(ITERATOR_CMP(it, end)  == 0);

  ITERATOR_INC(it);
  assert(ITERATOR_IS_NULL(it));

  VEC_DESTROY(vec);
}

int main(void)
{
  test_basic_func();
  test_capacity_growth();
  test_iterator_vec();
  test_new_methods();
  test_replace();
  test_remove();
  test_iterator_dec();
  test_iterator_move_at();
  test_iterator_get_at();
  test_iterator_replace();
  test_iterator_remove();
  test_iterator_insert();
  test_iterator_cmp();
  test_iterator_full_traversal_reverse();
  test_single_element();
    
  return 0;
}
