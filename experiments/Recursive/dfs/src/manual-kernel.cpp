#include <stdlib.h>
#include <stdio.h>

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
            space[i].left = _NULL;
            space[i].right = _NULL;
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

struct btnode {
    int value;
    int left, right;
};
typedef struct btnode node;
Allocator<node> tree;

/* function declarations */
void insert(Allocator<node>::ptr, Allocator<node>::ptr);
void dfs_traverse(Allocator<node>::ptr);

/* global declarations */
Allocator<node>::ptr root = _NULL;
int front = 0, rear = -1;
int queue[MAX_SIZE];

bool g_fallback = false;

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
    Allocator<node>::ptr newitem = _NULL;
    int num = 10;

    for (int i = 0; i < n; i++) {
        newitem = tree.malloc();
        if (newitem == _NULL) {
            *fallback = true;
            return;
        }

        tree.space[newitem].left = tree.space[newitem].right = _NULL;
        tree.space[newitem].value = input[i];

        if (root == _NULL) {
            root = newitem;
        } else {
            insert(newitem, root);
            if (g_fallback) goto fail;
        }
    }

    dfs_traverse(root);
    if (g_fallback) goto fail;
    for (int i = 0; i < n; i++)
        output[i] = queue[i];
fail:
    *fallback = g_fallback;
}
};

/* inserting nodes of a tree */
void insert(Allocator<node>::ptr newitem, Allocator<node>::ptr root) {
    // this is a tail recursion, we can transform to loop
    while (true)
    {
        if (tree.space[newitem].value > tree.space[root].value) {
            if (tree.space[root].right == _NULL) {
                tree.space[root].right = newitem;
                return;
            } else {
                // modify the arguments and go to the start of the function
                root = tree.space[root].right;
                continue;
            }
        } else { // newitem->value <= root->value
            if (tree.space[root].left == _NULL) {
                tree.space[root].left = newitem;
                return;
            } else {
                // modify the arguments and go to the start of the function
                root = tree.space[root].left;
                continue;
            }
        }
    }
}

struct _dfs_traverse_recursion_record {
    Allocator<node>::ptr root;
    int _location;
};

/* displaying elements using BFS traversal */
void dfs_traverse(Allocator<node>::ptr root) {
    volatile _dfs_traverse_recursion_record _stack[MAX_SIZE];
    volatile int _stack_top = 0;

    // initialize the stack
    _stack[_stack_top].root = root;
    _stack[_stack_top]._location = 0;
    _stack_top++;

    while (_stack_top != 0) {
        _stack_top--;
        // restore variables
        root = _stack[_stack_top].root;

        // restore location
        int _location = _stack[_stack_top]._location;
        if (_location == 0) goto dfs_traverse_L0;
        if (_location == 1) goto dfs_traverse_L1;
        if (_location == 2) goto dfs_traverse_L2;

dfs_traverse_L0:
        if (tree.space[root].left != _NULL) {
            // stack overflow
            if (_stack_top == MAX_SIZE) {
                g_fallback = true;
                return;
            }
            // function call: dfs_traverse(tree.space[root].left);
            // store variable
            _stack[_stack_top].root = root;
            // variable init
            root = tree.space[_stack[_stack_top].root].left;
            // store location
            _stack[_stack_top]._location = 1;
            // push stack
            _stack_top++;
            // jump to head of the function
            goto dfs_traverse_L0;

dfs_traverse_L1:;
        }
        queue[++rear] = tree.space[root].value;
        if (tree.space[root].right != _NULL) {
            // stack overflow
            if (_stack_top == MAX_SIZE) {
                g_fallback = true;
                return;
            }
            // function call: dfs_traverse(tree.space[root].right);
            // store variable
            _stack[_stack_top].root = root;
            // variable init
            root = tree.space[_stack[_stack_top].root].right;
            // store location
            _stack[_stack_top]._location = 2;
            // push stack
            _stack_top++;
            // jump to head of the function
            goto dfs_traverse_L0;

dfs_traverse_L2:;
        }
    }
}
