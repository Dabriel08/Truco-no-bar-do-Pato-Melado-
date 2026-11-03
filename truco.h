/*======================= BIBLIOTECA DE CONTROLE DO TRUCO =======================*/
#ifndef truco_h
#define truco_h

#include <stdio.h>
#include <string.h>
#include "tipos.h"
#include "fila.h"

void inicializaPlacar(int placar[]) { //Função que inicializa o placar da partida
    placar[0] = 0;
    placar[1] = 0;
}

int vencedorPartida(int placar[]) { //Função que verifica se alguma dupla atingiu a pontuação necessária (12) para vencer a partida
                                    //Retorna 1 ou 2: número da dupla vencedora  ||  Retorna 0: não há vencedor ainda
    if (placar[0] >= 12) return 1;
    if (placar[1] >= 12) return 2;
    return 0;
}

// Escada do Truco sem o 9: 1 -> 3 -> 6 -> 12
int proximoValor(int valorAtual) { //Função que retorna qual será o próximo valor da mão quando uma dupla pedir truco
//Caso seja 1, retorna 3 || Caso seja 3, retorna 6 || Caso seja 6, retorna 12 || Caso seja 12, retorna 0.
    if (valorAtual == 1) return 3;
    if (valorAtual == 3) return 6;
    if (valorAtual == 6) return 12;
    return 12;
}

// Permite pedir ou aumentar ate chegar em 12
int podeTruco(tp_truco *trucoAtual, int duplaAtual) { 
    if (trucoAtual->valormao >= 12) return 0; // Trava quando chega em 12
    if (trucoAtual->duplapediu == duplaAtual) return 0; // Mesma dupla nao pede seguido
    return 1;
}

// Escolhe qual jogador da dupla adversaria vai responder ao pedido
void escolherRespondente(tp_fila *Jogadores, int duplaAlvo, char nomeRespondente[50]) {
    tp_fila copia = *Jogadores;
    tp_jogador opcoes[2];
    int cont = 0;

    while (!filaVazia(&copia)) {
        tp_jogador temp;
        removeFila(&copia, &temp);
        if (temp.dupla == duplaAlvo && cont < 2) {
            opcoes[cont++] = temp;
        }
    }

    int esc = 0;
    while (esc != 1 && esc != 2) {
        printf("Qual jogador da Dupla %d vai responder? [1] %s | [2] %s: ", 
               duplaAlvo, opcoes[0].nome, opcoes[1].nome);
        scanf("%d", &esc);
    }
    strcpy(nomeRespondente, opcoes[esc - 1].nome);
}

void pedeTruco(tp_truco *trucoAtual, int duplaPediu, int placar[], tp_fila *Jogadores) {
    tp_jogador jogadorQuePediu;
    topoFila(Jogadores, &jogadorQuePediu);

    char nomeQuemPediu[50];
    strcpy(nomeQuemPediu, jogadorQuePediu.nome);

    int duplaAtacante = duplaPediu;

    // Laco que permite rebater: 3 -> 6 -> 12
    while (1) {
        int duplaDefensora = (duplaAtacante == 1) ? 2 : 1;
        int valorProposto = proximoValor(trucoAtual->valormao);

        // Sobrescreve sempre com o ULTIMO que pediu aumento (3, 6 ou 12)
        trucoAtual->duplapediu = duplaAtacante;
        strcpy(trucoAtual->quemTrucou.nome, nomeQuemPediu);

        if (valorProposto == 3)
            printf("\n>>> %s (Dupla %d) pediu TRUCO (valendo 3 pontos)! <<<\n", nomeQuemPediu, duplaAtacante);
        else if (valorProposto == 6)
            printf("\n>>> %s (Dupla %d) pediu SEIS (valendo 6 pontos)! <<<\n", nomeQuemPediu, duplaAtacante);
        else if (valorProposto == 12)
            printf("\n>>> %s (Dupla %d) pediu DOZE (valendo 12 pontos)! <<<\n", nomeQuemPediu, duplaAtacante);

        char nomeDefensor[50];
        escolherRespondente(Jogadores, duplaDefensora, nomeDefensor);

        int resp = 0;
        while (resp < 1 || resp > 3) {
            printf("%s (Dupla %d), qual a sua decisao?\n", nomeDefensor, duplaDefensora);
            printf("[1] Aceitar (mao passa a valer %d)\n", valorProposto);
            printf("[2] Recusar / Correr (Dupla %d ganha %d ponto(s) agora)\n", duplaAtacante, trucoAtual->valormao);
            if (valorProposto < 12) {
                printf("[3] Aumentar para %d pontos!\n", proximoValor(valorProposto));
            }
            scanf("%d", &resp);

            if (resp == 3 && valorProposto >= 12) {
                printf("Opcao invalida! A mao ja chegou no limite maximo de 12 pontos.\n");
                resp = 0;
            }
        }

        if (resp == 1) {
            // Aceitou: atualiza o valor da mao e guarda quem aceitou por ultimo
            trucoAtual->valormao = valorProposto;
            strcpy(trucoAtual->quemRetrucou.nome, nomeDefensor);
            printf("%s aceitou! A mao agora vale %d pontos!\n", nomeDefensor, trucoAtual->valormao);
            break;
        } else if (resp == 2) {
            // Recusou/Correu: encerra a mao, soma os pontos anteriores e guarda quem correu
            trucoAtual->maoEncerrada = duplaAtacante;
            strcpy(trucoAtual->quemRetrucou.nome, nomeDefensor);
            placar[duplaAtacante - 1] += trucoAtual->valormao;
            break;
        } else if (resp == 3) {
            // Aumentou (3 -> 6 ou 6 -> 12):
            // O valor anterior passa a valer e o defensor vira o novo atacante que pediu o aumento!
            trucoAtual->valormao = valorProposto;
            duplaAtacante = duplaDefensora;
            strcpy(nomeQuemPediu, nomeDefensor);
        }
    }
}

#endif  