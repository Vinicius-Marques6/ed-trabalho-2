#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "./structs.h"

char *get_cod(void *reg) {
    return ((tmunicipio * )reg)->codigo_ibge;
}

double cmp_nome(void *a, void *b) {
    return strncasecmp(((tstring *) a)->nome, ((tstring *) b)->nome, 1);
}

double cmp_codigo_ibge(void *a, void *b) {
    return strcmp(((tstring *) a)->codigo_ibge, ((tstring *) b)->codigo_ibge);
}

double cmp_double(void *a, void *b) {
    return ((tdouble *) a)->valor - ((tdouble *) b)->valor;
}

double cmp_int(void *a, void *b) {
    return ((tint *) a)->valor - ((tint *) b)->valor;
}

tstring *aloca_string(char *nome, char *codigo_ibge) {
    tstring *s = (tstring *) malloc(sizeof(tstring));
    if (s == NULL) {
        printf("Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
    strcpy(s->nome, nome);
    strcpy(s->codigo_ibge, codigo_ibge);
    return s;
}

tdouble *aloca_double(double valor, char *codigo_ibge) {
    tdouble *d = (tdouble *) malloc(sizeof(tdouble));
    if (d == NULL) {
        printf("Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
    d->valor = valor;
    strcpy(d->codigo_ibge, codigo_ibge);
    return d;
}

tint *aloca_int(int valor, char *codigo_ibge) {
    tint *i = (tint *) malloc(sizeof(tint));
    if (i == NULL) {
        printf("Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
    i->valor = valor;
    strcpy(i->codigo_ibge, codigo_ibge);
    return i;
}

void aloca_municipio(void **municipio) {
    *municipio = (tmunicipio *) malloc(sizeof(tmunicipio));
    if (*municipio == NULL) {
        printf("Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
}