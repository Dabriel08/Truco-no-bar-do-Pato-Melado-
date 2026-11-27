#ifndef pilha_h
#define pilha_h

#include <stdio.h>
#include "tipos.h"

#define MAX_PILHA 100

typedef carta tp_item_pilha;

typedef struct {
    int topo;
    tp_item_pilha item[MAX_PILHA];
} tp_pilha;

void inicializapilha(tp_pilha* p) {
    p->topo = -1;
}

int pilhavazia(tp_pilha* p) {
    if (p->topo == -1) return 1;
    return 0;
}

int pilhacheia(tp_pilha* p) {
    if (p->topo == MAX_PILHA - 1) return 1;
    return 0;
}

int push(tp_pilha* p, tp_item_pilha elemento) {
    if (pilhacheia(p) == 1) return 0;
    p->topo++;
    p->item[p->topo] = elemento;
    return 1;
}

int pop(tp_pilha* p, tp_item_pilha* elemento) {
    if (pilhavazia(p)) return 0;
    *elemento = p->item[p->topo];
    p->topo--;
    return 1;
}

int top(tp_pilha* p, tp_item_pilha* elemento) {
    if (pilhavazia(p)) return 0;
    *elemento = p->item[p->topo];
    return 1;
}

void imprimepilha(tp_pilha* p) {
    if (!pilhavazia(p)) {
        for (int i = 0; i <= p->topo; i++) {
            printf("%d%c ", p->item[i].numero, p->item[i].naipe);
        }
        printf("\n");
    }
}

int alturapilha(tp_pilha p) {
    if (pilhavazia(&p)) return 0; // Corrigido: sem o '!'
    return p.topo + 1;
}

#endif
