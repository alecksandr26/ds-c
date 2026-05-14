#include <stdio.h>
#include <except/assert.h>
#include "../include/ds/linked.h"

/* ── Basic init ──────────────────────────────────────────────────────────── */

void test_init(void)
{
  typedef struct {
    char name[100];
    int age;
  } Person;

  DEFINE_LINKED_LIST_TYPE(Person);

  LINKED_LIST(Person) list = LINKED_LIST_INIT(list);
  assert(list.type_size == sizeof(Person));
  assert(list.head == NULL);
  assert(list.tail == NULL);
  assert(list.size == 0);

  LINKED_LIST(int) list_int = LINKED_LIST_INIT(list_int);
  assert(list_int.type_size == sizeof(int));
  assert(list_int.size == 0);
  assert(list_int.head == NULL);
  assert(list_int.tail == NULL);
}

/* ── Push / pop / front / back ───────────────────────────────────────────── */

void test_push_pop(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);

  LINKED_LIST_PUSH_BACK(list, 10);
  LINKED_LIST_PUSH_BACK(list, 12);
  LINKED_LIST_PUSH_BACK(list, 13);

  assert(LINKED_LIST_SIZE(list) == 3);
  assert(LINKED_LIST_FRONT(list) == 10);
  assert(LINKED_LIST_BACK(list)  == 13);

  LINKED_LIST_PUSH_FRONT(list, 20);
  assert(LINKED_LIST_FRONT(list) == 20);
  assert(LINKED_LIST_SIZE(list)  == 4);

  assert(LINKED_LIST_POP_BACK(list) == 13);
  assert(LINKED_LIST_SIZE(list)     == 3);
  assert(LINKED_LIST_BACK(list)     == 12);

  assert(LINKED_LIST_POP_FRONT(list) == 20);
  assert(LINKED_LIST_SIZE(list)      == 2);
  assert(LINKED_LIST_FRONT(list)     == 10);

  LINKED_LIST_DESTROY(list);
}

/* ── Single element edge case ────────────────────────────────────────────── */

void test_single_element(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);

  LINKED_LIST_PUSH_BACK(list, 42);
  assert(LINKED_LIST_SIZE(list)  == 1);
  assert(LINKED_LIST_FRONT(list) == 42);
  assert(LINKED_LIST_BACK(list)  == 42);

  assert(LINKED_LIST_POP_BACK(list) == 42);
  assert(LINKED_LIST_SIZE(list)     == 0);

  LINKED_LIST_PUSH_FRONT(list, 99);
  assert(LINKED_LIST_SIZE(list)  == 1);
  assert(LINKED_LIST_FRONT(list) == 99);

  assert(LINKED_LIST_POP_FRONT(list) == 99);
  assert(LINKED_LIST_SIZE(list)      == 0);

  LINKED_LIST_DESTROY(list);
}

/* ── Clear ───────────────────────────────────────────────────────────────── */

void test_clear(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);

  LINKED_LIST_PUSH_BACK(list, 1);
  LINKED_LIST_PUSH_BACK(list, 2);
  LINKED_LIST_PUSH_BACK(list, 3);
  assert(LINKED_LIST_SIZE(list) == 3);

  LINKED_LIST_CLEAR(list);
  assert(LINKED_LIST_SIZE(list) == 0);
  assert(list.head              == NULL);
  assert(list.tail              == NULL);

  /* Must be usable again after clear */
  LINKED_LIST_PUSH_BACK(list, 10);
  assert(LINKED_LIST_SIZE(list)  == 1);
  assert(LINKED_LIST_FRONT(list) == 10);

  LINKED_LIST_DESTROY(list);
}

/* ── Destroy ─────────────────────────────────────────────────────────────── */

void test_destroy(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);

  LINKED_LIST_PUSH_BACK(list, 1);
  LINKED_LIST_PUSH_BACK(list, 2);
  LINKED_LIST_PUSH_BACK(list, 3);

  LINKED_LIST_DESTROY(list);
  assert(list.state == DS_STATE_DESTROYED);
  assert(list.size  == 0);
  assert(list.head  == NULL);
  assert(list.tail  == NULL);
}

