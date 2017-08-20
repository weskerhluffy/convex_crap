/**
 * https://www.hackerrank.com/contests/justins-test-contest/challenges/convex-hull-1/problem
 * https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=622
 */
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>
#include<assert.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327
#endif

typedef unsigned int natural;
typedef unsigned long long entero_largo_sin_signo;
typedef long long entero_largo;

typedef struct puto_cardinal {
	union {
		struct {
			int coordenada_y_puto_cardinal;
			int coordenada_x_puto_cardinal;
		} separados_puto_cardinal;
		entero_largo coordenadas_juntas_puto_cardinal;
	} datos_puto_cardinal;
} puto_cardinal;

#define coord_x datos_puto_cardinal.separados_puto_cardinal.coordenada_x_puto_cardinal
#define coord_y datos_puto_cardinal.separados_puto_cardinal.coordenada_y_puto_cardinal
#define coord_xy datos_puto_cardinal.coordenadas_juntas_puto_cardinal

typedef struct puto_polar {
	double anogulo_puto_polar;
	double distancia_puto_polar;
	puto_cardinal *cardinal_puto_polar;
} puto_polar;

#define ASS_MAX_PUTOS 100000

puto_cardinal coordenadas_cartesianas[ASS_MAX_PUTOS] = { 0 };
puto_polar coordenadas_polares[ASS_MAX_PUTOS + 1] = { 0 };
puto_polar coordenadas_polares_tmp[ASS_MAX_PUTOS + 1] = { 0 };
puto_cardinal *coordenadas_cartesianas_filtradas[ASS_MAX_PUTOS] = { 0 };

puto_cardinal *MIN_COORD_CARD = &(puto_cardinal ) { .coord_x = INT_MAX,
				.coord_y = INT_MAX };
puto_polar *MAX_COORD_POLAR = &(puto_polar ) { .distancia_puto_polar = 0 };

static void ass_puto_cartesiano_a_polar(puto_cardinal *pc1, puto_cardinal *pc2,
		puto_polar *pp) {
	float dif_x = (float) pc2->coord_x - (float) pc1->coord_x;
	float dif_y = (float) pc2->coord_y - (float) pc1->coord_y;
	/*
	 printf("q mierda pasa %f %d %d\n", dif_x, pc2->coord_x, pc1->coord_x);
	 */
	pp->distancia_puto_polar = sqrt(dif_x * dif_x + dif_y * dif_y);
	pp->anogulo_puto_polar = atan2(dif_y, dif_x);
	pp->cardinal_puto_polar = pc2;
	/*
	 printf("convertido %d %d a polar %f %f\n", pc2->coord_x, pc2->coord_y,
	 pp->anogulo_puto_polar, pp->distancia_puto_polar);
	 printf("convertido %f %f a polar %f %f\n", dif_x, dif_y,
	 (pp->anogulo_puto_polar * 180) / M_PI, pp->distancia_puto_polar);
	 */
}

static int ass_comparar_puto_polar(puto_polar *ppa, puto_polar *ppb) {
	int resul = 0;
	if (ppa->anogulo_puto_polar < ppb->anogulo_puto_polar) {
		resul = -1;
	} else {
		if (ppa->anogulo_puto_polar > ppb->anogulo_puto_polar) {
			resul = 1;
		} else {
			if (ppa->distancia_puto_polar < ppb->distancia_puto_polar) {
				resul = -1;
			} else {
				if (ppa->distancia_puto_polar > ppb->distancia_puto_polar) {
					resul = 1;
				} else {
					resul = 0;
				}
			}
		}
	}
	return resul;
}

static int ass_comparar_puto_polar_dist(puto_polar *ppa, puto_polar *ppb) {
	int resul = 0;
	if (ppa->distancia_puto_polar < ppb->distancia_puto_polar) {
		resul = -1;
	} else {
		if (ppa->distancia_puto_polar > ppb->distancia_puto_polar) {
			resul = 1;
		} else {
			if (ppa->anogulo_puto_polar < ppb->anogulo_puto_polar) {
				resul = -1;
			} else {
				if (ppa->anogulo_puto_polar > ppb->anogulo_puto_polar) {
					resul = 1;
				} else {
					resul = 0;
				}
			}
		}
	}
	return resul;
}

int ass_comparar_puto_polar_qsort(const void *a, const void *b) {
	return ass_comparar_puto_polar((puto_polar*) a, (puto_polar*) b);
}

static int ass_comparar_puto_polar_inv(puto_polar *ppa, puto_polar *ppb) {
	int resul = 0;
	if (ppa->anogulo_puto_polar < ppb->anogulo_puto_polar) {
		resul = -1;
	} else {
		if (ppa->anogulo_puto_polar > ppb->anogulo_puto_polar) {
			resul = 1;
		} else {
			if (ppa->distancia_puto_polar > ppb->distancia_puto_polar) {
				resul = -1;
			} else {
				if (ppa->distancia_puto_polar < ppb->distancia_puto_polar) {
					resul = 1;
				} else {
					resul = 0;
				}
			}
		}
	}
	return resul;
}

