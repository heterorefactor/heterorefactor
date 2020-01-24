// CPP program to implement Strassen’s Matrix
// Multiplication Algorithm
// from https://www.geeksforgeeks.org/strassens-matrix-multiplication-algorithm-implementation/
#include <cstdlib>

bool g_fallback = false;
int** Strassen(int** a, int** b, int n, int l, int m);

extern "C" {
void process_top(int *np, int *lp, int *mp,
		int *mat1, int *mat2, int *mat3, int *fallback) {
#pragma HLS INTERFACE m_axi port=np offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=lp offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=mp offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=mat1 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=mat2 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=mat3 offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=fallback offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=np bundle=control
#pragma HLS INTERFACE s_axilite port=lp bundle=control
#pragma HLS INTERFACE s_axilite port=mp bundle=control
#pragma HLS INTERFACE s_axilite port=mat1 bundle=control
#pragma HLS INTERFACE s_axilite port=mat2 bundle=control
#pragma HLS INTERFACE s_axilite port=mat3 bundle=control
#pragma HLS INTERFACE s_axilite port=fallback bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control
    int n = *np, l = *lp, m = *mp;
	int **matA = (int**)malloc(sizeof(int*) * n), **matB, **matC;
	if (!matA) { g_fallback = true; goto fail; }
	for (int i = 0; i < n; i++) {
		matA[i] = (int*)malloc(sizeof(int) * l);
		if (!matA[i]) { g_fallback = true; goto fail; }
		for (int j = 0; j < l; j++)
			matA[i][j] = mat1[i * l + j];
	}

	matB = (int**)malloc(sizeof(int*) * l);
	if (!matB) { g_fallback = true; goto fail; }
	for (int i = 0; i < l; i++) {
		matB[i] = (int*)malloc(sizeof(int) * m);
		if (!matB[i]) { g_fallback = true; goto fail; }
		for (int j = 0; j < m; j++)
			matB[i][j] = mat2[i * m + j];
	}

	matC = Strassen(matA, matB, n, l, m);
    if (g_fallback) goto fail;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			mat3[i * m + j] = matC[i][j];
		}
		free(matC[i]);
	}
	free(matC);

fail:
    *fallback = g_fallback;
}
};

// Strassen's Algorithm for matrix multiplication
// Complexity: O(n^2.808)
int** MatrixMultiply(int** a, int** b, int n, int l, int m)
{
	int** c = (int**)malloc(sizeof(int*) * n);
	if (!c) { g_fallback = true; return NULL; }
	for (int i = 0; i < n; i++) {
		c[i] = (int*)malloc(sizeof(int) * m);
        if (!c[i]) { g_fallback = true; return NULL; }
    }

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			c[i][j] = 0;
			for (int k = 0; k < l; k++) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return c;
}