/* ── Iterator forward traversal ──────────────────────────────────────────── */

void test_iterator_forward(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);
  int arr[] = {10, 12, 13};

  LINKED_LIST_PUSH_BACK(list, arr[0]);
  LINKED_LIST_PUSH_BACK(list, arr[1]);
  LINKED_LIST_PUSH_BACK(list, arr[2]);

  ITERATOR(int) it = LINKED_LIST_BEGIN(list, it);
  assert(ITERATOR_GET_DATA(it) == 10);

  ITERATOR_INC(it);
  assert(it.index == 1);
  assert(ITERATOR_GET_DATA(it) == 12);

  ITERATOR_INC(it);
  assert(it.index == 2);
  assert(ITERATOR_GET_DATA(it) == 13);

  ITERATOR_INC(it);
  assert(ITERATOR_IS_NULL(it));
  assert(it.index == list.size);

  LINKED_LIST_DESTROY(list);
}

/* ── Iterator backward traversal ────────────────────────────────────────── */

void test_iterator_backward(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);
  int arr[] = {10, 12, 13};

  LINKED_LIST_PUSH_BACK(list, arr[0]);
  LINKED_LIST_PUSH_BACK(list, arr[1]);
  LINKED_LIST_PUSH_BACK(list, arr[2]);

  ITERATOR(int) it = LINKED_LIST_END(list, it);
  assert(ITERATOR_GET_DATA(it) == 13);

  ITERATOR_DEC(it);
  assert(it.index == 1);
  assert(ITERATOR_GET_DATA(it) == 12);

  ITERATOR_DEC(it);
  assert(it.index == 0);
  assert(ITERATOR_GET_DATA(it) == 10);
  assert(it.index_ptr == list.tail);

  LINKED_LIST_DESTROY(list);
}

/* ── Iterator cmp loop ───────────────────────────────────────────────────── */

void test_iterator_cmp_loop(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);
  int arr[] = {10, 12, 13};

  LINKED_LIST_PUSH_BACK(list, arr[0]);
  LINKED_LIST_PUSH_BACK(list, arr[1]);
  LINKED_LIST_PUSH_BACK(list, arr[2]);

  ITERATOR(int) it  = LINKED_LIST_BEGIN(list, it);
  ITERATOR(int) end = LINKED_LIST_END(list, end);

  uint32_t count = 0;
  while (ITERATOR_CMP(end, it) >= 0) {
    assert(ITERATOR_GET_DATA(it) == arr[count]);
    ITERATOR_INC(it);
    count++;
  }
  assert(count == 3);
  assert(ITERATOR_IS_NULL(it));

  LINKED_LIST_DESTROY(list);
}

/* ── Iterator cmp ────────────────────────────────────────────────────────── */

void test_iterator_cmp(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);

  LINKED_LIST_PUSH_BACK(list, 1);
  LINKED_LIST_PUSH_BACK(list, 2);
  LINKED_LIST_PUSH_BACK(list, 3);

  ITERATOR(int) a = LINKED_LIST_BEGIN(list, a);
  ITERATOR(int) b = LINKED_LIST_BEGIN(list, b);

  assert(ITERATOR_CMP(a, b) == 0);

  ITERATOR_INC(b);
  assert(ITERATOR_CMP(a, b) < 0);
  assert(ITERATOR_CMP(b, a) > 0);

  /* Exhaust b — exhausted is always greater */
  ITERATOR_INC(b);
  ITERATOR_INC(b);
  assert(ITERATOR_IS_NULL(b));
  assert(ITERATOR_CMP(b, a) > 0);

  LINKED_LIST_DESTROY(list);
}

/* ── Iterator move_at ────────────────────────────────────────────────────── */

