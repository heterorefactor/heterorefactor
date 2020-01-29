#include <stdio.h>
#include <stdlib.h>

bool g_fallback = false;

typedef struct {
    int info;
} DATA;

typedef struct node {
    DATA data;
    struct node* next;
} NODE;

void init(NODE **head) {
    *head = NULL;
}

int *output_list(NODE *head, int *curr) {
    NODE *temp;
    for (temp = head; temp; temp = temp->next) {
        *(curr++) = temp->data.info;
    }
    return curr;
}

NODE *add(NODE *node, DATA data) {
    NODE *temp = (NODE *) malloc(sizeof (NODE));
    if (temp == NULL) {
        g_fallback = true; return NULL;
    }
    temp->data = data;
    temp->next = node;
    node = temp;
    return node;
}

void add_at(NODE *node, DATA data) {
    NODE *temp = (NODE*) malloc(sizeof (NODE));
    if (temp == NULL) {
        g_fallback = true; return;
    }
    temp->data = data;
    temp->next = node->next;
    node->next = temp;
}

void remove_node(NODE *head) {
    NODE *temp = head->next;
    head->next = head->next->next;
    free(temp);
}

NODE *reverse_rec(NODE *ptr, NODE *previous) {
    NODE *temp;
    if (ptr->next == NULL) {
        ptr->next = previous;
        return ptr;
    } else {
        temp = reverse_rec(ptr->next, ptr);
        ptr->next = previous;
        return temp;
    }
}

NODE *sort_list(NODE *head) {
    NODE *tmpPtr = head, *tmpNxt = head->next;
    DATA tmp;
    while (tmpNxt != NULL) {
        while (tmpNxt != tmpPtr) {
            if (tmpNxt->data.info < tmpPtr->data.info) {
                tmp = tmpPtr->data;
                tmpPtr->data = tmpNxt->data;
                tmpNxt->data = tmp;
            }
            tmpPtr = tmpPtr->next;
        }
        tmpPtr = head;
        tmpNxt = tmpNxt->next;
    }
    return tmpPtr;
}

extern "C" {
void process_top(int n, int *input, int *output, int *fallback) {
#pragma HLS INTERFACE m_axi port=input offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=fallback offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=n bundle=control
#pragma HLS INTERFACE s_axilite port=input bundle=control
#pragma HLS INTERFACE s_axilite port=output bundle=control
#pragma HLS INTERFACE s_axilite port=fallback bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
    NODE* head;
    NODE* node;
    DATA element;
    int *curr;

    init(&head);
    for (int i = 0; i < n; i++) {
        element.info = input[i];
        head = add(head, element);
        if (!head) { g_fallback = true; goto fail; }
    }

    curr = output;
    curr = output_list(head, curr);
    *(curr++) = -1;

    node = head->next->next->next;
    element.info = 2000;
    add_at(node, element);
    if (g_fallback) goto fail;
    curr = output_list(head, curr);
    *(curr++) = -1;

    node = head->next->next;
    remove_node(node);
    head = sort_list(head);
    curr = output_list(head, curr);
    *(curr++) = -1;

    head = reverse_rec(head, NULL);
    if (g_fallback) goto fail;
    curr = output_list(head, curr);
    *(curr++) = -1;

fail:
    *fallback = g_fallback;
}
};
