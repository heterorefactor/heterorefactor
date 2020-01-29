#include <cassert>
#include <cstdlib>

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

/*
 * Trie tree with Aho-Corasick state machine.
 *
 * de la Briandais, René (1959). File searching using variable length keys.
 * Proc. Western J. Computer Conf. pp. 295–298. Cited by Brass.
 *
 * Aho, Alfred V.; Corasick, Margaret J. (June 1975). "Efficient string
 * matching: An aid to bibliographic search". Communications of the ACM.
 * 18 (6): 333–340. doi:10.1145/360825.360855. MR 0371172.
 */
typedef struct node {
    int substring_index;
    int fail;
    int next[26];
} node;
Allocator<node> tree;

int node_count = 1;
bool g_fallback = false;

Allocator<node>::ptr new_node() {
    Allocator<node>::ptr curr = tree.malloc();
    if (curr == _NULL) { g_fallback = true; return _NULL; }
    tree.space[curr].substring_index = -1;
    tree.space[curr].fail = _NULL;
    for (int i = 0; i < 26; i++) {
        tree.space[curr].next[i] = _NULL;
    }
    return curr;
}

/*
 * Insert a new trie node with string as content.
 * The node will be inserted to the trie specified by root.
 *
 * Return: the '%' location of the string.
 */
int insert_node(Allocator<node>::ptr root, char *str, int substring_index) {
    // accumulator for inserted node numbers
    int count = 0;

    // this is a tail recursion, we can transform to loop
    while (true) {
        char ch = *str;

        if (ch == '%') {
            tree.space[root].substring_index = substring_index;
            return count;
        } else {
            assert(ch >= 'a' && ch <= 'z');
            int idx = ch - 'a';
            if (tree.space[root].next[idx] == _NULL) {
                tree.space[root].next[idx] = new_node();
                if (tree.space[root].next[idx] == _NULL) return 0;
                node_count += 1;
            }

            // modify the arguments and go to the start of the function
            root = tree.space[root].next[idx];
            str = str + 1;
            count = count + 1;
            continue;
        }
    }
}

/*
 * Build Aho-Corasick state machine.
 */
void build_AhoCorasick(Allocator<node>::ptr root, int node_count) {
    Allocator<node>::ptr queue[MAX_SIZE];

    // initialize queue
    int head = 0, tail = 1;
    queue[0] = root;

    for (; head < tail; head++) {
        Allocator<node>::ptr curr = queue[head];
        for (int i = 0; i < 26; i++) {
            // non-existent node
            if (tree.space[curr].next[i] == _NULL) continue;

            // Aho-Corasick fail link
            tree.space[tree.space[curr].next[i]].fail = root;
            for (Allocator<node>::ptr p = tree.space[curr].fail;
                    p != _NULL; p = tree.space[p].fail) {
                if (tree.space[p].next[i] != _NULL) {
                    tree.space[tree.space[curr].next[i]].fail =
                        tree.space[p].next[i];
                    break;
                }
            }

            // add to queue
            queue[tail++] = tree.space[curr].next[i];
        }
    }
}

/*
 * Query on Aho-Corasick state machine and write matched indexes.
 */
void query_AhoCorasick(Allocator<node>::ptr root, char *query,
        int *substring_indexes, int *query_indexes) {
    Allocator<node>::ptr curr = root;

    for (int offset = 0; ; offset++) {
        char ch = query[offset];
        if (ch == '%') break;
        assert(ch >= 'a' && ch <= 'z');
        int idx = ch - 'a';

        // follow fail link if not matched in curr
        while ((tree.space[curr].next[idx] == _NULL) && curr != root)
            curr = tree.space[curr].fail;

        // if matched next char
        if (tree.space[curr].next[idx] != _NULL)
            curr = tree.space[curr].next[idx];

        // follow fail link to check matches
        for (Allocator<node>::ptr follow = curr;
                follow != root; follow = tree.space[follow].fail) {
            if (tree.space[follow].substring_index != -1) {
                *substring_indexes++ =
                    tree.space[follow].substring_index;
                *query_indexes++ = offset;
            }
        }
    }

    // end indexes
    *substring_indexes = -1;
    *query_indexes = -1;
}

struct _delete_tree_recursion_record {
    Allocator<node>::ptr root;
    int i;
    int _location;
};

/*
 * Delete the whole tree
 */
void delete_tree(Allocator<node>::ptr root) {
    volatile _delete_tree_recursion_record _stack[MAX_SIZE];
    volatile int _stack_top = 0;

    // initialize the stack
    _stack[_stack_top].root = root;
    _stack[_stack_top]._location = 0;
    _stack_top++;

    while (_stack_top != 0) {
        _stack_top--;
        // restore variables
        root = _stack[_stack_top].root;
        int i = _stack[_stack_top].i;

        // restore location
        int _location = _stack[_stack_top]._location;
        if (_location == 0) goto delete_tree_L0;
        if (_location == 1) goto delete_tree_L1;

delete_tree_L0:
        if (root == _NULL) {
            // function return
            // restore last level function
            continue;
        }
        for (i = 0; i < 26; i++) {
            // function call: delete_tree(tree.space[root].next[i]);
            // store variable
            _stack[_stack_top].root = root;
            _stack[_stack_top].i = i;
            // variable init
            root = tree.space[root].next[i];
            // store location
            _stack[_stack_top]._location = 1;
            // push stack
            _stack_top++;
            // jump to head of the function
            goto delete_tree_L0;

delete_tree_L1:;
        }
        tree.free(root);
    }
}

extern "C" {

/*
 * Implementation of Aho-Corasick Algorithm
 *
 * Search substrings in query, and fill matches in result arrays,
 * Assume all strings contain only letters 'a'-'z'.
 *
 * Parameter Format:
 *   substrings: multiple strings (target substrings) each with '%' ending.
 *               e.g. "string1%string2%", substring_length = 7+1+7+1.
 *   query: a string (the query document) with '%' ending
 *   substring_indexes: an output array, the indexes of found substrings.
 *   query_indexes: an output array, the corresponding indexes in query.
 */
void process_top(
        int *substring_length_p, char *substrings, char *query,
        int *substring_indexes, int *query_indexes, int *fallback) {
#pragma HLS INTERFACE m_axi port=substring_length_p offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=substrings offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=query offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=substring_indexes offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=query_indexes offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=fallback offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=substring_length_p bundle=control
#pragma HLS INTERFACE s_axilite port=substrings bundle=control
#pragma HLS INTERFACE s_axilite port=query bundle=control
#pragma HLS INTERFACE s_axilite port=substring_indexes bundle=control
#pragma HLS INTERFACE s_axilite port=query_indexes bundle=control
#pragma HLS INTERFACE s_axilite port=fallback bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
    int substring_length = *substring_length_p;

    Allocator<node>::ptr root = new_node();
    char substring_buf[MAX_SIZE];
    if (substring_length > MAX_SIZE) {
        g_fallback = true; goto fail;
    }

    for (int i = 0; i < substring_length; i++) {
        substring_buf[i] = substrings[i];
    }

    for (int offset = 0; offset < substring_length; ) {
        offset += insert_node(root, substring_buf + offset, offset) + 1;
        if (g_fallback) goto fail;
    }

    build_AhoCorasick(root, node_count);
    query_AhoCorasick(root, query, substring_indexes, query_indexes);
    delete_tree(root);
    if (g_fallback) goto fail;

fail:
    *fallback = g_fallback;
}

};
