#include <stdio.h>
#include <stdlib.h>

#include "../include/avl.h"
#include "../include/hash.h"


int max(int a, int b) {
    return a > b ? a : b;
}

// Compara dois nós e retorna o menor
tnode *menor_no(tarv *parv, tnode *a, tnode *b) {
    if (a == NULL) {
        return b;
    }
    if (b == NULL) {
        return a;
    }
    if (parv->cmp(a->lista->reg, b->lista->reg) < 0) {
        return a;
    }
    return b;
}

int altura(tnode *arv) {
    if (arv == NULL)
       return -1;

    return arv->h;
}

void _rd(tnode **pnode) {
    tnode *y = *pnode;
    tnode *x = y->esq;
    tnode *A = x->esq;
    tnode *B = x->dir;
    tnode *C = y->dir;

    y->esq = B;
    x->dir = y;
    *pnode = x;

    if (B != NULL)
        B->pai = y;
    x->pai = y->pai;
    y->pai = x;

    y->h = max(altura(B), altura(C)) + 1;
    x->h = max(altura(A), altura(y)) + 1;
}

void _re(tnode **pnode) {
    tnode *x = *pnode;
    tnode *y = x->dir;
    tnode *A = x->esq;
    tnode *B = y->esq;
    tnode *C = y->dir;

    x->dir = B;
    y->esq = x;
    *pnode = y;

    if (B != NULL)
        B->pai = x;
    y->pai = x->pai;
    x->pai = y;

    x->h = max(altura(A), altura(B)) + 1;
    y->h = max(altura(x), altura(C)) + 1;
}

void _avl_rebalancear(tnode **pnode) {
    int fb;
    int fbf;
    tnode *filho;
    fb = altura((*pnode)->esq) - altura((*pnode)->dir);

    if (fb == -2) {
        filho = (*pnode)->dir;
        fbf = altura(filho->esq) - altura(filho->dir);
        if (fbf <= 0) { /* Caso 1  --> ->*/
            _re(pnode);
        } else { /* Caso 2  --> <-*/
            _rd(&(*pnode)->dir);
            _re(pnode);
        }
    } else if (fb == 2) {
        filho = (*pnode)->esq;
        fbf = altura(filho->esq) - altura(filho->dir);
        if (fbf >= 0) { /* Caso 3  <-- <-*/
            _rd(pnode);
        } else { /* Caso 4  <-- ->*/
            _re(&(*pnode)->esq);
            _rd(pnode);
        }
    }
}

tnode **percorre_esq(tnode **ppnode) {
    tnode *aux = *ppnode;
    if (aux->esq == NULL) {
        return ppnode;
    } else {
        while (aux->esq->esq != NULL)
            aux = aux->esq;
        return &(aux->esq);
    }
}

tnode **sucessor(tnode **ppnode) {
    if (*ppnode == NULL) {
        return NULL;
    }

    if ((*ppnode)->dir != NULL) {
        return percorre_esq(&(*ppnode)->dir);
    }

    tnode *pai = (*ppnode)->pai;
    while (pai != NULL && *ppnode == pai->dir) {
        *ppnode = pai;
        pai = pai->pai;
    }
    return &(*ppnode)->pai;
}

void avl_constroi(tarv *parv, double (*cmp)(void *, void *)) {
    parv->raiz = NULL;
    parv->cmp = cmp;
}

void _lista_insere(tlista **plista, void *reg) {
    if (*plista == NULL) {
        *plista = (tlista *) malloc(sizeof(tlista));
        (*plista)->reg = reg;
        (*plista)->prox = NULL;
    } else {
        _lista_insere(&(*plista)->prox, reg);
    }
}

void _avl_insere_node(tarv *parv, tnode **ppnode, tnode *ppai, void *reg) {
    if (*ppnode == NULL) {
        *ppnode = (tnode *) malloc(sizeof(tnode));
        (*ppnode)->lista = NULL;
        _lista_insere(&(*ppnode)->lista, reg);
        (*ppnode)->pai = ppai;
        (*ppnode)->esq = NULL;
        (*ppnode)->dir = NULL;
        (*ppnode)->h = 0;
    } else if (parv->cmp((*ppnode)->lista->reg, reg) > 0) {
       _avl_insere_node(parv, &(*ppnode)->esq, *ppnode, reg);
    } else if (parv->cmp((*ppnode)->lista->reg, reg) < 0) {
        _avl_insere_node(parv, &(*ppnode)->dir, *ppnode, reg);
    } else {
        _lista_insere(&(*ppnode)->lista, reg);
    }
    (*ppnode)->h = max(altura((*ppnode)->esq), altura((*ppnode)->dir)) + 1;

    _avl_rebalancear(ppnode);
}

void avl_insere(tarv *parv, void *reg) {
    _avl_insere_node(parv, &parv->raiz, NULL, reg);
}

void busca_menor_no(tarv *parv, tnode *pnode, tnode **menor, void *reg_min) {
    if (reg_min == NULL) {
        *menor = *(percorre_esq(&pnode));
    } else if (pnode != NULL) {
        if (parv->cmp(pnode->lista->reg, reg_min) < 0) {
            busca_menor_no(parv, pnode->dir, menor, reg_min);
        } else if (parv->cmp(pnode->lista->reg, reg_min) > 0){
            *menor = menor_no(parv, *menor, pnode);
            busca_menor_no(parv, pnode->esq, menor, reg_min);
        } else {
            *menor = pnode;
        }
    }
}

