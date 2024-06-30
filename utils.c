#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "./utils.h"
#include "./lib/include/avl.h"
#include "./structs.h"

void int_ou_null(void *reg, char min[10]) {
    if (reg == NULL) {
        strcpy(min, "NULL");
    } else {
        sprintf(min, "%d", ((tint *)reg)->valor);
    }
}

void double_ou_null(void *reg, char min[50]) {
    if (reg == NULL) {
        strcpy(min, "NULL");
    } else {
        sprintf(min, "%.2f", ((tdouble *)reg)->valor);
    }
}

void string_ou_null(void *reg, char min[200]) {
    if (reg == NULL) {
        strcpy(min, "NULL");
    } else {
        strcpy(min, ((tstring *)reg)->nome);
    }
}

void _free_null(void *reg) {
    if (reg != NULL) {
        free(reg);
    }
}

void le_int(tint **i1, tint **i2, tintervalo *query, char nome[100], tarv *avl) {
    char min[100], max[100];
    printf("Digite o valor mínimo de %s: ", nome);
    scanf("%s", min);
    if (strcasecmp(min, "NULL") == 0) {
        *i1 = NULL;
    } else {
        *i1 = aloca_int(atoi(min), "");
    }
    printf("Digite o valor máximo de %s: ", nome);
    scanf("%s", max);
    if (strcasecmp(max, "NULL") == 0) {
        *i2 = NULL;
    } else {
        *i2 = aloca_int(atoi(max), "");
    }

    _free_null(query->reg_min);
    _free_null(query->reg_max);

    query->reg_min = *i1;
    query->reg_max = *i2;
    query->lista = avl_range(avl, query->reg_min, query->reg_max);
}

void le_double(tdouble **d1, tdouble **d2, tintervalo *query, char nome[100], tarv *avl) {
    char min[100], max[100];
    printf("Digite o valor mínimo de %s: ", nome);
    scanf("%s", min);
    if (strcasecmp(min, "NULL") == 0) {
        *d1 = NULL;
    } else {
        *d1 = aloca_double(atof(min), "");
    }
    printf("Digite o valor máximo de %s: ", nome);
    scanf("%s", max);
    if (strcasecmp(max, "NULL") == 0) {
        *d2 = NULL;
    } else {
        *d2 = aloca_double(atof(max), "");
    }

    _free_null(query->reg_min);
    _free_null(query->reg_max);

    query->reg_min = *d1;
    query->reg_max = *d2;
    query->lista = avl_range(avl, query->reg_min, query->reg_max);
}

void le_string(tstring **s1, tstring **s2, tintervalo *query, char nome[100], tarv *avl) {
    char min[100], max[100];
    printf("Digite o valor mínimo de %s: ", nome);
    scanf("%s", min);
    if (strcmp(min, "NULL") == 0) {
        *s1 = NULL;
    } else {
        *s1 = aloca_string(min, "");
    }
    printf("Digite o valor máximo de %s: ", nome);
    scanf("%s", max);
    if (strcasecmp(max, "NULL") == 0) {
        *s2 = NULL;
    } else {
        *s2 = aloca_string(max, "");
    }

    _free_null(query->reg_min);
    _free_null(query->reg_max);

    query->reg_min = *s1;
    query->reg_max = *s2;
    query->lista = avl_range(avl, query->reg_min, query->reg_max);
}