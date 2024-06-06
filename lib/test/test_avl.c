#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/avl.h"

typedef struct {
    int id;
    char nome[100];
} aluno;

double cmp(void *a, void *b) {
    return ((aluno *) a)->id - ((aluno *) b)->id;
}

aluno * aloca_aluno(int id, char *nome) {
    aluno *a = (aluno *) malloc(sizeof(aluno));
    assert(a != NULL);
    a->id = id;
    strcpy(a->nome, nome);
    return a;
}

int main() {
    tarv arv;
    avl_constroi(&arv, cmp);

    avl_insere(&arv, aloca_aluno(1, "João"));
    avl_insere(&arv, aloca_aluno(2, "Maria"));
    avl_insere(&arv, aloca_aluno(3, "José"));

    aluno *a = (aluno *) avl_busca(&arv, aloca_aluno(2, ""));
    assert(a != NULL);
    assert(a->id == 2);
    assert(strcmp(a->nome, "Maria") == 0);

    return EXIT_SUCCESS;
}