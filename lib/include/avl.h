#ifndef __AVL__
#define __AVL__

typedef struct _node {
    void *reg;
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
void avl_remove(tarv *parv, void *reg);
void avl_destroi(tarv *parv);
void *avl_busca(tarv *parv, void *reg);

#endif