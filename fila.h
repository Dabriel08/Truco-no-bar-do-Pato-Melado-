// BIBLIOTECA DE FILA
#ifndef fila_h
#define fila_h

#include <stdio.h>
#include "tipos.h"

#define MAX_FILA 100

typedef tp_jogador tp_item_fila;

typedef struct {
    tp_item_fila item[MAX_FILA];
    int inicio, fim;
} tp_fila;

void inicializaFila(tp_fila* f) {
    f->inicio = f->fim = MAX_FILA - 1;
}

int filaVazia(tp_fila* f) {
    if (f->inicio == f->fim) return 1;
    return 0;
}

int proximaPosicao(int posicao) {
    if (posicao == MAX_FILA - 1) return 0;
    return ++posicao;
}

int filaCheia(tp_fila* f) {
    if (proximaPosicao(f->fim) == f->inicio) return 1;
    return 0;
}

int insereFila(tp_fila* f, tp_item_fila e) {
    if (filaCheia(f)) return 0;
    f->fim = proximaPosicao(f->fim);
    f->item[f->fim] = e;
    return 1;
}

int removeFila(tp_fila* f, tp_item_fila* e) {
    if (filaVazia(f)) return 0;
    f->inicio = proximaPosicao(f->inicio);
    *e = f->item[f->inicio];
    return 1;
}

// Função adicionada que faltava no projeto!
int topoFila(tp_fila* f, tp_item_fila* e) {
    if (filaVazia(f)) return 0;
    int pos = proximaPosicao(f->inicio);
    *e = f->item[pos];
    return 1;
}

void imprimeFila(tp_fila f) {
    tp_item_fila e;
    while (!filaVazia(&f)) {
        removeFila(&f, &e);
        printf("\nJogador: %s (Dupla %d)", e.nome, e.dupla);
    }
}

int tamanhoFila(tp_fila* f) {
    if (filaVazia(f)) return 0;
    if (f->inicio < f->fim) return (f->fim - f->inicio);
    return (MAX_FILA - 1 - f->inicio + f->fim + 1);
}

void inverteFila(tp_fila* f) {
    int tamanho = tamanhoFila(f);
    tp_item_fila auxiliar[MAX_FILA];
    tp_item_fila removido;
    int i = 0; // Corrigido: declarado fora do while

    while (!filaVazia(f)) {
        removeFila(f, &removido);
        auxiliar[i] = removido;
        i++;
    }

    for (int j = tamanho; j > 0; j--) {
        insereFila(f, auxiliar[j - 1]);
    }
}

void prox_jogador(tp_fila* Jogadores) { //Função que movimenta a fila de jogadores
    tp_jogador proximo;
    removeFila(Jogadores, &proximo); //retira o jogador da primera posição
    insereFila(Jogadores, proximo); //reinsere o jogador no fim da fila
}

#endif