#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 16384

const int _NULL = -1;
template <class T>
struct Allocator {
    typedef int ptr;

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

typedef struct {
    int info;
} DATA;

typedef struct NODE {
    DATA data;
    int next;
} NODE;
Allocator<NODE> list;

void init(Allocator<NODE>::ptr *head) {
    *head = _NULL;
}

int *output_list(Allocator<NODE>::ptr head, int *curr) {
    Allocator<NODE>::ptr temp;
    for (temp = head; temp != _NULL; temp = list.space[temp].next) {
        *(curr++) = list.space[temp].data.info;
    }
    return curr;
}

Allocator<NODE>::ptr add(Allocator<NODE>::ptr node, DATA data) {
    Allocator<NODE>::ptr temp = list.malloc();
    if (temp == _NULL) {
        g_fallback = true; return _NULL;
    }
    list.space[temp].data = data;
    list.space[temp].next = node;
    node = temp;
    return node;
}

void add_at(Allocator<NODE>::ptr node, DATA data) {
    Allocator<NODE>::ptr temp = list.malloc();
    if (temp == _NULL) {
        g_fallback = true; return;
    }
    list.space[temp].data = data;
    list.space[temp].next = list.space[node].next;
    list.space[node].next = temp;
}

void remove_node(Allocator<NODE>::ptr head) {
    Allocator<NODE>::ptr temp = list.space[head].next;
    list.space[head].next = list.space[list.space[head].next].next;
    list.free(temp);
}

struct _reverse_rec_recursion_record {
    Allocator<NODE>::ptr ptr;
    Allocator<NODE>::ptr previous;
    Allocator<NODE>::ptr temp;
    int _location;
    Allocator<NODE>::ptr return_val;
};

Allocator<NODE>::ptr reverse_rec(
        Allocator<NODE>::ptr ptr, Allocator<NODE>::ptr previous) {
    volatile _reverse_rec_recursion_record _stack[MAX_SIZE];
    volatile int _stack_top = 0;

    // initialize the stack
    _stack[_stack_top].ptr = ptr;
    _stack[_stack_top].previous = previous;
    _stack[_stack_top]._location = 0;
    _stack_top++;

    while (_stack_top != 0) {
        _stack_top--;
        // restore variables
        ptr = _stack[_stack_top].ptr;
        previous = _stack[_stack_top].previous;
        Allocator<NODE>::ptr temp = _stack[_stack_top].temp;

        // restore location
        int _location = _stack[_stack_top]._location;
        if (_location == 0) goto reverse_rec_L0;
        if (_location == 1) goto reverse_rec_L1;

reverse_rec_L0:
        if (list.space[ptr].next == _NULL) {
            list.space[ptr].next = previous;
            // function return: return ptr;
            // set return value
            _stack[_stack_top].return_val = ptr;
            // restore to the previous level
            continue;

        } else {
            // stack overflow
            if (_stack_top + 1 == MAX_SIZE) {
                g_fallback = true;
                return 0;
            }
            // function call: reverse_rec(list.space[ptr].next, ptr);
            // store variable
            _stack[_stack_top].ptr = ptr;
            _stack[_stack_top].previous = previous;
            _stack[_stack_top].temp = temp;
            // variable init
            ptr = list.space[_stack[_stack_top].ptr].next;
            previous = _stack[_stack_top].ptr;
            // store location
            _stack[_stack_top]._location = 1;
            // push stack
            _stack_top++;
            // jump to head of the function
            goto reverse_rec_L0;

reverse_rec_L1:
            // get return value
            temp = _stack[_stack_top+1].return_val;
            list.space[ptr].next = previous;
            // function return: return temp;
            // set return value
            _stack[_stack_top].return_val = temp;
            // restore to the previous level
            continue;
        }
    }

    // return the top level return value;
    return _stack[_stack_top].return_val;
}

Allocator<NODE>::ptr sort_list(Allocator<NODE>::ptr head) {
    Allocator<NODE>::ptr tmpPtr = head, tmpNxt = list.space[head].next;
    DATA tmp;
    while (tmpNxt != _NULL) {
        while (tmpNxt != tmpPtr) {
            if (list.space[tmpNxt].data.info < list.space[tmpPtr].data.info) {
                tmp = list.space[tmpPtr].data;
                list.space[tmpPtr].data = list.space[tmpNxt].data;
                list.space[tmpNxt].data = tmp;
            }
            tmpPtr = list.space[tmpPtr].next;
        }
        tmpPtr = head;
        tmpNxt = list.space[tmpNxt].next;
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
    Allocator<NODE>::ptr head;
    Allocator<NODE>::ptr node;
    DATA element;
    int *curr;

    init(&head);
    for (int i = 0; i < n; i++) {
        element.info = input[i];
        head = add(head, element);
        if (head == _NULL) { g_fallback = true; goto fail; }
    }

    curr = output;
    curr = output_list(head, curr);
    *(curr++) = -1;

    node = list.space[list.space[list.space[head].next].next].next;
    element.info = 2000;
    add_at(node, element);
    if (g_fallback) goto fail;
    curr = output_list(head, curr);
    *(curr++) = -1;

    node = list.space[list.space[head].next].next;
    remove_node(node);
    head = sort_list(head);
    curr = output_list(head, curr);
    *(curr++) = -1;

    head = reverse_rec(head, _NULL);
    if (g_fallback) goto fail;
    curr = output_list(head, curr);
    *(curr++) = -1;

fail:
    *fallback = g_fallback;
}
};
