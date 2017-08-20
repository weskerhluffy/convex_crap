#include<stdio.h>
#include<stdlib.h>
#define maxP 300000
typedef struct {
	int x, y;
} Point;
Point P[maxP], CH[maxP << 1];
int cmp(const void *i, const void *j) {
	Point *a, *b;
	a = (Point *) i, b = (Point *) j;
	if (a->y != b->y)
		return a->y - b->y;
	return a->x - b->x;
}
int Cross(Point *o, Point *a, Point *b) {
	return (a->x - o->x) * (b->y - o->y) - (a->y - o->y) * (b->x - o->x);
}
void Print(int N, Point CH[]) {
	int i;
	for (i = 0; i < N; i++)
		printf("%d %d\n", CH[i].x, CH[i].y);
}
void ConvexHull(int N, Point P[]) {
	qsort(P, N, sizeof(Point), cmp);
	int i, m = 0, t;
	for (i = 0; i < N; i++) {
//		printf("puta mierda %d %d\n", P[i].x, P[i].y);
		while (m >= 2 && Cross(&CH[m - 2], &CH[m - 1], &P[i]) <= 0)
			m--;
		CH[m++] = P[i];
	}
	for (int j = 0; j < m; j++) {
//		printf("caca %d %d\n", CH[j].x, CH[j].y);
	}
	for (i = N - 1, t = m + 1; i >= 0; i--) {
//		printf("concha de la lora %d %d\n", P[i].x, P[i].y);
		while (m >= t && Cross(&CH[m - 2], &CH[m - 1], &P[i]) <= 0) {
//			printf("borrado %d %d\n", CH[m - 1].x, CH[m - 1].y);
			m--;
		}
		CH[m++] = P[i];
	}
	printf("%d\n", m);
	Print(m, CH);
}
int main() {
	int K, N, i;
	scanf("%d", &K);
	printf("%d\n", K);
	while (K--) {
		scanf("%d", &N);
		for (i = 0; i < N; i++)
			scanf("%d %d", &P[i].x, &P[i].y);
		if (K)
			scanf("%d", &i);
		ConvexHull(N, P);
		if (K)
			puts("-1");
	}
	return 0;
}
