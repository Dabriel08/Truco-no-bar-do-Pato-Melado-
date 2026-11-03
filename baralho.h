/*======================= BIBLIOTECA DE MANIPULAÇÃO DO BARALHO =======================*/
#ifndef baralho_h
#define baralho_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "tipos.h"
#include "fila.h"
#include "pilha.h"

void randomizador(tp_pilha* ordem) { //função para o embaralhamento
    int embaralhando[40]; //Vetor dos índices das cartas
    carta embaralhado[40]; //Vetor de cartas

    //vetor de referencia de cartas, está organizado em ordem crescente por força do jogo truco:
    carta baralho[40] = {
        {4,'O'}, {5,'O'}, {6,'O'}, {7,'O'}, {11,'O'}, {12,'O'}, {13,'O'}, {14,'O'}, {2,'O'}, {3,'O'},
        {4,'E'}, {5,'E'}, {6,'E'}, {7,'E'}, {11,'E'}, {12,'E'}, {13,'E'}, {14,'E'}, {2,'E'}, {3,'E'},
        {4,'C'}, {5,'C'}, {6,'C'}, {7,'C'}, {11,'C'}, {12,'C'}, {13,'C'}, {14,'C'}, {2,'C'}, {3,'C'},
        {4,'P'}, {5,'P'}, {6,'P'}, {7,'P'}, {11,'P'}, {12,'P'}, {13,'P'}, {14,'P'}, {2,'P'}, {3,'P'}
    };

    for (int i = 0; i < 40; i++) {
        embaralhando[i] = i;
    }

    //pega o número do vetor embaralhando e troca ele de lugar de acordo com um número aleatório, embaralhando os índices
    for (int i = 40 - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int suporte = embaralhando[i];
        embaralhando[i] = embaralhando[j];
        embaralhando[j] = suporte;
    }

    //pega o vetor de referência (baralho) e insere-o embaralhado no vetor embaralhado:
    for (int i = 0; i < 40; i++) {
        embaralhado[i] = baralho[embaralhando[i]]; // Simplificado para acesso direto O(1)
    }

    //pega o vetor embaralhado e insere-o em uma pilha:
    //OBJETIVO: a pilha permite que o baralho seja manipulado apenas pelo topo, atendendo às regras do truco.
    for (int i = 0; i < 40; i++) {
        push(ordem, embaralhado[i]);
    }
}

void vira_manilha(tp_pilha* baralho, carta* manilha, carta* vira) { //Função que mostra a carta vira e guarda o valor da manilha da rodada
    pop(baralho, vira);
    if (vira->numero == 7) {
        manilha->numero = 11; // Depois do 7 vem a Dama (11)
    } else if (vira->numero == 14) {
        manilha->numero = 2;  // Depois do Ás (14) vem o 2
    } else if (vira->numero == 3) {
        manilha->numero = 4;  // Depois do 3 vem o 4
    } else {
        manilha->numero = vira->numero + 1;
    }
}

void mostra_vira(carta* vira) {
    //Mostra a vira da rodada:
    printf("A carta vira eh: %d %c\n", vira->numero, vira->naipe);
}

void mostrar_cartas_mesa(carta_jogada mesa[4], int rodAtual) { //Função que mostra as cartas jogadas na mesa e seu respectivo jogador:
    if (rodAtual == 0) {
        printf("Ainda nao existem cartas na mesa, voce eh o primeiro a jogar!\n");
        return;
    }
    printf("Cartas da mesa:\n");
    for (int i = 0; i < rodAtual; i++) {
        printf("%d %c (%s)\n", mesa[i].cartaJogada.numero, mesa[i].cartaJogada.naipe, mesa[i].jogador);
    }
}

void distribui_cartas(tp_pilha* baralho, tp_fila* Jogadores) {  //distribui as cartas para os jogadores
    for (int i = 0; i < nJogadores; i++) { //cada iteração distribui as 3 cartas de um jogador
        tp_jogador aux, atual;
        topoFila(Jogadores, &atual); //seleciona o jogador do topo a fim de distribir sus cartas
        for (int j = 0; j < nCartas; j++) { //uma iteração para cada carta
            pop(baralho, &atual.mao[j]); //insere a carta na mão do jogador
        }
        atual.cartasRestantes = nCartas; //atualiza o número de cartas restantes na mão do jogador
        removeFila(Jogadores, &aux); //remove o tp_jogador que estava sem as cartas da fila
        insereFila(Jogadores, atual); // insere o tp_jogador com as 3 cartas dadas na fila
    }
}

void mostra_mao(tp_fila Jogadores) { //mostra na tela a mão do jogador
    tp_jogador aux;
    topoFila(&Jogadores, &aux); //seleciona o jogador do topo a fim de mostrar suas cartas, com a numeração e o naipe
    printf("A mao de %s eh:\n", aux.nome);
    for (int j = 0; j < aux.cartasRestantes; j++) {
        printf("[%d] %d %c\n", j + 1, aux.mao[j].numero, aux.mao[j].naipe);
    }
}

void qual_carta_jogar(tp_fila* Jogadores, carta_jogada* cartajogada) {
    //Função que pergunta qual carta o jogador deseja jogar, e atualiza a carta jogada na mesa (cartaJogada)
    tp_jogador aux, atual;
    topoFila(Jogadores, &atual); //seleciona o jogador do topo
    int cartaEscolhida; //inteiro para guardar a carta escolhida pelo jogador
    printf("Qual carta deseja jogar? (Digite um numero entre 1 e %d)\n", atual.cartasRestantes);
    scanf("%d", &cartaEscolhida);

    if (cartaEscolhida >= 1 && cartaEscolhida <= atual.cartasRestantes) { //Se a carta escolhida é válida, então atualiza a carta jogada na mesa:
        int j = cartaEscolhida - 1;
        cartajogada->cartaJogada = atual.mao[j]; //atualiza a carta jogada na mesa
        strcpy(cartajogada->jogador, atual.nome); //atualiza o jogador que jogou a carta
        cartajogada->dupla = atual.dupla; //atualiza a dupla do jogador que jogou a carta

        for (int k = j; k < atual.cartasRestantes - 1; k++) {
            atual.mao[k] = atual.mao[k + 1];
        }
        atual.cartasRestantes--;
        removeFila(Jogadores, &aux);
        insereFila(Jogadores, atual);

        printf("O jogador %s jogou a carta %d%c\n", cartajogada->jogador, cartajogada->cartaJogada.numero, cartajogada->cartaJogada.naipe); 
        return;
    }

    printf("Carta invalida. Tente novamente.\n"); //Se a carta escolhida é inválida, então apresenta erro e faz chamada recursiva da função
    qual_carta_jogar(Jogadores, cartajogada);
}

#endif