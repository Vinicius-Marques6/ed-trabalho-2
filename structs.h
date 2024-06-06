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
    char nome[100];
    char codigo_ibge[10];
} tstring;

typedef struct {
    double valor;
    char codigo_ibge[10];
} tdouble;

typedef struct {
    int valor;
    char codigo_ibge[10];
} tint;

#endif