int** Strassen(int** a, int** b, int n, int l, int m)
{
	if (n == 1 || l == 1 || m == 1)
		return MatrixMultiply(a, b, n, l, m);

	int** c = (int**)malloc(sizeof(int*) * n);
	if (!c) { g_fallback = true; return NULL; }
	for (int i = 0; i < n; i++) {
		c[i] = (int*)malloc(sizeof(int) * m);
		if (!c[i]) { g_fallback = true; return NULL; }
	}

	int adjN = (n >> 1) + (n & 1);
	int adjL = (l >> 1) + (l & 1);
	int adjM = (m >> 1) + (m & 1);

	int**** As = (int****)malloc(sizeof(int***) * 2);
	if (!As) { g_fallback = true; return NULL; }
	for (int x = 0; x < 2; x++) {
		As[x] = (int***)malloc(sizeof(int**) * 2);
		if (!As[x]) { g_fallback = true; return NULL; }
		for (int y = 0; y < 2; y++) {
			As[x][y] = (int**)malloc(sizeof(int*) * adjN);
			if (!As[x][y]) { g_fallback = true; return NULL; }
			for (int i = 0; i < adjN; i++) {
				As[x][y][i] = (int*)malloc(sizeof(int) * adjL);
				if (!As[x][y][i]) { g_fallback = true; return NULL; }
				for (int j = 0; j < adjL; j++) {
					int I = i + (x & 1) * adjN;
					int J = j + (y & 1) * adjL;
					As[x][y][i][j] = (I < n && J < l) ? a[I][J] : 0;
				}
			}
		}
	}

	int**** Bs = (int****)malloc(sizeof(int***) * 2);
	if (!Bs) { g_fallback = true; return NULL; }
	for (int x = 0; x < 2; x++) {
		Bs[x] = (int***)malloc(sizeof(int**) * 2);
		if (!Bs[x]) { g_fallback = true; return NULL; }
		for (int y = 0; y < 2; y++) {
			Bs[x][y] = (int**)malloc(sizeof(int*) * adjN);
			if (!Bs[x][y]) { g_fallback = true; return NULL; }
			for (int i = 0; i < adjL; i++) {
				Bs[x][y][i] = (int*)malloc(sizeof(int) * adjM);
				if (!Bs[x][y][i]) { g_fallback = true; return NULL; }
				for (int j = 0; j < adjM; j++) {
					int I = i + (x & 1) * adjL;
					int J = j + (y & 1) * adjM;
					Bs[x][y][i][j] = (I < l && J < m) ? b[I][J] : 0;
				}
			}
		}
	}

	int*** s = (int***)malloc(sizeof(int**) * 10);
	if (!s) { g_fallback = true; return NULL; }
	for (int i = 0; i < 10; i++) {
		switch (i) {
		case 0:
			s[i] = (int**)malloc(sizeof(int*) * adjL);
			if (!s[i]) { g_fallback = true; return NULL; }
			for (int j = 0; j < adjL; j++) {
				s[i][j] = (int*)malloc(sizeof(int) * adjM);
				if (!s[i][j]) { g_fallback = true; return NULL; }
				for (int k = 0; k < adjM; k++) {
					s[i][j][k] = Bs[0][1][j][k] - Bs[1][1][j][k];
				}
			}
			break;
		case 1:
			s[i] = (int**)malloc(sizeof(int*) * adjN);
			if (!s[i]) { g_fallback = true; return NULL; }
			for (int j = 0; j < adjN; j++) {
				s[i][j] = (int*)malloc(sizeof(int) * adjL);
				if (!s[i][j]) { g_fallback = true; return NULL; }
				for (int k = 0; k < adjL; k++) {
					s[i][j][k] = As[0][0][j][k] + As[0][1][j][k];
				}
			}
			break;
		case 2:
			s[i] = (int**)malloc(sizeof(int*) * adjN);
			if (!s[i]) { g_fallback = true; return NULL; }
			for (int j = 0; j < adjN; j++) {
				s[i][j] = (int*)malloc(sizeof(int) * adjL);
				if (!s[i][j]) { g_fallback = true; return NULL; }
				for (int k = 0; k < adjL; k++) {
					s[i][j][k] = As[1][0][j][k] + As[1][1][j][k];
				}
			}
			break;
		case 3:
			s[i] = (int**)malloc(sizeof(int*) * adjL);
			if (!s[i]) { g_fallback = true; return NULL; }
			for (int j = 0; j < adjL; j++) {
				s[i][j] = (int*)malloc(sizeof(int) * adjM);
				if (!s[i][j]) { g_fallback = true; return NULL; }
				for (int k = 0; k < adjM; k++) {
					s[i][j][k] = Bs[1][0][j][k] - Bs[0][0][j][k];
				}
			}
			break;
		case 4:
			s[i] = (int**)malloc(sizeof(int*) * adjN);
			if (!s[i]) { g_fallback = true; return NULL; }
			for (int j = 0; j < adjN; j++) {
				s[i][j] = (int*)malloc(sizeof(int) * adjL);
				if (!s[i][j]) { g_fallback = true; return NULL; }
				for (int k = 0; k < adjL; k++) {
					s[i][j][k] = As[0][0][j][k] + As[1][1][j][k];
				}
			}
			break;
		case 5:
			s[i] = (int**)malloc(sizeof(int*) * adjL);
			if (!s[i]) { g_fallback = true; return NULL; }
			for (int j = 0; j < adjL; j++) {
				s[i][j] = (int*)malloc(sizeof(int) * adjM);
				if (!s[i][j]) { g_fallback = true; return NULL; }
				for (int k = 0; k < adjM; k++) {
					s[i][j][k] = Bs[0][0][j][k] + Bs[1][1][j][k];
				}
			}
			break;
		case 6:
			s[i] = (int**)malloc(sizeof(int*) * adjN);
			if (!s[i]) { g_fallback = true; return NULL; }
			for (int j = 0; j < adjN; j++) {
				s[i][j] = (int*)malloc(sizeof(int) * adjL);
				if (!s[i][j]) { g_fallback = true; return NULL; }
				for (int k = 0; k < adjL; k++) {
					s[i][j][k] = As[0][1][j][k] - As[1][1][j][k];
				}
			}
			break;
		case 7:
			s[i] = (int**)malloc(sizeof(int*) * adjL);
			if (!s[i]) { g_fallback = true; return NULL; }
			for (int j = 0; j < adjL; j++) {
				s[i][j] = (int*)malloc(sizeof(int) * adjM);
				if (!s[i][j]) { g_fallback = true; return NULL; }
				for (int k = 0; k < adjM; k++) {
					s[i][j][k] = Bs[1][0][j][k] + Bs[1][1][j][k];
				}
			}
			break;
		case 8:
			s[i] = (int**)malloc(sizeof(int*) * adjN);
			if (!s[i]) { g_fallback = true; return NULL; }
			for (int j = 0; j < adjN; j++) {
				s[i][j] = (int*)malloc(sizeof(int) * adjL);
				if (!s[i][j]) { g_fallback = true; return NULL; }
				for (int k = 0; k < adjL; k++) {
					s[i][j][k] = As[0][0][j][k] - As[1][0][j][k];
				}
			}
			break;
		case 9:
			s[i] = (int**)malloc(sizeof(int*) * adjL);
			if (!s[i]) { g_fallback = true; return NULL; }
			for (int j = 0; j < adjL; j++) {
				s[i][j] = (int*)malloc(sizeof(int) * adjM);
				if (!s[i][j]) { g_fallback = true; return NULL; }
				for (int k = 0; k < adjM; k++) {
					s[i][j][k] = Bs[0][0][j][k] + Bs[0][1][j][k];
				}
			}
			break;
		}
	}

	int*** p = (int***)malloc(sizeof(int**) * 7);
	if (!p) { g_fallback = true; return NULL; }
	p[0] = Strassen(As[0][0], s[0], adjN, adjL, adjM);
    if (g_fallback) return NULL;
	p[1] = Strassen(s[1], Bs[1][1], adjN, adjL, adjM);
    if (g_fallback) return NULL;
	p[2] = Strassen(s[2], Bs[0][0], adjN, adjL, adjM);
    if (g_fallback) return NULL;
	p[3] = Strassen(As[1][1], s[3], adjN, adjL, adjM);
    if (g_fallback) return NULL;
	p[4] = Strassen(s[4], s[5], adjN, adjL, adjM);
    if (g_fallback) return NULL;
	p[5] = Strassen(s[6], s[7], adjN, adjL, adjM);
    if (g_fallback) return NULL;
	p[6] = Strassen(s[8], s[9], adjN, adjL, adjM);
    if (g_fallback) return NULL;

	for (int i = 0; i < adjN; i++) {
		for (int j = 0; j < adjM; j++) {
			c[i][j] = p[4][i][j] + p[3][i][j] - p[1][i][j] + p[5][i][j];
			if (j + adjM < m)
				c[i][j + adjM] = p[0][i][j] + p[1][i][j];
			if (i + adjN < n)
				c[i + adjN][j] = p[2][i][j] + p[3][i][j];
			if (i + adjN < n && j + adjM < m)
				c[i + adjN][j + adjM] = p[4][i][j] + p[0][i][j] - p[2][i][j] - p[6][i][j];
		}
	}

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int i = 0; i < adjN; i++) {
				free(As[x][y][i]);
			}
			free(As[x][y]);
		}
		free(As[x]);
	}
	free(As);

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int i = 0; i < adjL; i++) {
				free(Bs[x][y][i]);
			}
			free(Bs[x][y]);
		}
		free(Bs[x]);
	}
	free(Bs);

	for (int i = 0; i < 10; i++) {
		switch (i) {
		case 0:
		case 3:
		case 5:
		case 7:
		case 9:
			for (int j = 0; j < adjL; j++) {
				free(s[i][j]);
			}
			break;
		case 1:
		case 2:
		case 4:
		case 6:
		case 8:
			for (int j = 0; j < adjN; j++) {
				free(s[i][j]);
			}
			break;
		}
		free(s[i]);
	}
	free(s);

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < (n >> 1); j++) {
			free(p[i][j]);
		}
		free(p[i]);
	}
	free(p);

	return c;
}