void test_iterator_move_at(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);

  LINKED_LIST_PUSH_BACK(list, 10);
  LINKED_LIST_PUSH_BACK(list, 20);
  LINKED_LIST_PUSH_BACK(list, 30);
  LINKED_LIST_PUSH_BACK(list, 40);
  LINKED_LIST_PUSH_BACK(list, 50);

  ITERATOR(int) it = LINKED_LIST_BEGIN(list, it);

  ITERATOR_MOVE_AT(it, 2);
  assert(it.index == 2);
  assert(ITERATOR_GET_DATA(it) == 30);

  ITERATOR_MOVE_AT(it, 0);
  assert(it.index == 0);
  assert(ITERATOR_GET_DATA(it) == 10);

  ITERATOR_MOVE_AT(it, 4);
  assert(it.index == 4);
  assert(ITERATOR_GET_DATA(it) == 50);

  LINKED_LIST_DESTROY(list);
}

/* ── Iterator get_at ─────────────────────────────────────────────────────── */

void test_iterator_get_at(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);

  LINKED_LIST_PUSH_BACK(list, 100);
  LINKED_LIST_PUSH_BACK(list, 200);
  LINKED_LIST_PUSH_BACK(list, 300);

  ITERATOR(int) it = LINKED_LIST_GET_ITERATOR_AT(list, it, 1);
  assert(it.index == 1);
  assert(ITERATOR_GET_DATA(it) == 200);

  ITERATOR(int) it2 = LINKED_LIST_GET_ITERATOR_AT(list, it2, 0);
  assert(it2.index == 0);
  assert(ITERATOR_GET_DATA(it2) == 100);

  ITERATOR(int) it3 = LINKED_LIST_GET_ITERATOR_AT(list, it3, 2);
  assert(it3.index == 2);
  assert(ITERATOR_GET_DATA(it3) == 300);

  LINKED_LIST_DESTROY(list);
}

/* ── Iterator null after exhaustion ─────────────────────────────────────── */

void test_iterator_null(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);

  LINKED_LIST_PUSH_BACK(list, 1);
  LINKED_LIST_PUSH_BACK(list, 2);

  ITERATOR(int) it = LINKED_LIST_BEGIN(list, it);

  ITERATOR_INC(it);
  ITERATOR_INC(it);

  assert(ITERATOR_IS_NULL(it));
  assert(it.index_ptr == NULL);
  assert(it.index == list.size);

  LINKED_LIST_DESTROY(list);
}

/* ── Reinit after destroy ────────────────────────────────────────────────── */

void test_reinit_after_destroy(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);

  LINKED_LIST_PUSH_BACK(list, 1);
  LINKED_LIST_PUSH_BACK(list, 2);
  LINKED_LIST_DESTROY(list);
  assert(list.state == DS_STATE_DESTROYED);

  LINKED_LIST_INIT(list);
  assert(list.size == 0);

  LINKED_LIST_PUSH_BACK(list, 99);
  assert(LINKED_LIST_SIZE(list)  == 1);
  assert(LINKED_LIST_FRONT(list) == 99);

  LINKED_LIST_DESTROY(list);
}

/* ── Many elements ───────────────────────────────────────────────────────── */

void test_many_elements(void)
{
  LINKED_LIST(int) list = LINKED_LIST_INIT(list);

  for (int i = 0; i < 100; i++)
    LINKED_LIST_PUSH_BACK(list, i);

  assert(LINKED_LIST_SIZE(list)  == 100);
  assert(LINKED_LIST_FRONT(list) == 0);
  assert(LINKED_LIST_BACK(list)  == 99);

  ITERATOR(int) it  = LINKED_LIST_BEGIN(list, it);
  ITERATOR(int) end = LINKED_LIST_END(list, end);

  int expected = 0;
  while (ITERATOR_CMP(end, it) >= 0) {
    assert(ITERATOR_GET_DATA(it) == expected++);
    ITERATOR_INC(it);
  }
  assert(expected == 100);

  LINKED_LIST_DESTROY(list);
}

/* ── Entry point ─────────────────────────────────────────────────────────── */

int main(void)
{
  test_init();
  test_push_pop();
  test_single_element();
  test_clear();
  test_destroy();
  test_iterator_forward();
  test_iterator_backward();
  test_iterator_cmp_loop();
  test_iterator_cmp();
  test_iterator_move_at();
  test_iterator_get_at();
  test_iterator_null();
  test_reinit_after_destroy();
  test_many_elements();

  return 0;
}
