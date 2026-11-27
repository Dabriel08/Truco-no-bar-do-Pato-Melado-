/*======================= CÓDIGO MAIN DO JOGO =======================*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "tipos.h"
#include "pilha.h"
#include "fila.h"
#include "listade.h"
#include "baralho.h"
#include "truco.h"
#include "jogador.h"
#include "var.h"
#include "rodadas.h"
#include "caderneta.h"
#include "estatistica.h"

#define nCartas 3
#define nJogadores 4

int main() {
    /*__________ Inicializando o Jogo __________*/
    // A semente de randomização do código
    srand(time(NULL));

    // Iniciaalizando a arvoreAVL da caderneta
    ArvAVL* CADERNETA; //ANAAAAAAAA eu n sei se é exatamente ass q faz p salvar em disco, se tem q mexer mto
    CADERNETA = criarAVL();
    carregarCaderneta(CADERNETA);

    // Declarando e cadastrando os jogadores:
    tp_fila JOGADORES;
    inicializaFila(&JOGADORES);
    organizaJogadores(CADERNETA, &JOGADORES);
    printf("\n");

    // Inicializando a lista duplamente encadeada do VAR:
    tp_listad* listaVAR = inicializa_listad();

    carta manilha;
    int MAOatual;
    //inicializando a mão atual:
    inicializaMao(&MAOatual);

    int placar[2];
    inicializaPlacar(placar); // Zera a pontuação

	int valoresMao[MAX_MAOS]; // valoresMao[i] = quantos pontos valeu a mao de posicao i
	int duplasMao[MAX_MAOS];  //duplasMao = qual dupla (1 ou 2) ganhou essa mão
	int totalMaos = 0;        //totalMaos = quantas mãos foram jogadas até o momento (e a proxima posição livre dos vetores)

    // Loop principal da partida até alguma dupla atingir a pontuação de vitória
    while (vencedorPartida(placar) == 0) {
        printf("========================================\n");
        printf("Mao atual: %d \n\n", (MAOatual + 1));

		int antes[2] = { placar[0], placar[1] };  // Salva o placar antes da mão atual para calcular quantos pontos a mão valeu

		realizar_mao(&JOGADORES, placar, listaVAR, MAOatual + 1, CADERNETA); // realiza a mão atual, atualizando o placar e o VAR

       
		valoresMao[totalMaos] = (placar[0] - antes[0]) + (placar[1] - antes[1]); // só uma das duplas pontua, então a diferença de pontos no placar indica quantos pontos a mão valeu
		duplasMao[totalMaos] = (placar[0] > antes[0]) ? 1 : 2;  // se a dupla 1 pontuou, então ela ganhou a mão, senão a dupla 2 ganhou a mão
		totalMaos++;                                           // proxima mão vai para a próxima posição

        MAOatual++;
    }

    printf("\nParabens dupla %d! Voces venceram a partida!!!\n", vencedorPartida(placar));

    // Estatística extra: mural da vergonha (taxa de "correu do truco" de cada jogador)
    exibirtaxascorrida();
    exibirmaomaiscara(valoresMao, duplasMao, totalMaos);

    // Pergunta se a galera quer puxar o histórico completo do jogo no VAR
    int opcaoVAR;
    printf("\nDeseja exibir o historico completo do VAR? (1 - Sim / 0 - Nao): ");
    scanf("%d", &opcaoVAR);

    if (opcaoVAR == 1) {
        exibir_VAR(listaVAR);

    }

    abrirCaderneta(*CADERNETA);

    return 0;
}