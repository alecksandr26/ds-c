
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
  
  assert(((uint64_t) list_int.foo_ptr) == 1);
  
  return 0;
}
