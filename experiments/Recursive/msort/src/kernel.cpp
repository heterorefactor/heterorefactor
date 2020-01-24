// C code for linked list merged sort
#include <stdlib.h>

bool g_fallback = false;

/* Link list node */
struct Node {
  int data;
  struct Node *next;
};

/* function prototypes */
struct Node *SortedMerge(struct Node *a, struct Node *b);

/* sorts the linked list by changing next pointers (not data) */
struct Node *MergeSort(struct Node *head) {
  /* Base case -- length 0 or 1 */
  if ((head == NULL) || (head->next == NULL)) {
    return head;
  }

  struct Node *fast = head->next;
  struct Node *slow = head;

  /* Advance 'fast' two nodes, and advance 'slow' one node */
  while (fast != NULL) {
    fast = fast->next;
    if (fast != NULL) {
      slow = slow->next;
      fast = fast->next;
    }
  }

  /* 'slow' is before the midpoint in the list, so split it in two
  at that point. */
  struct Node *a = head;
  struct Node *b = slow->next;
  slow->next = NULL;

  /* Recursively sort the sublists */
  a = MergeSort(a);
  b = MergeSort(b);

  /* answer = merge the two sorted lists together */
  return SortedMerge(a, b);
}

/* See https://www.geeksforgeeks.org/?p=3622 for details of this
function */
struct Node *SortedMerge(struct Node *a, struct Node *b) {
  struct Node *result = NULL;

  /* Base cases */
  if (a == NULL)
    return (b);
  else if (b == NULL)
    return (a);

  /* Pick either a or b, and recur */
  if (a->data <= b->data) {
    result = a;
    result->next = SortedMerge(a->next, b);
  } else {
    result = b;
    result->next = SortedMerge(a, b->next);
  }
  return (result);
}

/* Function to print nodes in a given linked list */
int *printList(struct Node *node, int *output) {
  while (node != NULL) {
    *(output++) = node->data;
    node = node->next;
  }
  return output;
}

/* Function to insert a node at the beginging of the linked list */
void push(struct Node **head_ref, int new_data) {
  /* allocate node */
  struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
  if (new_node == NULL) {
    g_fallback = true;
    return;
  }

  /* put in the data */
  new_node->data = new_data;

  /* link the old list off the new node */
  new_node->next = (*head_ref);

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
  struct Node *res = NULL;
  struct Node *a = NULL;

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