int ass_comparar_puto_polar_inv_qsort(const void *a, const void *b) {
	return ass_comparar_puto_polar_inv((puto_polar*) a, (puto_polar*) b);
}

static char ass_puto_dentro(puto_cardinal *puto, puto_cardinal *segmento_inicio,
		puto_cardinal *segmento_fin) {
	char result = 0;
	double angulo = 0;
	double angulo_puto = 0;

	angulo = atan2(segmento_fin->coord_y - segmento_inicio->coord_y,
			segmento_fin->coord_x - segmento_inicio->coord_x);

	angulo_puto = atan2(puto->coord_y - segmento_inicio->coord_y,
			puto->coord_x - segmento_inicio->coord_x);

	angulo_puto = angulo_puto < 0 ? (M_PI * 2) + angulo_puto : angulo_puto;
	angulo = angulo < 0 ? (M_PI * 2) + angulo : angulo;
	/*
	 printf("angulo puto orig %f\n", angulo_puto * 180 / M_PI);
	 */
	angulo_puto = angulo_puto - angulo + M_PI;
	angulo_puto =
			angulo_puto > (M_PI * 2) ? angulo_puto - (M_PI * 2) : angulo_puto;

	result = !(angulo_puto > 0.0 && angulo_puto < M_PI);
	/*
	 printf(
	 "el anogulo %f (%d %d - %d %d) y el angulo puto %f (%d %d - %d %d)\n",
	 angulo * 180 / M_PI, segmento_inicio->coord_x,
	 segmento_inicio->coord_y, segmento_fin->coord_x,
	 segmento_fin->coord_y, angulo_puto * 180 / M_PI,
	 segmento_inicio->coord_x, segmento_inicio->coord_y, puto->coord_x,
	 puto->coord_y);
	 */
	return result;
}

