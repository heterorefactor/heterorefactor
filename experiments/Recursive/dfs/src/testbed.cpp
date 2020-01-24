#include <stdio.h>

extern "C" {
void process_top(int n, int *input, int *output, int *fallback);
};

int main(int argc, char** argv) {
    int n;
    scanf("%d", &n);

    int input[n];
    int results[n];
    for (int i = 0; i < n; i++) {
        scanf("%d", &input[i]);
    }

    int fallback = 0;
    process_top(n, input, results, &fallback);

    if (fallback) {
        printf("The kernel reports fallback.\n");
    } else {
        for (int i = 0; i < n; i++) {
            printf("%d ", results[i]);
        }
        printf("\n");
    }

    return 0;
}
