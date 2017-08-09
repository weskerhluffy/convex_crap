#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>
#include<assert.h>

typedef unsigned int natural;
typedef unsigned long long entero_largo_sin_signo;

typedef struct puto_cardinal {
	union {
		struct {
			int coordenada_x_puto_cardinal;
			int coordenada_y_puto_cardinal;
		} separados_puto_cardinal;
		entero_largo_sin_signo coordenadas_juntas_puto_cardinal;
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
puto_polar coordenadas_polares[ASS_MAX_PUTOS] = { 0 };

puto_cardinal *MIN_COORD_CARD = &(puto_cardinal ) { .coord_xy = LONG_MAX };
puto_polar *MAX_COORD_POLAR = &(puto_polar ) { .distancia_puto_polar = 0 };

static inline void ass_puto_cartesiano_a_polar(puto_cardinal *pc1,
		puto_cardinal *pc2, puto_polar *pp) {
	int dif_x = pc2->coord_x - pc1->coord_x;
	int dif_y = pc2->coord_y - pc1->coord_y;
	pp->distancia_puto_polar = sqrt(dif_x * dif_x + dif_y * dif_y);
	pp->anogulo_puto_polar = atan2(dif_y, dif_x);
	pp->cardinal_puto_polar = pc2;
}

static inline int ass_comparar_puto_polar(puto_polar *ppa, puto_polar *ppb) {
	if (ppa->anogulo_puto_polar == ppb->anogulo_puto_polar) {
		return ppa->distancia_puto_polar - ppb->distancia_puto_polar;
	}
	return ppa->anogulo_puto_polar - ppb->anogulo_puto_polar;
}

static inline int ass_comparar_puto_polar_dist(puto_polar *ppa, puto_polar *ppb) {
	if (ppa->distancia_puto_polar == ppb->distancia_puto_polar) {
		return ppa->anogulo_puto_polar - ppb->anogulo_puto_polar;
	}
	return ppa->distancia_puto_polar - ppb->distancia_puto_polar;
}

int ass_comparar_puto_polar_qsort(const void *a, const void *b) {
	return ass_comparar_puto_polar((puto_polar*) a, (puto_polar*) b);
}

static inline int ass_comparar_puto_polar_inv(puto_polar *ppa, puto_polar *ppb) {
	if (ppa->anogulo_puto_polar == ppb->anogulo_puto_polar) {
		return (ppa->distancia_puto_polar - ppb->distancia_puto_polar) * -1;
	}
	return ppa->anogulo_puto_polar - ppb->anogulo_puto_polar;
}

int ass_comparar_puto_polar_inv_qsort(const void *a, const void *b) {
	return ass_comparar_puto_polar_inv((puto_polar*) a, (puto_polar*) b);
}

static inline void ass_main() {
	natural num_casos = 0;

	scanf("%u\n", &num_casos);

	for (natural k = 0; k < num_casos; k++) {
		natural num_coor = 0;
		natural cont_coor = 0;
		puto_cardinal *min_card = MIN_COORD_CARD;
		puto_polar max_pol = *MAX_COORD_POLAR;
		scanf("%u\n", &num_coor);

		for (natural i = 0; i < num_coor; i++) {
			int coord_x_in = 0;
			int coord_y_in = 0;
			char caca = 0;

			scanf("%d %d %c\n", &coord_x_in, &coord_y_in, &caca);

			if (caca == 'Y') {
				puto_cardinal *cur_puto = coordenadas_cartesianas + cont_coor++;
				cur_puto->coord_x = coord_x_in;
				cur_puto->coord_y = coord_y_in;
				if (cur_puto->coord_xy < min_card->coord_xy) {
					min_card = cur_puto;
				}
			}
		}

		for (natural i = 0; i < cont_coor; i++) {
			puto_cardinal *cur_puto = coordenadas_cartesianas + i;
			puto_polar *cur_puto_pol = coordenadas_polares + i;

			ass_puto_cartesiano_a_polar(min_card, cur_puto, cur_puto_pol);
			if (ass_comparar_puto_polar_dist(&max_pol, cur_puto_pol) <= 0) {
				max_pol = *cur_puto_pol;
			}
		}

		qsort(coordenadas_polares, cont_coor, sizeof(puto_polar),
				ass_comparar_puto_polar_qsort);

		natural idx_max_pol = -1;
		for (natural i = 0; i < cont_coor; i++) {
			puto_polar *cur_puto_pol = coordenadas_polares + i;
			if (cur_puto_pol->cardinal_puto_polar
					== max_pol.cardinal_puto_polar) {
				idx_max_pol = i;
				break;
			}
		}
		assert(idx_max_pol != -1);
		if (idx_max_pol + 1 < cont_coor) {
			qsort(coordenadas_polares + idx_max_pol + 1, cont_coor,
					sizeof(puto_polar), ass_comparar_puto_polar_inv_qsort);
		}

		for (natural i = 0; i < cont_coor; i++) {
			puto_polar *cur_puto_pol = coordenadas_polares + i;
			puto_cardinal *cur_puto = cur_puto_pol->cardinal_puto_polar;
			printf("%d %d\n", cur_puto->coord_x, cur_puto->coord_y);
		}
	}
}

int main(void) {
	ass_main();
	return EXIT_SUCCESS;
}