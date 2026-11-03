/*======================= BIBLIOTECA DE CADASTRO DOS JOGADORES =======================*/
#ifndef jogador_h
#define jogador_h

#include <stdio.h>
#include "tipos.h"
#include "fila.h"
#include "baralho.h"
#include "truco.h"

void organizaJogadores(tp_fila *Jogadores) { //Criando e cadastrando os 4 jogadores na função void (cujo parâmetro é vetor - passagem por referência), já separando-os em duplas:
    //Lendo e armazenando dados numa fila de jogadores:
    /*OBJETIVO: a fila possibilita que os jogadores sejam organizados em um array que segue uma ordem de chegada,
    fazendo com que a roda de jogo seja respeitada do início ao fim */

    for (int i = 0; i < nJogadores; i++) { 
        tp_jogador jog_aux;
        printf("Insira o nome do jogador %d: ", i + 1);
        scanf(" %[^\n]", jog_aux.nome); 

        if (i % 2 == 0) //Separando as duplas de acordo com o valor de i:
            jog_aux.dupla = 1;
        else
            jog_aux.dupla = 2;

        jog_aux.cartasRestantes = 0;
        insereFila(Jogadores, jog_aux);
    }
}

void prox_jogador(tp_fila *Jogadores) { //Função que movimenta a fila de jogadores
    tp_jogador proximo;
    removeFila(Jogadores, &proximo); //retira o jogador da primera posição
    insereFila(Jogadores, proximo); //reinsere o jogador no fim da fila
}

void defineOrdem(tp_fila *Jogadores) {
    //Determinando a ordem do jogo na função void utilizando a mão atual (cujo parâmetro é vetor - passagem por referência):
    tp_jogador jogador_atual;
    topoFila(Jogadores, &jogador_atual);
    printf("Vez de: %s (Dupla %d)\n", jogador_atual.nome, jogador_atual.dupla);
}

void mostrarDuplas(tp_fila Jogadores) { //Apresenta o jogador atual e sua dupla
    for (int i = 0; i < nJogadores; i++) {
        tp_jogador atual;
        topoFila(&Jogadores, &atual);
        printf("O nome do jogador %d eh: %s | Dupla: %d\n", i + 1, atual.nome, atual.dupla);
        prox_jogador(&Jogadores); //anda a fila de jogadores a fim de mostrar o próximo na iteração seguinte
    }
}

int qualDuplaAtual(tp_fila *Jogadores) {
 //Função que retorna a dupla a qual o jogador atual pertence:
    tp_jogador atual;
    topoFila(Jogadores, &atual); //Descubro o jogador da vez
    return atual.dupla;  //Retorno sua dupla
}

void escolheuJogar(tp_fila *Jogadores, carta_jogada mesa[4], int numJogadorAtual) {
    //Função que permite que o primeiro jogador da fila escolha uma carta da própria mão e coloque-a na mesa (vez do jogador):
    carta_jogada cartaEscolhida;
    qual_carta_jogar(Jogadores, &cartaEscolhida); //Jogador seleciona a carta a ser jogada
    mesa[numJogadorAtual] = cartaEscolhida; //Coloca a carta selecionada pelo jogador na mesa
}

void situacaoAtual(tp_fila *Jogadores, carta_jogada mesa[4], carta *vira, int numJogadorAtual) {
    //Função que apresenta ao jogador as informações necessárias antes de realizar sua jogada para que decida o que fazer:
    printf("=============================\n");
    defineOrdem(Jogadores);
    mostra_vira(vira);
    mostrar_cartas_mesa(mesa, numJogadorAtual);
    mostra_mao(*Jogadores);
}

void vezJogador(tp_fila *Jogadores, carta_jogada mesa[4], carta *vira, int numJogadorAtual, tp_truco *trucoAtual, int placar[]) {
//Função que controla o que acontece durante a vez de cada jogador:
    int duplaAtual = qualDuplaAtual(Jogadores);
    int truco_possivel = podeTruco(trucoAtual, duplaAtual);
    situacaoAtual(Jogadores, mesa, vira, numJogadorAtual);

    switch (truco_possivel) {
        case 1: {
            printf("O que voce deseja fazer?\nJogar carta: pressione 1\nPedir truco: pressione 2\n");
            int acao;
            scanf("%d", &acao);
            if (acao == 1) {
                escolheuJogar(Jogadores, mesa, numJogadorAtual);
            } else if (acao == 2) {
                pedeTruco(trucoAtual, duplaAtual, placar, Jogadores);
                if (trucoAtual->maoEncerrada == -1) {
                    situacaoAtual(Jogadores, mesa, vira, numJogadorAtual);
                    escolheuJogar(Jogadores, mesa, numJogadorAtual);
                }
            } else {
                printf("Comando invalido, digite um comando valido\n");
                vezJogador(Jogadores, mesa, vira, numJogadorAtual, trucoAtual, placar);
            }
            break;
        }
        case 0:
            printf("Nao eh possivel pedir truco\n");
            escolheuJogar(Jogadores, mesa, numJogadorAtual);
            break;
    }
}

#endif