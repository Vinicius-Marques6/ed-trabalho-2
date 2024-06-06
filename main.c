#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "./lib/include/json.h"
#include "./lib/include/hash.h"
#include "./lib/include/avl.h"
#include "./structs.h"

/* ANSI escape codes
RED     "\x1b[31m"
GREEN   "\x1b[32m"
YELLOW  "\x1b[33m"
BLUE    "\x1b[34m"
MAGENTA "\x1b[35m"
CYAN    "\x1b[36m"
RESET   "\x1b[0m"
*/

#define BOLD "\x1b[1m"
#define NO_BOLD "\x1b[22m"
#define COR_BARRA "\x1b[33m"
#define COR_TITULO "\x1b[34m"
#define COR_AVISO "\x1b[33m"
#define COR_ERRO "\x1b[31m"
#define COR_RESET "\x1b[0m"

#define Coluna(...) printf("%s|%s", COR_BARRA, COR_RESET);\
printf(__VA_ARGS__);

#define Error(x) printf("%s%s%s", COR_ERRO, x, COR_RESET);

#define NAO_ENCONTRADO "Município não encontrado\n"

char *get_cod(void *reg) {
    return ((tmunicipio * )reg)->codigo_ibge;
}

double cmp_nome(void *a, void *b) {
    return strcmp(((tstring *) a)->nome, ((tstring *) b)->nome);
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

int isEqual(void *reg, const char *key) {
    return strcmp(get_cod(reg), key) == 0;
}

void aloca_municipio(void **municipio) {
    *municipio = (tmunicipio *) malloc(sizeof(tmunicipio));
    if (*municipio == NULL) {
        printf("Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }
}

void insere(void *municipio, char *key, char *value) {
    tmunicipio *m = (tmunicipio *) municipio;
    if (isEqual(key, "codigo_ibge")) {
        strcpy(m->codigo_ibge, value);
    } else if (isEqual(key, "nome")) {
        strcpy(m->nome, value);
    } else if (isEqual(key, "latitude")) {
        m->latitude = atof(value);
    } else if (isEqual(key, "longitude")) {
        m->longitude = atof(value);
    } else if (isEqual(key, "capital")) {
        m->capital = atoi(value);
    } else if (isEqual(key, "codigo_uf")) {
        m->codigo_uf = atoi(value);
    } else if (isEqual(key, "siafi_id")) {
        m->siafi_id = atoi(value);
    } else if (isEqual(key, "ddd")) {
        m->ddd = atoi(value);
    } else if (isEqual(key, "fuso_horario")) {
        strcpy(m->fuso_horario, value);
    }
}

void salva(void *reg, void *dest[]) {
    hash_insere((thash *) dest[0], reg);
    avl_insere((tarv *) dest[1], aloca_string(((tmunicipio *) reg)->nome, ((tmunicipio *) reg)->codigo_ibge));
    avl_insere((tarv *) dest[2], aloca_double(((tmunicipio *) reg)->latitude, ((tmunicipio *) reg)->codigo_ibge));
    avl_insere((tarv *) dest[3], aloca_double(((tmunicipio *) reg)->longitude, ((tmunicipio *) reg)->codigo_ibge));
    avl_insere((tarv *) dest[4], aloca_int(((tmunicipio *) reg)->codigo_uf, ((tmunicipio *) reg)->codigo_ibge));
    avl_insere((tarv *) dest[5], aloca_int(((tmunicipio *) reg)->ddd, ((tmunicipio *) reg)->codigo_ibge));    
}

void imprime_municipio(thash hash,char *chave) {
    tmunicipio *m = (tmunicipio *) hash_busca(hash, chave);
    Coluna(" %*s ", 9, m->codigo_ibge);
    Coluna(" %*s ", 20, m->nome);
    Coluna(" %*f ", 10, m->latitude);
    Coluna(" %*f ", 10, m->longitude);
    Coluna(" %*d ", 7, m->capital);
    Coluna(" %*d ", 7, m->codigo_uf);
    Coluna(" %*d ", 8, m->siafi_id);
    Coluna(" %*d ", 3, m->ddd);
    Coluna(" %*s ", 20, m->fuso_horario);
    Coluna("\n");
}

int main(int argc, char *argv[]) {
    char *filename = "./municipios.json";
    if (argc != 1) {
        filename = argv[1];
    }

    thash hash_cod;
    tarv arv_nome, arv_latitude, arv_longitude, arv_uf, arv_ddd;
    hash_constroi(&hash_cod, 10006, get_cod);
    avl_constroi(&arv_nome, cmp_nome);
    avl_constroi(&arv_latitude, cmp_double);
    avl_constroi(&arv_longitude, cmp_double);
    avl_constroi(&arv_uf, cmp_int);
    avl_constroi(&arv_ddd, cmp_int);

    FILE *fmunicipios;

    fmunicipios = fopen(filename, "r");

    if (fmunicipios == NULL) {
        printf("%sErro! Não foi possível abrir o arquivo %s\n", COR_ERRO, filename);
        return EXIT_FAILURE;
    }

    void *dest[] = {&hash_cod, &arv_nome, &arv_latitude, &arv_longitude, &arv_uf, &arv_ddd};
    char *keys[] = {"codigo_ibge", "nome", "latitude", "longitude", "capital", "codigo_uf", "siafi_id", "ddd", "fuso_horario"};
    int r = json_parse(fmunicipios, keys, 9, aloca_municipio, insere, salva, dest);

    fclose(fmunicipios);
    if (r == EXIT_FAILURE) {
        printf("Erro ao fazer o parse do arquivo %s\n", filename);
        return EXIT_FAILURE;
    }

    while(1) {
        int ddd;
        printf("Digite o DDD do município: ");
        scanf("%d", &ddd);

        if (ddd == 0) {
            break;
        }

        tint *t = aloca_int(ddd, "");
        tlista *l = avl_range(&arv_ddd, t);
        if (l == NULL) {
            printf(NAO_ENCONTRADO);
        } else {
            printf("Municípios com DDD %d:\n", ddd);
            while (l != NULL) {
                imprime_municipio(hash_cod, ((tint *) l->reg)->codigo_ibge);
                l = l->prox;
            }
        }
        free(t);
    }

    hash_apaga(&hash_cod);

    return EXIT_SUCCESS;
}