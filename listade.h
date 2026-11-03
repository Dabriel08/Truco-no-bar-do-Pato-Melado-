/*=======================BIBLIOTECA DE LISTA DUPLAMENTE ENCADEADA =======================*/
#ifndef listad_h
#define listad_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tipos.h"

// Usamos diretamente o tp_salvar do tipos.h como o item da nossa lista
typedef tp_salvar tp_item;

typedef struct tp_no_aux{
     struct tp_no_aux *ant;
     tp_item info;
     struct tp_no_aux *prox;
} tp_no;

typedef struct{
     tp_no *inicio;
     tp_no *fim;
} tp_listad;
 
tp_listad *inicializa_listad(){  //Inicializo uma lista sem nós anterior e próximo, apenas alocando o espaço do 1° nó
     tp_listad *lista = (tp_listad*) malloc(sizeof(tp_listad));
     lista->inicio = NULL;
     lista->fim = NULL;
     return lista;
}
 
int listad_vazia (tp_listad *lista){ //Verifica se a lista está vazia (com o ponteiro do início apontando para o nada)
    if (lista->inicio == NULL) return 1;
    return 0;
}
 
tp_no *aloca(){ //Função auxiliar de alocação de memória
    tp_no* pt = (tp_no*) malloc(sizeof(tp_no));
    return pt;
}

int insere_listad_no_fim (tp_listad *lista, tp_item elemento){ //Adiciono elementos no fim da lista
    tp_no *novo_no = aloca();
    if (!novo_no) return 0;
    
    novo_no->info = elemento;
    novo_no->prox = NULL;
    
    if(listad_vazia(lista)){
        novo_no->ant = NULL;
        lista->inicio = lista->fim = novo_no;
    } else {
        novo_no->ant = lista->fim;
        lista->fim->prox = novo_no;
        lista->fim = novo_no;
    }
    return 1;
}

int remove_listad (tp_listad *lista, tp_item elemento){ //Remove um elemento da lista
    tp_no *atual = lista->inicio;
    while ((atual != NULL) && (atual->info.mao != elemento.mao)){
        atual = atual->prox;
    }
    
    if (atual == NULL) return 0;
    
    if (lista->inicio == lista->fim){
        lista->inicio = lista->fim = NULL;
    } else {
        if (lista->inicio == atual){
            lista->inicio = atual->prox;
            atual->prox->ant = NULL;
        } else {
            if (lista->fim == atual){
                lista->fim = atual->ant;
                atual->ant->prox = NULL;
            } else {
                atual->prox->ant = atual->ant;
                atual->ant->prox = atual->prox;
            }
        }
    }
    free(atual);
    return 1;
}

#endif