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

void avl_constroi(tarv *parv, double (*cmp)(void *, void *));
void avl_insere(tarv *parv, void *reg);
// void avl_remove(tarv *parv, void *reg);
void avl_destroi(tarv *parv);
tlista *avl_range(tarv *parv, void *reg);
tlista *lista_interseccao(tlista **listas, int n_listas);

#endif