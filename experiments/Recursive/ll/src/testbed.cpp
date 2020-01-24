#include <stdio.h>

extern "C" {
void process_top(int n, int *input, int *output, bool *fallback);
};

int main(int argc, char** argv) {
    int n;
    scanf("%d", &n);

    int input[n];
    int results[4 * n + 5] = {0};
    for (int i = 0; i < n; i++) {
        scanf("%d", &input[i]);
    }

    bool fallback;
    process_top(n, input, results, &fallback);

    if (fallback) {
        printf("The kernel reports fallback.\n");
    } else {
        for (int i = 0; i < 4 * n + 5; i++) {
            if (results[i] == -1) printf("\n");
            else printf("%d ", results[i]);
        }
    }

    return 0;
}