/*
void _avl_remove_node(tarv *parv, tnode **ppnode, void *reg) {
    int cmp;
    tnode *aux;
    tnode **sucessor;
    if (*ppnode != NULL) {
        cmp = parv->cmp((*ppnode)->lista->reg, reg);
        if (cmp > 0) { // ir esquerda
            _avl_remove_node(parv, &((*ppnode)->esq), reg);
        } else if (cmp < 0) { //ir direita 
            _avl_remove_node(parv, &((*ppnode)->dir), reg);
        } else {                                                // ACHOU
            if ((*ppnode)->esq == NULL && (*ppnode)->dir == NULL) { // no folha
                free(*ppnode);
                *ppnode = NULL;
            } else if ((*ppnode)->esq == NULL || (*ppnode)->dir == NULL) { // tem um filho
                aux = *ppnode;
                if ((*ppnode)->esq == NULL) {
                    *ppnode = (*ppnode)->dir;
                } else {
                    *ppnode = (*ppnode)->esq;
                }
                free(aux);
            } else { // tem dois filhos
                sucessor = percorre_esq(&(*ppnode)->dir);
                (*ppnode)->lista->reg = (*sucessor)->lista->reg;
                _avl_remove_node(parv, &(*ppnode)->dir, (*sucessor)->lista->reg);
            }
        }
        if (*ppnode != NULL) {
            (*ppnode)->h = max(altura((*ppnode)->esq), altura((*ppnode)->dir)) + 1;
            _avl_rebalancear(ppnode);
        }
    }
}

void avl_remove(tarv *parv, void *reg) {
    _avl_remove_node(parv, &parv->raiz, reg);
}*/

tlista *_avl_busca_node(tarv *parv, tnode *pnode, void *reg) {
    if (pnode == NULL) {
        return NULL;
    } else {
        if (parv->cmp(pnode->lista->reg, reg) == 0) {
            return pnode->lista;
        } else if (parv->cmp(pnode->lista->reg, reg) > 0) {
            return _avl_busca_node(parv, pnode->esq, reg);
        } else {
            return _avl_busca_node(parv, pnode->dir, reg);
        }
    }
}

tlista *avl_busca(tarv *parv, void *reg) {
    return _avl_busca_node(parv, parv->raiz, reg);
}

tlista * _avl_range_node(tarv *parv, tnode *pnode, void *min, void *max) {
    tlista *listaInicio = NULL;
    tlista *listaFim = NULL;
    tnode *menor = NULL;
    busca_menor_no(parv, pnode, &menor, min);
    if (menor != NULL) {
        while (menor != NULL && (max == NULL || parv->cmp(menor->lista->reg, max) <= 0)) {
            tlista *aux = menor->lista;
            while(aux != NULL) {
                if (listaInicio == NULL) {
                    listaInicio = (tlista *) malloc(sizeof(tlista));
                    listaInicio->reg = aux->reg;
                    listaFim = listaInicio;
                } else {
                    listaFim->prox = (tlista *) malloc(sizeof(tlista));
                    listaFim = listaFim->prox;
                    listaFim->reg = aux->reg;
                    listaFim->prox = NULL;
                }
                aux = aux->prox;
            }
            menor = *(sucessor(&menor));
        }
    }

    return listaInicio;
}

tlista *avl_range(tarv *parv, void *min, void *max) {
    return _avl_range_node(parv, parv->raiz, min, max);
}


tlista *lista_interseccao(double (*cmp)(void *, void *), tlista **listas, int n_listas) {
    tlista *interseccao = NULL;
    tlista *aux = listas[0];
    while (aux != NULL) {
        int i;
        for (i = 1; i < n_listas; i++) {
            tlista *aux2 = listas[i];
            while (aux2 != NULL && cmp(aux->reg, aux2->reg) != 0) {
                aux2 = aux2->prox;
            }
            if (aux2 == NULL) {
                break;
            }
        }
        if (i == n_listas) {
            if (interseccao == NULL) {
                interseccao = (tlista *) malloc(sizeof(tlista));
                interseccao->reg = aux->reg;
                interseccao->prox = NULL;
            } else {
                tlista *aux3 = interseccao;
                while (aux3->prox != NULL) {
                    aux3 = aux3->prox;
                }
                aux3->prox = (tlista *) malloc(sizeof(tlista));
                aux3->prox->reg = aux->reg;
                aux3->prox->prox = NULL;
            }
        }
        aux = aux->prox;
    }
    return interseccao;
}

void lista_destroi(tlista *plista) {
    if (plista != NULL) {
        lista_destroi(plista->prox);
        free(plista);
    }
}

void _avl_destroi_node(tnode *pnode) {
     if (pnode != NULL) {
        _avl_destroi_node(pnode->esq);
        _avl_destroi_node(pnode->dir);
        tlista *aux = pnode->lista;
        while (aux != NULL) {
            tlista *aux2 = aux;
            aux = aux->prox;
            free(aux2);
        }
        free(pnode);
    }
}

void avl_destroi(tarv *parv) {
    _avl_destroi_node(parv->raiz);
    parv->raiz = NULL;
}
