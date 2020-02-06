// C code for linked list merged sort
#include <stdlib.h>

#define MAX_SIZE 16384

const long long _NULL = -1;
template <class T>
struct Allocator {
  typedef long long ptr;

  // the actual storage space
  T space[MAX_SIZE];
  // stack to store all indexes of free space
  ptr freeMemory[MAX_SIZE];
  // number of free mem, pointer to the top of the stack
  int noOffreeMemory;

  Allocator() {
    noOffreeMemory = MAX_SIZE;
    // at the begining, all space are free
    for (int i = 0; i < noOffreeMemory; i++) {
      freeMemory[i] = i;
    }
  }

  ptr malloc() {
    if (noOffreeMemory < 1) {
      return _NULL;
    } else{
      // pop one free space from the stack
      noOffreeMemory--;
      return freeMemory[noOffreeMemory];
    }
  }

  void free(ptr ptr1) {
    // push the deallocated spcae back to stack
    freeMemory[noOffreeMemory++] = ptr1;
  }
};

bool g_fallback = false;

/* Link list node */
struct Node {
  int data;
  int next;
};
typedef struct Node node;
Allocator<node> list;

/* function prototypes */
Allocator<node>::ptr SortedMerge(
        Allocator<node>::ptr a, Allocator<node>::ptr b);

struct _recurse_rec_MergeSort {
  Allocator<node>::ptr head;
  Allocator<node>::ptr fast;
  Allocator<node>::ptr slow;
  Allocator<node>::ptr a;
  Allocator<node>::ptr b;
  int _location;
  Allocator<node>::ptr return_val;
};

/* sorts the linked list by changing next pointers (not data) */
Allocator<node>::ptr MergeSort(Allocator<node>::ptr head) {
  volatile _recurse_rec_MergeSort _stack[MAX_SIZE];
  volatile int _stack_top = 0;

  // initialize the stack
  _stack[_stack_top].head = head;
  _stack[_stack_top]._location = 0;
  _stack_top++;

  while (_stack_top != 0) {
    _stack_top--;
    // restore variables
    head = _stack[_stack_top].head;
    Allocator<node>::ptr fast = _stack[_stack_top].fast;
    Allocator<node>::ptr slow = _stack[_stack_top].slow;
    Allocator<node>::ptr a = _stack[_stack_top].a;
    Allocator<node>::ptr b = _stack[_stack_top].b;

    // restore location
    int _location = _stack[_stack_top]._location;
    if (_location == 0) goto MergeSort_L0;
    if (_location == 1) goto MergeSort_L1;
    if (_location == 2) goto MergeSort_L2;

MergeSort_L0:
    /* Base case -- length 0 or 1 */
    if ((head == _NULL) || (list.space[head].next == _NULL)) {
      // function return: return head;
      // set return value
      _stack[_stack_top].return_val = head;
      // restore to the previous level
      continue;
    }

    fast = list.space[head].next;
    slow = head;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != _NULL) {
      fast = list.space[fast].next;
      if (fast != _NULL) {
        slow = list.space[slow].next;
        fast = list.space[fast].next;
      }
    }

    /* 'slow' is before the midpoint in the list, so split it in two
    at that point. */
    a = head;
    b = list.space[slow].next;
    list.space[slow].next = _NULL;

    /* Recursively sort the sublists */
    // stack overflow
    if (_stack_top + 1 == MAX_SIZE) {
      g_fallback = true;
      return 0;
    }
    // function call: MergeSort(a);
    // store variable
    _stack[_stack_top].head = head;
    _stack[_stack_top].fast = fast;
    _stack[_stack_top].slow = slow;
    _stack[_stack_top].a = a;
    _stack[_stack_top].b = b;
    // variable init
    head = a;
    // store location
    _stack[_stack_top]._location = 1;
    // push stack
    _stack_top++;
    // jump to head of the function
    goto MergeSort_L0;

MergeSort_L1:
    // get return value
    a = _stack[_stack_top+1].return_val;
    // stack overflow
    if (_stack_top + 1 == MAX_SIZE) {
      g_fallback = true;
      return 0;
    }
    // function call: MergeSort(b);
    // store variable
    _stack[_stack_top].head = head;
    _stack[_stack_top].fast = fast;
    _stack[_stack_top].slow = slow;
    _stack[_stack_top].a = a;
    _stack[_stack_top].b = b;
    // variable init
    head = b;
    // store location
    _stack[_stack_top]._location = 2;
    // push stack
    _stack_top++;
    // jump to head of the function
    goto MergeSort_L0;

MergeSort_L2:
    // get return value
    b = _stack[_stack_top+1].return_val;

    /* answer = merge the two sorted lists together */
    // function return: return SortedMerge(a, b);
    _stack[_stack_top].return_val = SortedMerge(a, b);
  }

  // return the top level return value;
  return _stack[_stack_top].return_val;
}

