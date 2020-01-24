#include <stdio.h>

extern "C" {
void process_top(int *np, int *lp, int *mp,
		int *mat1, int *mat2, int *mat3, int *fallback);
};

int main(int argc, char** argv) {
    int n, l, m;
    scanf("%d%d%d", &n, &l, &m);

	int* matA = new int[n * l];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < l; j++) {
            scanf("%d", &matA[i * l + j]);
        }
    }
	int* matB = new int[l * m];
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &matB[i * m + j]);
        }
    }
    int* matC = new int[n * m];
    int fallback = 0;
    process_top(&n, &l, &m, matA, matB, matC, &fallback);

    if (fallback) {
        printf("The kernel reports fallback.\n");
    } else {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                printf("%d ", matC[i * m + j]);
            }
            printf("\n");
        }
    }

    return 0;
}
