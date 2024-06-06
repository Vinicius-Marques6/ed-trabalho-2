#include <stdio.h>
#include <stdlib.h>

#include "../include/avl.h"


int max(int a, int b) {
    return a > b ? a : b;
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

tnode **percorre_esq(tnode **arv) {
    tnode *aux = *arv;
    if (aux->esq == NULL) {
        return arv;
    } else {
        while (aux->esq->esq != NULL)
            aux = aux->esq;
        return &(aux->esq);
    }
}

void avl_constroi(tarv *parv, double (*cmp)(void *, void *)) {
    parv->raiz = NULL;
    parv->cmp = cmp;
}

void _avl_insere_node(tarv *parv, tnode **ppnode, void *reg) {
    if (*ppnode == NULL) {
        *ppnode = (tnode *) malloc(sizeof(tnode));
        (*ppnode)->reg = reg;
        (*ppnode)->esq = NULL;
        (*ppnode)->dir = NULL;
        (*ppnode)->h = 0;
    } else if (parv->cmp((*ppnode)->reg, reg) > 0) {
       _avl_insere_node(parv, &(*ppnode)->esq, reg);
    } else {
        _avl_insere_node(parv, &(*ppnode)->dir, reg);
    }
    (*ppnode)->h = max(altura((*ppnode)->esq), altura((*ppnode)->dir)) + 1;

    _avl_rebalancear(ppnode);
}

void avl_insere(tarv *parv, void *reg) {
    _avl_insere_node(parv, &parv->raiz, reg);
}

void _avl_remove_node(tarv *parv, tnode **ppnode, void *reg) {
    int cmp;
    tnode *aux;
    tnode **sucessor;
    if (*ppnode != NULL) {
        cmp = parv->cmp((*ppnode)->reg, reg);
        if (cmp > 0) { /* ir esquerda*/
            _avl_remove_node(parv, &((*ppnode)->esq), reg);
        } else if (cmp < 0) { /*ir direita*/
            _avl_remove_node(parv, &((*ppnode)->dir), reg);
        } else {                                                /* ACHOU  */
            if ((*ppnode)->esq == NULL && (*ppnode)->dir == NULL) { /* no folha */
                free(*ppnode);
                *ppnode = NULL;
            } else if ((*ppnode)->esq == NULL || (*ppnode)->dir == NULL) { /* tem um filho*/
                aux = *ppnode;
                if ((*ppnode)->esq == NULL) {
                    *ppnode = (*ppnode)->dir;
                } else {
                    *ppnode = (*ppnode)->esq;
                }
                free(aux);
            } else { /* tem dois filhos */
                sucessor = percorre_esq(&(*ppnode)->dir);
                (*ppnode)->reg = (*sucessor)->reg;
                _avl_remove_node(parv, &(*ppnode)->dir, (*sucessor)->reg);
            }
        }
        if (*ppnode != NULL) {
            (*ppnode)->h = max(altura((*ppnode)->esq), altura((*ppnode)->dir)) + 1;
            _avl_rebalancear(ppnode);
        }
    }
}

void * _avl_busca_node(tarv *parv, tnode *pnode, void *reg) {
    if (pnode == NULL) {
        return NULL;
    } else {
        int cmp = parv->cmp(pnode->reg, reg);
        if (cmp == 0) {
            return pnode->reg;
        } else if (cmp > 0) {
            return _avl_busca_node(parv, pnode->esq, reg);
        } else {
            return _avl_busca_node(parv, pnode->dir, reg);
        }
    }
}

void * avl_busca(tarv *parv, void *reg) {
    return _avl_busca_node(parv, parv->raiz, reg);
}

void avl_remove(tarv *parv, void *reg) {
    _avl_remove_node(parv, &parv->raiz, reg);
}

void _avl_destroi_node(tnode *pnode) {
     if (pnode != NULL) {
        _avl_destroi_node(pnode->esq);
        _avl_destroi_node(pnode->dir);
        free(pnode);
    }
}

void avl_destroi(tarv *parv) {
    _avl_destroi_node(parv->raiz);
    parv->raiz = NULL;
}
