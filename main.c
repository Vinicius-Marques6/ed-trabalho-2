#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "./lib/include/json.h"

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

typedef struct {
    char codigo_ibge[8];
    char nome[100];
    double latitude;
    double longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[100];
} tmunicipio;

int isEqual(void *reg, const char *key) {
    return strcmp(get_cod(reg), key) == 0;
}

void aloca(void **municipio) {
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
   
}

int main(int argc, char *argv[]) {
    char *filename = "./municipios.json";
    if (argc != 1) {
        filename = argv[1];
    }

    FILE *fmunicipios;

    fmunicipios = fopen(filename, "r");

    if (fmunicipios == NULL) {
        printf("%sErro! Não foi possível abrir o arquivo %s\n", COR_ERRO, filename);
        return EXIT_FAILURE;
    }

    char *dest[5]; /* TODO */
    char *keys[] = {"codigo_ibge", "nome", "latitude", "longitude", "capital", "codigo_uf", "siafi_id", "ddd", "fuso_horario"};
    int r = json_parse(fmunicipios, keys, 9, aloca, insere, salva, dest);

    fclose(fmunicipios);
    if (r == EXIT_FAILURE) {
        printf("Erro ao fazer o parse do arquivo %s\n", filename);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}