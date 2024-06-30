#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/avl.h"

typedef struct {
    int id;
    char nome[100];
} aluno;

typedef struct {
    int a;
    char bnome[100];
} prof;

double cmp_aluno(void *a, void *b) {
    return ((aluno *) a)->id - ((aluno *) b)->id;
}

double cmp_prof(void *a, void *b) {
    return ((prof *) a)->a - ((prof *) b)->a;
}

aluno * aloca_aluno(int id, char *nome) {
    aluno *a = (aluno *) malloc(sizeof(aluno));
    assert(a != NULL);
    a->id = id;
    strcpy(a->nome, nome);
    return a;
}

prof * aloca_prof(int id, char *nome) {
    prof *p = (prof *) malloc(sizeof(prof));
    assert(p != NULL);
    p->a = id;
    strcpy(p->bnome, nome);
    return p;
}

void test_avl_insere() {
    tarv arv;
    avl_constroi(&arv, cmp_aluno);

    avl_insere(&arv, aloca_aluno(1, "João"));
    avl_insere(&arv, aloca_aluno(2, "Maria"));
    avl_insere(&arv, aloca_aluno(3, "José"));
    avl_insere(&arv, aloca_aluno(3, "Ana"));

    tlista *l = avl_busca(&arv, aloca_aluno(3, ""));
    assert(l != NULL);
    assert(((aluno *) l->reg)->id == 3);
    assert(strcmp(((aluno *) l->reg)->nome, "José") == 0);
    assert(((aluno *) l->prox->reg)->id == 3);
    assert(strcmp(((aluno *) l->prox->reg)->nome, "Ana") == 0);

    avl_destroi(&arv);
}

void test_avl_menor() {
    tarv arv;
    avl_constroi(&arv, cmp_aluno);

    avl_insere(&arv, aloca_aluno(1, "João"));
    avl_insere(&arv, aloca_aluno(2, "Maria"));
    avl_insere(&arv, aloca_aluno(3, "José"));
    avl_insere(&arv, aloca_aluno(4, "Ana"));

    tnode *menor = NULL;
    busca_menor_no(&arv, arv.raiz, &menor, aloca_aluno(3, ""));
    assert(menor != NULL);
    assert(menor->lista != NULL);
    assert(((aluno *) menor->lista->reg)->id == 3);
    assert(strcmp(((aluno *) menor->lista->reg)->nome, "José") == 0);
    //printf("%s\n", ((aluno *) menor->lista->reg)->nome);

    avl_destroi(&arv);

}

void test_avl_range() {
    tarv arv;
    avl_constroi(&arv, cmp_aluno);

    avl_insere(&arv, aloca_aluno(1, "João"));
    avl_insere(&arv, aloca_aluno(2, "Maria"));
    avl_insere(&arv, aloca_aluno(3, "José"));
    avl_insere(&arv, aloca_aluno(4, "Ana"));

    // >= 1
    tlista *l = avl_range(&arv, aloca_aluno(1, ""), NULL);
    assert(l != NULL);
    assert(((aluno *) l->reg)->id == 1);
    l = l->prox;
    assert(((aluno *) l->reg)->id == 2);
    l = l->prox;
    assert(((aluno *) l->reg)->id == 3);
    l = l->prox;
    assert(((aluno *) l->reg)->id == 4);
    l = l->prox;
    assert(l == NULL);

    // <= 3
    l = avl_range(&arv, NULL, aloca_aluno(3, ""));
    assert(l != NULL);
    assert(((aluno *) l->reg)->id == 1);
    l = l->prox;
    assert(((aluno *) l->reg)->id == 2);
    l = l->prox;
    assert(((aluno *) l->reg)->id == 3);
    l = l->prox;
    assert(l == NULL);

    // >= 2 e <= 3
    l = avl_range(&arv, aloca_aluno(2, ""), aloca_aluno(3, ""));
    assert(l != NULL);
    assert(((aluno *) l->reg)->id == 2);
    l = l->prox;
    assert(((aluno *) l->reg)->id == 3);
    l = l->prox;
    assert(l == NULL);

    // >= 5
    l = avl_range(&arv, aloca_aluno(5, ""), NULL);
    assert(l == NULL);

    // <= 0
    l = avl_range(&arv, NULL, aloca_aluno(0, ""));
    assert(l == NULL);

    avl_destroi(&arv);
}

void test_avl_interseccao() {
    tarv arvAluno, arvProf;
    avl_constroi(&arvAluno, cmp_aluno);
    avl_constroi(&arvProf, cmp_aluno);

    avl_insere(&arvAluno, aloca_aluno(1, "João"));
    avl_insere(&arvAluno, aloca_aluno(2, "Maria"));
    avl_insere(&arvAluno, aloca_aluno(3, "José"));
    avl_insere(&arvAluno, aloca_aluno(4, "Ana"));

    avl_insere(&arvProf, aloca_prof(1, "Prof. João"));
    avl_insere(&arvProf, aloca_prof(2, "Prof. Maria"));
    avl_insere(&arvProf, aloca_prof(3, "Prof. José"));
    avl_insere(&arvProf, aloca_prof(4, "Prof. Ana"));

    // 2, 3
    tlista *l1 = avl_range(&arvAluno, aloca_aluno(1, ""), aloca_aluno(3, ""));
    tlista *l2 = avl_range(&arvProf, aloca_prof(2, ""), aloca_prof(4, ""));

    tlista *listas[] = {l1, l2};
    tlista *l = lista_interseccao(&arvAluno, listas, 2);
    
    assert(l != NULL);
    assert(((aluno *) l->reg)->id == 2);
    l = l->prox;
    assert(((aluno *) l->reg)->id == 3);
    l = l->prox;
    assert(l == NULL);

    // vazio
    l1 = avl_range(&arvAluno, aloca_aluno(1, ""), aloca_aluno(2, ""));
    l2 = avl_range(&arvAluno, aloca_aluno(3, ""), aloca_aluno(4, ""));
    listas[0] = l1;
    listas[1] = l2;
    l = lista_interseccao(&arvAluno, listas, 2);

    assert(l == NULL); 

    l1 = avl_range(&arvAluno, aloca_aluno(1, ""), aloca_aluno(2, ""));
    listas[0] = l1;
    l = lista_interseccao(&arvAluno, listas, 1);

    assert(l != NULL);
    assert(((aluno *) l->reg)->id == 1);
    l = l->prox;
    assert(((aluno *) l->reg)->id == 2);
    l = l->prox;
    assert(l == NULL);

}

int main() {
    test_avl_insere();
    test_avl_menor();
    test_avl_range();
    test_avl_interseccao();

    return EXIT_SUCCESS;
}