struct _recurse_rec_SortedMerge {
  Allocator<node>::ptr a;
  Allocator<node>::ptr b;
  Allocator<node>::ptr result;
  int _location;
  Allocator<node>::ptr return_val;
};

/* See https://www.geeksforgeeks.org/?p=3622 for details of this
function */
Allocator<node>::ptr SortedMerge(
        Allocator<node>::ptr a, Allocator<node>::ptr b) {
  volatile _recurse_rec_SortedMerge _stack[MAX_SIZE];
  volatile int _stack_top = 0;

  // initialize the stack
  _stack[_stack_top].a = a;
  _stack[_stack_top].b = b;
  _stack[_stack_top]._location = 0;
  _stack_top++;

  while (_stack_top != 0) {
    _stack_top--;
    // restore variables
    a = _stack[_stack_top].a;
    b = _stack[_stack_top].b;
    Allocator<node>::ptr result = _stack[_stack_top].result;

    // restore location
    int _location = _stack[_stack_top]._location;
    if (_location == 0) goto SortedMerge_L0;
    if (_location == 1) goto SortedMerge_L1;
    if (_location == 2) goto SortedMerge_L2;

SortedMerge_L0:
    result = _NULL;

    /* Base cases */
    if (a == _NULL) {
      // function return: return (b);
      // set return value
      _stack[_stack_top].return_val = b;
      // restore to the previous level
      continue;
    }
    else if (b == _NULL) {
      // function return: return (a);
      // set return value
      _stack[_stack_top].return_val = a;
      // restore to the previous level
      continue;
    }

    /* Pick either a or b, and recur */
    if (list.space[a].data <= list.space[b].data) {
      result = a;
      // function call: SortedMerge(list.space[a].next, b);
      // store variable
      _stack[_stack_top].a = a;
      _stack[_stack_top].b = b;
      _stack[_stack_top].result = result;
      // variable init
      a = list.space[a].next;
      b = b;
      // store location
      _stack[_stack_top]._location = 1;
      // push stack
      _stack_top++;
      // jump to head of the function
      goto SortedMerge_L0;

SortedMerge_L1:
      // get return value
      list.space[result].next = _stack[_stack_top+1].return_val;
    } else {
      result = b;
      // function call: SortedMerge(a, list.space[b].next);
      // store variable
      _stack[_stack_top].a = a;
      _stack[_stack_top].b = b;
      _stack[_stack_top].result = result;
      // variable init
      a = a;
      b = list.space[b].next;
      // store location
      _stack[_stack_top]._location = 2;
      // push stack
      _stack_top++;
      // jump to head of the function
      goto SortedMerge_L0;

SortedMerge_L2:
      // get return value
      list.space[result].next = _stack[_stack_top+1].return_val;
    }
    // function return: return (result);
    // set return value
    _stack[_stack_top].return_val = result;
    // restore to the previous level
    continue;
  }

  // return the top level return value;
  return _stack[_stack_top].return_val;
}

/* Function to print nodes in a given linked list */
int *printList(Allocator<node>::ptr node, int *output) {
  while (node != _NULL) {
    *(output++) = list.space[node].data;
    node = list.space[node].next;
  }
  return output;
}

/* Function to insert a node at the beginging of the linked list */
void push(Allocator<node>::ptr *head_ref, int new_data) {
  /* allocate node */
  Allocator<node>::ptr new_node = list.malloc();
  if (new_node == _NULL) {
    g_fallback = true;
    return;
  }

  /* put in the data */
  list.space[new_node].data = new_data;

  /* link the old list off the new node */
  list.space[new_node].next = (*head_ref);

  /* move the head to point to the new node */
  (*head_ref) = new_node;
}

extern "C" {
/* Drier program to test above functions*/
void process_top(int n, int *input, int *output, int *fallback) {
#pragma HLS INTERFACE m_axi port = input offset = slave bundle = gmem
#pragma HLS INTERFACE m_axi port = output offset = slave bundle = gmem
#pragma HLS INTERFACE m_axi port = fallback offset = slave bundle = gmem
#pragma HLS INTERFACE s_axilite port = n bundle = control
#pragma HLS INTERFACE s_axilite port = input bundle = control
#pragma HLS INTERFACE s_axilite port = output bundle = control
#pragma HLS INTERFACE s_axilite port = fallback bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control
  /* Start with the empty list */
  Allocator<node>::ptr res = _NULL;
  Allocator<node>::ptr a = _NULL;

  /* Let us create a unsorted linked lists to test the functions */
  for (int i = 0; i < n; i++) {
    push(&a, input[i]);
    if (g_fallback) goto fail;
  }

  /* Sort the above created Linked List */
  a = MergeSort(a);
  if (g_fallback) goto fail;

  output = printList(a, output);

fail:
  *fallback = g_fallback;
}
};
