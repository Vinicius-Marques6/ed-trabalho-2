#include <stdio.h>
#include <stdlib.h>

#include "../include/avl.h"
#include "../include/hash.h"


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

tnode **predecessor(tnode **arv) {
    if (*arv == NULL) {
        return NULL;
    }

    if ((*arv)->esq != NULL) {
        return percorre_esq(&(*arv)->esq);
    }

    tnode *pai = (*arv)->pai;
    while (pai != NULL && *arv == pai->esq) {
        *arv = pai;
        pai = pai->pai;
    }
    return &(*arv)->pai;
}

tnode **sucessor(tnode **arv) {
    if (*arv == NULL) {
        return NULL;
    }

    if ((*arv)->dir != NULL) {
        return percorre_esq(&(*arv)->dir);
    }

    tnode *pai = (*arv)->pai;
    while (pai != NULL && *arv == pai->dir) {
        *arv = pai;
        pai = pai->pai;
    }
    return &(*arv)->pai;
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

void _lista_copia(tlista **pplista, tlista *lista) {
    while (lista != NULL) {
        _lista_insere(pplista, lista->reg);
        lista = lista->prox;
    }
}

void _lista_apaga(tlista **plista) {
    if (*plista != NULL) {
        _lista_apaga(&(*plista)->prox);
        free(*plista);
        *plista = NULL;
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
}

tnode **_avl_busca_node(tarv *parv, tnode **pnode, void *reg) {
    if (*pnode == NULL) {
        return NULL;
    } else {
        if (parv->cmp((*pnode)->lista->reg, reg) == 0) {
            return pnode;
        } else if (parv->cmp((*pnode)->lista->reg, reg) > 0) {
            return _avl_busca_node(parv, &(*pnode)->esq, reg);
        } else {
            return _avl_busca_node(parv, &(*pnode)->dir, reg);
        }
    }
}

void _avl_menor_que(tarv *parv, tlista **lista, tnode **pnode) {
    tnode **pred = predecessor(pnode);
    while (*pred != NULL) {
        _lista_copia(lista, (*pred)->lista);
        pred = predecessor(pred);
        printf("pred: %p\n", (void *) pred);
    }
}

tlista * _avl_range_node(tarv *parv, tnode **pnode, void *reg_min, void *reg_max) {
    tlista *l = NULL;
    tnode **pred = _avl_busca_node(parv, pnode, reg_max);
    printf("pred: %p\n", (void *) pred);
    _avl_menor_que(parv, &l, pred);
    printf("l: %p\n", (void *) l);
    return l;
}

tlista *avl_range(tarv *parv, void *reg_min, void *reg_max) {
    return _avl_range_node(parv, &parv->raiz, reg_min, reg_max);
}

typedef struct {
    void *reg;
    int freq;
} tfreq;

tlista *lista_interseccao(tlista **listas, int n_listas, int max_size, char *(*get_key)(void *)) {
    thash hash_freq;
    hash_constroi(&hash_freq, max_size, get_key);
    for(int i = 0; i < n_listas; i++) {
        tlista *aux = listas[i];
        while (aux != NULL) {
            tfreq *freq = (tfreq *) hash_busca(hash_freq, get_key(aux->reg));
            if (freq == NULL) {
                freq = (tfreq *) malloc(sizeof(tfreq));
                freq->reg = aux->reg;
                freq->freq = 1;
                hash_insere(&hash_freq, freq);
            } else {
                freq->freq++;
            }
            aux = aux->prox;
        }
    }

    tlista *result = NULL;
    for (int i = 0; i < hash_freq.size; i++) {
        if(hash_freq.table[i] != 0) {
            tfreq *freq = (tfreq *) hash_freq.table[i];
            if (freq->freq == n_listas) {
                tlista *aux = (tlista *) malloc(sizeof(tlista));
                aux->reg = freq->reg;
                aux->prox = result;
                result = aux;
            }
        }
    }

    hash_apaga(&hash_freq);
    return result;
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