static void ass_main() {
	natural num_casos = 0;
	natural i, k;

	scanf("%u", &num_casos);

	printf("%u\n", num_casos);
	for (k = 0; k < num_casos; k++) {
		natural num_coor = 0;
		natural cont_coor = 0;
		natural cont_coor_filt = 0;
		puto_cardinal *min_card = MIN_COORD_CARD;
		puto_polar max_pol = *MAX_COORD_POLAR;
		puto_polar *puto_polar_tmp = NULL;
		natural coord_filtradas_tam = 0;
		int shit;
		char todos_iguales = 0;
		scanf("%u", &num_coor);

		for (i = 0; i < num_coor; i++) {
			int coord_x_in = 0;
			int coord_y_in = 0;

			scanf("%d %d", &coord_x_in, &coord_y_in);

			puto_cardinal *cur_puto = coordenadas_cartesianas + cont_coor++;
			cur_puto->coord_x = coord_x_in;
			cur_puto->coord_y = coord_y_in;
			/*
			 printf("en i %u x %d(%x) y %d(%x) x-y %lld (%llx)\n", cont_coor - 1,
			 cur_puto->coord_x, cur_puto->coord_x, cur_puto->coord_y,
			 cur_puto->coord_y, cur_puto->coord_xy, cur_puto->coord_xy);
			 */
			if ((min_card->coord_y > cur_puto->coord_y)
					|| (min_card->coord_y == cur_puto->coord_y
							&& min_card->coord_x > cur_puto->coord_x)) {
				min_card = cur_puto;
			}
			if (i > 1) {
				puto_cardinal *ant_puto = coordenadas_cartesianas + cont_coor
						- 2;
				todos_iguales = cur_puto->coord_xy == ant_puto->coord_xy;
			}
		}
		/*
		 printf("el min ess %d %d\n", min_card->coord_x, min_card->coord_y);
		 if (todos_iguales) {
		 abort();
		 }
		 */
		if (k < num_casos - 1) {
			scanf("%d", &shit);
		}

		for (i = 0; i < cont_coor; i++) {
			puto_cardinal *cur_puto = coordenadas_cartesianas + i;
			puto_polar *cur_puto_pol = coordenadas_polares_tmp + i;

			ass_puto_cartesiano_a_polar(min_card, cur_puto, cur_puto_pol);
			/*
			 printf("ano %f dist %f\n", cur_puto_pol->anogulo_puto_polar,
			 cur_puto_pol->distancia_puto_polar);
			 */
			if (ass_comparar_puto_polar_dist(&max_pol, cur_puto_pol) <= 0) {
				max_pol = *cur_puto_pol;
			}
		}

		qsort(coordenadas_polares_tmp, cont_coor, sizeof(puto_polar),
				ass_comparar_puto_polar_qsort);

		natural idx_max_pol = -1;
		for (i = 0; i < cont_coor; i++) {
			puto_polar *cur_puto_pol_filt = NULL;
			puto_polar *cur_puto_pol = coordenadas_polares_tmp + i;
			/*
			 printf("%p ano1 %f dist %f\n", cur_puto_pol,
			 cur_puto_pol->anogulo_puto_polar,
			 cur_puto_pol->distancia_puto_polar);
			 */
			if (cur_puto_pol->cardinal_puto_polar->coord_xy
					== min_card->coord_xy) {
				continue;
			}
			if (i) {
				puto_polar *ant_puto_pol = coordenadas_polares_tmp + i - 1;
				if (ant_puto_pol->cardinal_puto_polar->coord_xy
						== cur_puto_pol->cardinal_puto_polar->coord_xy) {
					continue;
				}
			}
			cur_puto_pol_filt = coordenadas_polares + cont_coor_filt++;
			*cur_puto_pol_filt = *cur_puto_pol;
			if (cur_puto_pol->anogulo_puto_polar == max_pol.anogulo_puto_polar
					&& idx_max_pol == -1) {
				idx_max_pol = cont_coor_filt - 1;
			}
		}
		assert(idx_max_pol != -1);
		cont_coor = cont_coor_filt;
		assert(idx_max_pol < cont_coor);

		qsort(coordenadas_polares + idx_max_pol, cont_coor - idx_max_pol,
				sizeof(puto_polar), ass_comparar_puto_polar_inv_qsort);

		/*
		 printf("el mas lejano %d %d\n",
		 (coordenadas_polares + idx_max_pol)->cardinal_puto_polar->coord_x,
		 (coordenadas_polares + idx_max_pol)->cardinal_puto_polar->coord_y);
		 */

		puto_polar_tmp = coordenadas_polares + cont_coor;
		puto_polar_tmp->cardinal_puto_polar = min_card;

		coordenadas_cartesianas_filtradas[coord_filtradas_tam] = min_card;
		for (i = 0, coord_filtradas_tam = 1;
				coord_filtradas_tam < 2 && i < cont_coor;
				i++, coord_filtradas_tam++) {
			coordenadas_cartesianas_filtradas[coord_filtradas_tam] =
					(coordenadas_polares + i)->cardinal_puto_polar;
			/*
			 printf("puta madre %u %d %d de %u\n", coord_filtradas_tam,
			 coordenadas_cartesianas_filtradas[coord_filtradas_tam]->coord_x,
			 coordenadas_cartesianas_filtradas[coord_filtradas_tam]->coord_y,
			 i);
			 */
		}

		for (; cont_coor > 2 && i <= cont_coor; i++) {
			puto_cardinal *segmento_fin = NULL;

			segmento_fin = (coordenadas_polares + i)->cardinal_puto_polar;
			/*
			 printf("mierda %u %d %d\n", i, segmento_fin->coord_x,
			 segmento_fin->coord_y);
			 printf("puta madre, el ano actual %f dist %f\n",
			 (coordenadas_polares + i)->anogulo_puto_polar,
			 (coordenadas_polares + i)->distancia_puto_polar);
			 */
			while (coord_filtradas_tam >= 2) {
				puto_cardinal *segmento_ini = NULL;
				puto_cardinal *puto = NULL;
				char puto_dentro = 0;
				segmento_ini =
						coordenadas_cartesianas_filtradas[coord_filtradas_tam
								- 2];
				puto =
						coordenadas_cartesianas_filtradas[coord_filtradas_tam
								- 1];

				puto_dentro = ass_puto_dentro(puto, segmento_ini, segmento_fin);

				if (!puto_dentro) {
					break;
				}
				/*
				 printf("eliminando %d %d debido a %d %d\n", puto->coord_x,
				 puto->coord_y, segmento_fin->coord_x,
				 segmento_fin->coord_y);
				 */
				coord_filtradas_tam--;
			}

			if (i < cont_coor) {
				coordenadas_cartesianas_filtradas[coord_filtradas_tam++] =
						segmento_fin;
			}
		}

		printf("%u\n", coord_filtradas_tam + 1);
		for (i = 0; i < coord_filtradas_tam; i++) {
			puto_cardinal *cur_puto = coordenadas_cartesianas_filtradas[i];
			printf("%d %d\n", cur_puto->coord_x, cur_puto->coord_y);
		}
		printf("%d %d\n", min_card->coord_x, min_card->coord_y);
		if (k < num_casos - 1) {
			printf("%d\n", -1);
		} else {
		}
	}
}

int main(void) {
	ass_main();
	return EXIT_SUCCESS;
}
