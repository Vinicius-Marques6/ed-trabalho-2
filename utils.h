#ifndef __UTILS__
#define __UTILS__

#include "./structs.h"
#include "./lib/include/avl.h"

#define BOLD "\x1b[1m"
#define NO_BOLD "\x1b[22m"
#define COR_BARRA "\x1b[33m"
#define COR_TITULO "\x1b[34m"
#define COR_AVISO "\x1b[33m"
#define COR_ERRO "\x1b[31m"
#define COR_RESET "\x1b[0m"
#define COR_VERDE "\x1b[32m"

#define Coluna(...) printf("%s|%s", COR_BARRA, COR_RESET);\
printf(__VA_ARGS__);

#define Error(x) printf("%s%s%s", COR_ERRO, x, COR_RESET);
#define Success(x) printf("%s%s%s", COR_VERDE, x, COR_RESET);
#define Warning(x) printf("%s%s%s", COR_AVISO, x, COR_RESET);

#define NAO_ENCONTRADO "Município não encontrado\n"

void int_ou_null(void *reg, char min[10]);
void double_ou_null(void *reg, char min[50]);
void string_ou_null(void *reg, char min[200]);
void le_int(tint **i1, tint **i2, tintervalo *query, char nome[100], tarv *avl);
void le_double(tdouble **d1, tdouble **d2, tintervalo *query, char nome[100], tarv *avl);
void le_string(tstring **s1, tstring **s2, tintervalo *query, char nome[100], tarv *avl);
#endif
