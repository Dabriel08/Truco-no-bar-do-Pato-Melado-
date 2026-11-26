/*======================= CÓDIGO MAIN DO JOGO =======================*/
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

#define nCartas 3
#define nJogadores 4

int main() {
    /*__________ Inicializando o Jogo __________*/
    // A semente de randomização do código
    srand(time(NULL));

    // Declarando e cadastrando os jogadores:
    tp_fila JOGADORES;
    inicializaFila(&JOGADORES);
    organizaJogadores(&JOGADORES);
    printf("\n");

    // Inicializando a lista duplamente encadeada do VAR:
    tp_listad *listaVAR = inicializa_listad();

    carta manilha;
    int MAOatual;
    //inicializando a mão atual:
    inicializaMao(&MAOatual);

    int placar[2];
    inicializaPlacar(placar); // Zera a pontuação

    // Loop principal da partida até alguma dupla atingir a pontuação de vitória
    while (vencedorPartida(placar) == 0) {
        printf("========================================\n");
        printf("Mao atual: %d \n\n", MAOatual + 1);
        
        // Executa a mão (até 3 rodadas) e já salva cada rodada no listaVAR
        realizar_mao(&JOGADORES, placar, listaVAR, MAOatual + 1);
        
        // Atualiza a fila para que na próxima mão a ordem "ande" um jogador
        prox_jogador(&JOGADORES);
        MAOatual++;
    }

    printf("\nParabens dupla %d! Voces venceram a partida!!!\n", vencedorPartida(placar));

    // Pergunta se a galera quer puxar o histórico completo do jogo no VAR
    int opcaoVAR;
    printf("\nDeseja exibir o historico completo do VAR? (1 - Sim / 0 - Nao): ");
    scanf("%d", &opcaoVAR);
    
    if (opcaoVAR == 1) {
        exibir_VAR(listaVAR);

    }

    return 0;
}