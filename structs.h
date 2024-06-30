#ifndef __STRUCTS__
#define __STRUCTS__

typedef struct {
    char codigo_ibge[10];
    char nome[100];
    double latitude;
    double longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[100];
} tmunicipio;

typedef struct {
    char codigo_ibge[10];
} tcodigo_ibge;

typedef struct {
    char codigo_ibge[10];
    char nome[100];
} tstring;

typedef struct {
    char codigo_ibge[10];
    double valor;
} tdouble;

typedef struct {
    char codigo_ibge[10];
    int valor;
} tint;

char *get_cod(void *reg);
double cmp_codigo_ibge(void *a, void *b);
double cmp_nome(void *a, void *b);
double cmp_double(void *a, void *b);
double cmp_int(void *a, void *b);
tstring *aloca_string(char *nome, char *codigo_ibge);
tdouble *aloca_double(double valor, char *codigo_ibge);
tint *aloca_int(int valor, char *codigo_ibge);
void aloca_municipio(void **municipio);

#endif