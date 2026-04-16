#include <inttypes.h>
#include <except/assert.h>

#include "../include/ds/linked.h"
#include "../include/ds/ds_exception.h"
#include "../include/ds/ds_mem.h"
#include "consts.h"
#include "../include/ds/iterator.h"

uint64_t *LinkedList_fetch_foo_ptr_mem_reg(LinkedList *linked)
{
  assert(linked != NULL);
  
  return (uint64_t *) (((uint8_t *) linked) + sizeof(LinkedList));
}

void LinkedList_set_state(LinkedList *linked, DS_State state)
{
  assert(linked != NULL);
  assert(state == DS_STATE_ALIVE || state == DS_STATE_DESTROYED);
  
  uint64_t *foo_ptr_mem_reg = LinkedList_fetch_foo_ptr_mem_reg(linked);
  (*foo_ptr_mem_reg) = 0;
  (*foo_ptr_mem_reg) = (uint64_t) state;
}


DS_State LinkedList_get_state(LinkedList *linked)
{
  assert(linked != NULL);

  return (DS_State) *LinkedList_fetch_foo_ptr_mem_reg(linked);
}


void LinkedList_Iterator_inc(Iterator *iterator)
{
  // TODO: Find the way of adding some reusable function
  if (iterator == NULL) RAISE(ExceptInvalidArgument, "Iterator can't be null");
  if (iterator->ds == NULL) RAISE(ExceptInvalidArgument, "Iterator.ds can't be null");
  LinkedList *linked = (LinkedList *) iterator->ds;

  
  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't iterate in an empty linked list");
  if (iterator->index >= linked->size) RAISE(ExceptInvalidArgument, "Invalid Iterator");
  if (iterator->index_ptr == NULL) RAISE(ExceptInvalidArgument, "Invalid Iterator");
  if (iterator->index + 1 == linked->size) RAISE(ExceptInvalidArgument, "Can't iterate; already in the top");
  
  iterator->index_ptr = (uint8_t *) ((Node * ) iterator->index_ptr)->next;
  iterator->index++;
}


void LinkedList_Iterator_dec(Iterator *iterator)
{
  // TODO: Find the way of adding some reusable function
  if (iterator == NULL) RAISE(ExceptInvalidArgument, "Iterator can't be null");
  if (iterator->ds == NULL) RAISE(ExceptInvalidArgument, "Iterator.ds can't be null");
  LinkedList *linked = (LinkedList *) iterator->ds;

  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't iterate in an empty linked list");
  if (iterator->index >= linked->size) RAISE(ExceptInvalidArgument, "Invalid Iterator");
  if (iterator->index_ptr == NULL) RAISE(ExceptInvalidArgument, "Invalid Iterator");
  if (iterator->index == 0) RAISE(ExceptInvalidArgument, "Can't iterate; already in the bottom");
  
  iterator->index_ptr = (uint8_t *) ((Node * ) iterator->index_ptr)->prev;
  iterator->index--;
}


const uint8_t *LinkedList_Iterator_get_data(Iterator *iterator)
{
  if (iterator == NULL) RAISE(ExceptInvalidArgument, "Iterator can't be null");
  if (iterator->ds == NULL) RAISE(ExceptInvalidArgument, "Iterator.ds can't be null");
  LinkedList *linked = (LinkedList *) iterator->ds;

  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't iterate in an empty linked list");

  return ((Node *) iterator->index_ptr)->data_ptr;
}


void LinkedList_basic_validations(LinkedList *linked)
{
  if (linked == NULL) RAISE(ExceptInvalidArgument, "Linked can't be null");
  if (LinkedList_get_state(linked) == DS_STATE_DESTROYED) RAISE(ExceptInvalidArgument, "Linked list has been destroyed");
}

void LinkedList_init(LinkedList *linked, uint32_t type_size)
{
  if (linked == NULL) RAISE(ExceptInvalidArgument, "Linked can't be null");
  if (type_size == 0) RAISE(ExceptInvalidArgument, "TypeSize can't be zero");

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
  linked->type_size = 0;
  LinkedList_set_state(linked, DS_STATE_DESTROYED);
}


void LinkedList_begin(LinkedList *linked, Iterator *iterator)
{
  LinkedList_basic_validations(linked);
  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't fetch any iterator from an empty linked list");
  
  (*iterator) = (Iterator) {
    .ds = (const uint8_t *) linked,
    .index_ptr = (uint8_t *) linked->tail,
    .index = 0,
    .inc = &(LinkedList_Iterator_inc),
    .dec = &(LinkedList_Iterator_dec),
    .get_data = &(LinkedList_Iterator_get_data)
  };
}


void LinkedList_end(LinkedList *linked, Iterator *iterator)
{
  LinkedList_basic_validations(linked);
  if (linked->size == 0) RAISE(ExceptEmptyDataStructure, "Can't fetch any iterator from an empty linked list");

  (*iterator) = (Iterator) {
    .ds = (const uint8_t *) linked,
    .index_ptr = (uint8_t *) linked->head,
    .index = linked->size - 1,
    .inc = &(LinkedList_Iterator_inc),
    .dec = &(LinkedList_Iterator_dec),
    .get_data = &(LinkedList_Iterator_get_data)
  };
  
}
