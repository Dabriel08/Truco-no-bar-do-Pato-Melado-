/*=======================BIBLIOTECA DE PILHA =======================*/
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

void inicializapilha(tp_pilha *p) { //Inicializa a pilha com o índice do topo sendo -1
    p->topo = -1;
}

int pilhavazia(tp_pilha *p) { //Função que verifica se a pilha tem algum elemento por "booleanos" (0 ou 1)
    if (p->topo == -1) return 1;
    return 0;
}

int pilhacheia(tp_pilha *p) { //Função que verifica se a pilha está totalmente preenchida por "booleanos" (0 ou 1)
    if (p->topo == MAX_PILHA - 1) return 1;
    return 0;
}

int push(tp_pilha *p, tp_item_pilha elemento) { //Função que adiciona elementos à pilha, se for possível adicionar
    if (pilhacheia(p) == 1) return 0;
    p->topo++;
    p->item[p->topo] = elemento;
    return 1;
}

int pop(tp_pilha *p, tp_item_pilha *elemento) { //Função que remove elementos da pilha, se for possível remover
    if (pilhavazia(p)) return 0;
    *elemento = p->item[p->topo];
    p->topo--;
    return 1;
}

int top(tp_pilha *p, tp_item_pilha *elemento) { //Função que mostra o elemento no topo da pilha pelo seu índice
    if (pilhavazia(p)) return 0;
    *elemento = p->item[p->topo];
    return 1;
}

void imprimepilha(tp_pilha *p) { //Função que imprime todos os elementos da pilha
    if (!pilhavazia(p)) {
        for (int i = 0; i <= p->topo; i++) {
            printf("%d%c ", p->item[i].numero, p->item[i].naipe);
        }
        printf("\n");
    }
}

int alturapilha(tp_pilha p) { //Função que retorna o tamanho da pilha
    if (pilhavazia(&p)) return 0; // Corrigido: sem o '!'
    return p.topo + 1;
}

#endif
