#include <stdio.h>
#include <except/assert.h>

#include "../include/ds/linked.h"




int main(void)
{
  typedef struct {
    char name[100];
    int age;
  } Person;
  
  LINKED_LIST(Person) list = LINKED_LIST_INIT(list);

  assert(list.type_size == 104);
  assert(list.head == NULL);
  assert(list.tail == NULL);
  assert(list.size == 0);


  LINKED_LIST(int) list_int = LINKED_LIST_INIT(list_int);

  LINKED_LIST_PUSH_BACK(list_int, 10);
  LINKED_LIST_PUSH_BACK(list_int, 12);
  LINKED_LIST_PUSH_BACK(list_int, 13);

  assert(LINKED_LIST_SIZE(list_int) == 3);
  assert(LINKED_LIST_FRONT(list_int) == 10);
  assert(LINKED_LIST_BACK(list_int) == 13);

  LINKED_LIST_PUSH_FRONT(list_int, 20);
  assert(LINKED_LIST_FRONT(list_int) == 20);
  assert(LINKED_LIST_SIZE(list_int) == 4);

  
  assert(LINKED_LIST_POP_BACK(list_int) == 13);
  assert(LINKED_LIST_SIZE(list_int) == 3);
  assert(LINKED_LIST_BACK(list_int) == 12);


  assert(LINKED_LIST_POP_FRONT(list_int) == 20);
  assert(LINKED_LIST_SIZE(list_int) == 2);
  assert(LINKED_LIST_FRONT(list_int) == 10);


  LINKED_LIST_CLEAR(list_int);

  assert(list_int.size == 0);

  LINKED_LIST_PUSH_BACK(list_int, 10);
  LINKED_LIST_PUSH_BACK(list_int, 12);
  LINKED_LIST_PUSH_BACK(list_int, 13);

  LINKED_LIST_DESTROY(list_int);

  assert(list_int.state == DS_STATE_DESTROYED);

  LINKED_LIST_INIT(list_int);



  LINKED_LIST_PUSH_BACK(list_int, 10);
  LINKED_LIST_PUSH_BACK(list_int, 12);
  LINKED_LIST_PUSH_BACK(list_int, 13);

  
  ITERATOR(int) it = LINKED_LIST_BEGIN(list_int, it);
  ITERATOR(int) end = LINKED_LIST_END(list_int, end);

  while (ITERATOR_CMP(end, it) >= 0) {
    ITERATOR_INC(it);
  }
  
  assert(it.index == list_int.size);
  assert(it.index_ptr == NULL);

  // Reset the beging
  LINKED_LIST_BEGIN(list_int, it);
  
  assert(ITERATOR_GET_DATA(it) == 10);
  
  ITERATOR_INC(it);

  assert(it.index == 1);
  assert(ITERATOR_GET_DATA(it) == 12);


  ITERATOR_DEC(it);


  assert(it.index == 0);
  assert(it.index_ptr == list_int.tail);

  assert(ITERATOR_GET_DATA(it) == 10);


  ITERATOR_INC(it);
  ITERATOR_INC(it);

  assert(it.index == 2);
  assert(ITERATOR_GET_DATA(it) == 13);
  
  
  
  return 0;
}
