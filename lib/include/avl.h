#ifndef __AVL__
#define __AVL__

typedef struct _lista {
    void *reg;
    struct _lista *prox;
} tlista;

typedef struct _node {
    tlista *lista;
    struct _node *pai;
    struct _node *esq;
    struct _node *dir;
    int h;
} tnode;

typedef struct {
    tnode *raiz;
    double (*cmp)(void * , void *);
} tarv;

typedef struct {
    tlista *lista;
    void *reg_min;
    void *reg_max;
} tintervalo;

void avl_constroi(tarv *parv, double (*cmp)(void *, void *));
void avl_insere(tarv *parv, void *reg);
// void avl_remove(tarv *parv, void *reg);
void avl_destroi(tarv *parv);
tlista *avl_busca(tarv *parv, void *reg);
void busca_menor_no(tarv *parv, tnode *pnode, tnode **menor, void *reg_min);
tlista *avl_range(tarv *parv, void *min, void *max);
tlista *lista_interseccao(double (*cmp)(void *, void *), tlista **listas, int n_listas);
void lista_destroi(tlista *plista);

#endif