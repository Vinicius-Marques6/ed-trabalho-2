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
    avl_insere(&arv, aloca_aluno(3, "Ana"));

    tlista *l = avl_range(&arv, aloca_aluno(3, ""));
    assert(l != NULL);
    assert(((aluno *) l->reg)->id == 3);
    assert(strcmp(((aluno *) l->reg)->nome, "José") == 0);
    assert(((aluno *) l->prox->reg)->id == 3);
    assert(strcmp(((aluno *) l->prox->reg)->nome, "Ana") == 0);

    return EXIT_SUCCESS;
}