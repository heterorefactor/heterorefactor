#include <cassert>
#include <cstdlib>

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
struct node {
    int substring_index;
    node *fail;
    node *next[26];
};

int node_count = 1;
bool g_fallback = false;

node *new_node() {
    node *curr = (node *)malloc(sizeof(node));
    if (!curr) { g_fallback = true; return NULL; }
    curr->substring_index = -1;
    curr->fail = NULL;
    for (int i = 0; i < 26; i++) {
        curr->next[i] = NULL;
    }
    return curr;
}

/*
 * Insert a new trie node with string as content.
 * The node will be inserted to the trie specified by root.
 *
 * Return: the '%' location of the string.
 */
int insert_node(node *root, char *str, int substring_index) {
    char ch = *str;

    if (ch == '%') {
        root->substring_index = substring_index;
        return 0;
    } else {
        assert(ch >= 'a' && ch <= 'z');
        int idx = ch - 'a';
        if (!root->next[idx]) {
            root->next[idx] = new_node();
            node_count += 1;
        }

        return insert_node(root->next[idx], str + 1, substring_index) + 1;
    }
}

/*
 * Build Aho-Corasick state machine.
 */
void build_AhoCorasick(node *root, int node_count) {
    node **queue = (node **)malloc(sizeof(node *) * node_count);
    if (!queue) { g_fallback = true; return; }

    // initialize queue
    int head = 0, tail = 1;
    queue[0] = root;

    for (; head < tail; head++) {
        node *curr = queue[head];
        for (int i = 0; i < 26; i++) {
            // non-existent node
            if (!curr->next[i]) continue;

            // Aho-Corasick fail link
            curr->next[i]->fail = root;
            for (node *p = curr->fail; p; p = p->fail) {
                if (p->next[i]) {
                    curr->next[i]->fail = p->next[i];
                    break;
                }
            }

            // add to queue
            queue[tail++] = curr->next[i];
        }
    }

    free(queue);
}

/*
 * Query on Aho-Corasick state machine and write matched indexes.
 */
void query_AhoCorasick(node *root, char *query,
        int *substring_indexes, int *query_indexes) {
    node *curr = root;

    for (int offset = 0; ; offset++) {
        char ch = query[offset];
        if (ch == '%') break;
        assert(ch >= 'a' && ch <= 'z');
        int idx = ch - 'a';

        // follow fail link if not matched in curr
        while (!curr->next[idx] && curr != root)
            curr = curr->fail;

        // if matched next char
        if (curr->next[idx])
            curr = curr->next[idx];

        // follow fail link to check matches
        for (node *follow = curr; follow != root; follow = follow->fail) {
            if (follow->substring_index != -1) {
                *substring_indexes++ = follow->substring_index;
                *query_indexes++ = offset;
            }
        }
    }

    // end indexes
    *substring_indexes = -1;
    *query_indexes = -1;
}

/*
 * Delete the whole tree
 */
void delete_tree(node *root) {
    if (!root) return;
    for (int i = 0; i < 26; i++)
        delete_tree(root->next[i]);
    free(root);
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
void AhoCorasick_search(
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

    node *root = new_node();
    char *substring_buf = (char *)malloc(sizeof(char) * substring_length);
    if (!substring_buf) { g_fallback = true; goto fail; }

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
