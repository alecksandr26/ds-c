#include <stdio.h>

int main(void)
{
  char mem[1024];

  char *rand_ptr = mem + 100;

  
  puts("ARRAY: ");
  *((long *) rand_ptr) = 10;
  *((long *) (rand_ptr + sizeof(long))) = 11;
  *((long *) (rand_ptr + 2 * sizeof(long))) = 12;

  printf("%d\n", ((long *) rand_ptr)[0]);
  printf("%d\n", ((long *) rand_ptr)[1]);
  printf("%d\n", ((long *) rand_ptr)[2]);


  long *arr = (long *) rand_ptr;
  int arr_size = 3;
  
  for (int i = 0; i < arr_size; i++) {
    printf("%p -> arr[%d] = %d\n", &arr[i], i, arr[i]);
  }

  printf("%p - %p = %d\n", &arr[1], &arr[0], (long) ((char *) &arr[1] - (char *) &arr[0]));
  printf("%p - %p = %d\n", &arr[2], &arr[1], (long) ((char *) &arr[2] - (char *) &arr[1]));



  puts("LINKED:");
  typedef struct node_t {
    long number;
    struct node_t *ptr;
  } Node;

  char *rand_ptr_2 = mem + 200;

  ((Node *) rand_ptr_2)[20].number = 10;
  ((Node *) rand_ptr_2)[10].number = 11;
  ((Node *) rand_ptr_2)[5].number = 12;

  ((Node *) rand_ptr_2)[20].ptr = &(((Node *) rand_ptr_2)[10]);
  ((Node *) rand_ptr_2)[10].ptr = &(((Node *) rand_ptr_2)[5]);
  ((Node *) rand_ptr_2)[5].ptr = NULL; /* 0x00000 */

  printf("%p -> { number: %d, ptr: %p }\n",
	 &((Node *) rand_ptr_2)[20], ((Node *) rand_ptr_2)[20].number, ((Node *) rand_ptr_2)[20].ptr);
  printf("%p -> { number: %d, ptr: %p }\n",
	 ((Node *) rand_ptr_2)[20].ptr, ((Node *) rand_ptr_2)[20].ptr->number, ((Node *) rand_ptr_2)[20].ptr->ptr);
  printf("%p -> { number: %d, ptr: %p }\n",
	 ((Node *) rand_ptr_2)[20].ptr->ptr, ((Node *) rand_ptr_2)[20].ptr->ptr->number, ((Node *) rand_ptr_2)[20].ptr->ptr->ptr);




  int val = __extension__ ({
      int n = 10;
      
      while (--n > 5)
	;
      n - 2;
    });


  printf("%d\n", val);
  return 0;
  

}

