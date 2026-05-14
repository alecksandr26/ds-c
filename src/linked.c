#include <inttypes.h>
#include <except/assert.h>

#define LINKED_LIST_PRIVATE
#include "../include/ds/linked.h"
#include "../include/ds/ds_exception.h"
#include "../include/ds/ds_mem.h"


#define ITERATOR_PRIVATE
#undef ITERATOR_INCLUDED
#include "../include/ds/iterator.h"

static void LinkedList_set_state(LinkedList *linked, DS_State state)
{
  assert(linked != NULL);
  assert(state == DS_STATE_ALIVE || state == DS_STATE_DESTROYED);
  linked->state = state;
}

static DS_State LinkedList_get_state(LinkedList *linked)
{
  assert(linked != NULL);
  return linked->state;
}

static void LinkedList_basic_validations(LinkedList *linked)
{
  if (linked == NULL) RAISE(ExceptInvalidArgument, "Linked can't be null");
  if (LinkedList_get_state(linked) == DS_STATE_DESTROYED) RAISE(ExceptDestroyedDataStructure, "Linked list has been destroyed");
}

void LinkedList_init(LinkedList *linked, uint32_t type_size)
{
  if (linked == NULL) RAISE(ExceptInvalidArgument, "Linked can't be null");
  if (type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");

  if (linked->size != 0 && LinkedList_get_state(linked) == DS_STATE_ALIVE) {
    LinkedList_clear(linked);
    return;
  }

  linked->head = linked->tail = NULL;
  linked->type_size = type_size;
  linked->size = 0;

  LinkedList_set_state(linked, DS_STATE_ALIVE);
}

const uint8_t *LinkedList_front(LinkedList *linked)
{
  LinkedList_basic_validations(linked);
  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't fetch a front element in an empty linked list");
  assert(linked->tail != NULL);
  assert(linked->tail->data_ptr != NULL);
  return linked->tail->data_ptr;
}

const uint8_t *LinkedList_back(LinkedList *linked)
{
  LinkedList_basic_validations(linked);
  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't fetch a back element in an empty linked list");
  assert(linked->head != NULL);
  assert(linked->head->data_ptr != NULL);
  return linked->head->data_ptr;
}


void LinkedList_push_back(LinkedList *linked, const uint8_t *data)
{
  LinkedList_basic_validations(linked);
  if (linked->type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");
  assert(data != NULL);

  Node *new_node = ds_mem_alloc(sizeof(Node) + linked->type_size);
  if (new_node == NULL) RAISE(ExceptBadAlloc, "Null alloc with the defined ds_mem_alloc");
  new_node->next = new_node->prev = NULL;
  new_node->data_ptr = (uint8_t *) (new_node + 1);
  ds_mem_copy((uint8_t *) new_node->data_ptr, data, linked->type_size);

  if (linked->head) {
    linked->head->next = new_node;
    new_node->prev = linked->head;
  }

  if (linked->tail == NULL) {
    linked->tail = new_node;
  }

  linked->head = new_node;
  linked->size++;
}


void LinkedList_push_front(LinkedList *linked, const uint8_t *data)
{
  LinkedList_basic_validations(linked);
  if (linked->type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");
  assert(data != NULL);

  Node *new_node = ds_mem_alloc(sizeof(Node) + linked->type_size);
  if (new_node == NULL) RAISE(ExceptBadAlloc, "Null alloc with the defined ds_mem_alloc");
  new_node->next = new_node->prev = NULL;
  new_node->data_ptr = (uint8_t *) (new_node + 1);
  ds_mem_copy((uint8_t *) new_node->data_ptr, data, linked->type_size);

  if (linked->tail) {
    linked->tail->prev = new_node;
    new_node->next = linked->tail;
  }

  if (linked->head == NULL) {
    linked->head = new_node;
  }

  linked->tail = new_node;
  linked->size++;
}

void LinkedList_pop_back(LinkedList *linked)
{
  LinkedList_basic_validations(linked);  
  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't pop a front element in an empty linked list");

  assert(linked->head != NULL);
  assert(linked->head->data_ptr != NULL);
  
  Node *head = linked->head;
  
  if (head->prev != NULL) {
    assert(head->prev->next != NULL);
    head->prev->next = NULL;
    linked->head = head->prev;
  } else {
    linked->head = linked->tail = NULL;
  }
  ds_mem_free(head);
  linked->size--;
}

void LinkedList_pop_front(LinkedList *linked)
{
  LinkedList_basic_validations(linked);
  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't pop a back element in an empty linked list");

  assert(linked->tail != NULL);
  assert(linked->tail->data_ptr != NULL);

  Node *tail = linked->tail;
  
  if (tail->next != NULL) {
    assert(tail->next->prev != NULL);
    tail->next->prev = NULL;
    linked->tail = tail->next;
  } else {
    linked->head = linked->tail = NULL;
  }
  ds_mem_free(tail);
  linked->size--;
}

void LinkedList_clear(LinkedList *linked)
{
  LinkedList_basic_validations(linked);
  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't clear an empty linked list");

  assert(linked->head != NULL);
  assert(linked->tail != NULL);
  
  while (linked->head) {
    Node *prev = linked->head->prev;
    ds_mem_free(linked->head);
    linked->head = prev;
  }
  
  linked->head = linked->tail = NULL;
  linked->size = 0;
}

void LinkedList_destroy(LinkedList *linked)
{
  LinkedList_basic_validations(linked);

  while (linked->head) {
    Node *prev = linked->head->prev;
    ds_mem_free(linked->head);
    linked->head = prev;
  }
  
  linked->head = linked->tail = NULL;
  linked->size = 0;
  linked->type_size = 0;
  LinkedList_set_state(linked, DS_STATE_DESTROYED);
}

static void LinkedList_Iterator_inc(Iterator *iterator)
{
  Iterator_basic_validations(iterator);
  LinkedList *linked = (LinkedList *) iterator->ds;
  LinkedList_basic_validations(linked);
  
  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't iterate in an empty linked list");
  if (iterator->index > linked->size) RAISE(ExceptInvalidArgument, "Invalid Iterator");
  if (iterator->index == linked->size) RAISE(ExceptInvalidArgument, "Can't iterate; already pass the top");
  if (iterator->index_ptr == NULL) RAISE(ExceptInvalidArgument, "Invalid Iterator");
  
  iterator->index_ptr = (uint8_t *) ((Node * ) iterator->index_ptr)->next;
  iterator->index++;
}

static void LinkedList_Iterator_dec(Iterator *iterator)
{
  Iterator_basic_validations(iterator);
  LinkedList *linked = (LinkedList *) iterator->ds;
  LinkedList_basic_validations(linked);

  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't iterate in an empty linked list");
  if (iterator->index > linked->size) RAISE(ExceptInvalidArgument, "Can't iterate; already pass the bottom");
  if (iterator->index_ptr == NULL) RAISE(ExceptInvalidArgument, "Invalid Iterator");
  
  iterator->index_ptr = (uint8_t *) ((Node * ) iterator->index_ptr)->prev;
  iterator->index--;
}

static const uint8_t *LinkedList_Iterator_get_data(const Iterator *iterator)
{
  Iterator_basic_validations(iterator);
  LinkedList *linked = (LinkedList *) iterator->ds;
  LinkedList_basic_validations(linked);

  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't iterate in an empty linked list");
  // We must to return NULL if we have some null iterator
  if (iterator->index_ptr == NULL)
    return NULL;

  return ((Node *) iterator->index_ptr)->data_ptr;
}

static void LinkedList_Iterator_move_at(Iterator *iterator, uint32_t index)
{
  Iterator_basic_validations(iterator);
  LinkedList *linked = (LinkedList *) iterator->ds;
  LinkedList_basic_validations(linked);

  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't iterate in an empty linked list");
  if (index >= linked->size) RAISE(ExceptInvalidIndex, "Index out of bounds");

  if (index > linked->size / 2) {
    LinkedList_end(linked, iterator, iterator->cmp);
    for (uint32_t i = linked->size - 1; i > index; i--)
      LinkedList_Iterator_dec(iterator);
  } else {
    LinkedList_begin(linked, iterator, iterator->cmp);
    for (uint32_t i = 0; i < index; i++)
      LinkedList_Iterator_inc(iterator);
  }
}


static void LinkedList_Iterator_insert(Iterator *iterator, const uint8_t *new_value)
{
  Iterator_basic_validations(iterator);
  LinkedList *linked = (LinkedList *) iterator->ds;
  LinkedList_basic_validations(linked);

  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Invalid Iterator");

  Node *new_node = ds_mem_alloc(sizeof(Node) + linked->type_size);
  if (new_node == NULL) RAISE(ExceptBadAlloc, "Null alloc with the defined ds_mem_alloc");
  new_node->next = new_node->prev = NULL;
  new_node->data_ptr = (uint8_t *) (new_node + 1);
  ds_mem_copy((uint8_t *) new_node->data_ptr, new_value, linked->type_size);

  new_node->next = (Node *) iterator->index_ptr;  
  if (((Node *) iterator->index_ptr)->prev) {
    new_node->prev = ((Node *) iterator->index_ptr)->prev;
    ((Node *) iterator->index_ptr)->prev = new_node;
  }

  if (linked->tail == (Node *) iterator->index_ptr) {
    linked->tail = new_node;
  }
    
  linked->size++;
}

static void LinkedList_Iterator_remove(Iterator *iterator)
{
  Iterator_basic_validations(iterator);
  LinkedList *linked = (LinkedList *) iterator->ds;
  LinkedList_basic_validations(linked);

  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't removed anything from an empty linked list");
  
  if (((Node *) iterator->index_ptr)->next) {
    ((Node *) iterator->index_ptr)->next->prev = ((Node *) iterator->index_ptr)->prev;
  }

  if (((Node *) iterator->index_ptr)->prev) {
    ((Node *) iterator->index_ptr)->prev->next = ((Node *) iterator->index_ptr)->next;
  }

  if (linked->head == (Node *) iterator->index_ptr) {
    linked->head = ((Node *) iterator->index_ptr)->prev;
  }
  
  if (linked->tail == (Node *) iterator->index_ptr) {
    linked->tail = ((Node *) iterator->index_ptr)->next;
  }

  linked->size--;
}

static void LinkedList_Iterator_replace(Iterator *iterator, const uint8_t *new_value)
{
  Iterator_basic_validations(iterator);
  LinkedList *linked = (LinkedList *) iterator->ds;
  LinkedList_basic_validations(linked);

  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't replaced anything from an empty linked list");
  
  ds_mem_copy(
    (uint8_t *) ((Node *) iterator->index_ptr)->data_ptr,
    new_value,
    linked->type_size
  );
}

void LinkedList_begin(LinkedList *linked, Iterator *iterator, int (* const cmp)(const uint8_t *, const uint8_t *))
{
  LinkedList_basic_validations(linked);
  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't fetch any iterator from an empty linked list");

  assert(linked->tail != NULL);
  
  Iterator_init(
    iterator,
    (const uint8_t *) linked,
    (uint8_t *) linked->tail,
    0,
    &LinkedList_Iterator_inc,
    &LinkedList_Iterator_dec,
    &LinkedList_Iterator_get_data,
    &LinkedList_Iterator_move_at,
    &LinkedList_Iterator_insert,
    &LinkedList_Iterator_remove,
    &LinkedList_Iterator_replace,
    cmp
  );
}

void LinkedList_end(LinkedList *linked, Iterator *iterator, int (* const cmp)(const uint8_t *, const uint8_t *))
{
  LinkedList_basic_validations(linked);
  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't fetch any iterator from an empty linked list");

  assert(linked->head != NULL);
  
  Iterator_init(
    iterator,
    (const uint8_t *) linked,
    (uint8_t *) linked->head,
    linked->size - 1,
    &LinkedList_Iterator_inc,
    &LinkedList_Iterator_dec,
    &LinkedList_Iterator_get_data,
    &LinkedList_Iterator_move_at,
    &LinkedList_Iterator_insert,
    &LinkedList_Iterator_remove,
    &LinkedList_Iterator_replace,
    cmp
  );
}
