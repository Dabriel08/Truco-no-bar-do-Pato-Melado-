/*======================= BIBLIOTECA DE CADASTRO DOS JOGADORES =======================*/
#ifndef jogador_h
#define jogador_h
#include "tipos.h"
#include "pilha.h"
#include "fila.h"
#include "baralho.h"
#include "truco.h"
#include "arvoreavl.h"
#include "caderneta.h"
#define nCartas 3
#define nJogadores 4

//Criando e cadastrando os 4 jogadores na função void (cujo parâmetro é vetor - passagem por referência), já separando-os em duplas:


void organizaJogadores(ArvAVL* Caderneta, tp_fila* Jogadores) {
    //Lendo e armazenando dados numa fila de jogadores:
    /*OBJETIVO: a fila possibilita que os jogadores sejam organizados em um array que segue uma ordem de chegada,
    fazendo com que a roda de jogo seja respeitada do início ao fim */

    for (int i = 0; i < nJogadores; i++) {
        tp_jogador jog_aux;
        perguntaNomeJogador(Caderneta, jog_aux.nome, i + 1); //essa funcao garante que um nome ja existente na caderneta seja utilizado novamente sem o usario saber que ele ja esta na caderneta, evitando cadastros erroneos

        //Separando as duplas de acordo com o valor de i:
        if (i % 2 == 0)
            jog_aux.dupla = 1;

        else
            jog_aux.dupla = 2;

        insereFila(Jogadores, jog_aux);
    }

    atualizaPartidasJogadas(Caderneta, Jogadores); //adiciona um ao numero de partidas jogadas de cada jogador na caderneta
}

void inicializaMao(int* maoAtual) {
    //Inicializa a mão atual para as próximas operações
    *maoAtual = -1;
}

int defineMaoSeguinte(int* maoAtual) {
    //Determinando a próxima "mão" do jogo, mudando quem é a mão (parâmetro é ponteiro de inteiro, com passagem por referência para atualizar o valor da mão atual)
    int maoProx = ((*maoAtual) + 1) % nJogadores; //o trecho "% nJogadores" garante que a maoProx sempre será um valor entre 0 e 3, sem acessar um 'jogador' inexistente

    *maoAtual = maoProx;
    return (*maoAtual);
}

//Determinando a ordem do jogo na função void utilizando a mão atual (cujo parâmetro é vetor - passagem por referência):
void defineOrdem(tp_fila* Jogadores) {
    tp_jogador jogador_atual;
    topoFila(Jogadores, &jogador_atual);

    //Para indicar quem está jogando no momento, usa-se o índice da maoAtual - Determina-se a mão para escolher o jogador da vez pelo nome
    printf("Vez de: %s\n", jogador_atual.nome);
}

void mostrarDuplas(tp_fila Jogadores) { //Apresenta o jogador atual e sua dupla
    for (int i = 0; i < nJogadores; i++) {
        tp_jogador atual;
        topoFila(&Jogadores, &atual);
        printf("O nome do jogador %d eh: %s \n", i + 1, atual.nome);
        printf("A dupla do jogador %d eh: %d \n", i + 1, atual.dupla);
        prox_jogador(&Jogadores); //anda a fila de jogadores a fim de mostrar o próximo na iteração seguinte
    }
}

int qualDuplaAtual(tp_fila* Jogadores) {
    //Função que retorna a dupla a qual o jogador atual pertence:
    tp_jogador atual;
    topoFila(Jogadores, &atual); //Descubro o jogador da vez
    return atual.dupla; //Retorno sua dupla
}

void escolheuJogar(tp_fila* Jogadores, carta_jogada mesa[4], int numJogadorAtual) {
    //Função que permite que o primeiro jogador da fila escolha uma carta da própria mão e coloque-a na mesa (vez do jogador): 
    carta_jogada cartaEscolhida;
    qual_carta_jogar(Jogadores, &cartaEscolhida); //Jogador seleciona a carta a ser jogada
    mesa[numJogadorAtual] = cartaEscolhida; //Coloca a carta selecionada pelo jogador na mesa
}

void escolheuRoubar(tp_fila* jogadores, tp_pilha* baralho, int* repete) {
    //Função que permite que o primeiro jogador da fila escolha uma carta para tentar a sorte trocando-a com outra carta aleatória
    *repete = tentar_a_sorte(jogadores, baralho);
}

void situacaoAtual(tp_fila* Jogadores, carta_jogada mesa[4], carta* vira, int numJogadorAtual) {
    //Função que apresenta ao jogador as informações necessárias antes de realizar sua jogada para que decida o que fazer:
    printf("=============================\n");
    defineOrdem(Jogadores); //mostra de quem é a vez de jogar
    mostra_vira(vira); //mostra a carta vira da vez
    mostrar_cartas_mesa(mesa, numJogadorAtual); //Mostra as cartas já jogadas e seus donos, simulando um jogo real e evitando confusões
    mostra_mao(*Jogadores); //Jogador vê suas cartas na tela
}

int vezJogador(tp_fila* Jogadores, carta_jogada mesa[4], carta* vira, int numJogadorAtual, tp_truco* trucoAtual, int placar[], ArvAVL* Caderneta, tp_pilha* baralho) {
    //Função que controla o que acontece durante a vez de cada jogador:

    //Reunindo as informações da jogada:
    int repete = 0;
    int duplaAtual = qualDuplaAtual(Jogadores);
    int truco_possivel = podeTruco(trucoAtual, duplaAtual);
    situacaoAtual(Jogadores, mesa, vira, numJogadorAtual);
    switch (truco_possivel) {
    case 1: //Caso o jogador possa pedir truco:
        printf("O que voce deseja fazer?\nJogar carta: pressione 1\nPedir truco: pressione 2\nTentar a sorte: pressione 3\n");
        int acao;
        scanf("%d", &acao); //Identifica o que o jogador deseja fazer no momento atual
        //1: Jogar carta || 2: Aumentar a pontuação da mão
        if (acao == 1) {
            escolheuJogar(Jogadores, mesa, numJogadorAtual);
        }
        else if (acao == 2) {
            pedeTruco(trucoAtual, duplaAtual, placar, *Jogadores, Caderneta);
            if (trucoAtual->maoEncerrada == -1) { //se a mão não foi encerrada, o jogador ainda deve jogar, dando continuidade à partida
                situacaoAtual(Jogadores, mesa, vira, numJogadorAtual); //exibe novamente a situação, pois - com muitas rodadas de truco e retruco - a negociação do truco pode ter alterado o estado da partida
                escolheuJogar(Jogadores, mesa, numJogadorAtual);
            }
        }
        else if (acao == 3) {
            escolheuRoubar(Jogadores, baralho, &repete);
            if (repete == 1) {
                vezJogador(Jogadores, mesa, vira, numJogadorAtual, trucoAtual, placar, Caderneta, baralho);
            }
            else {
                printf("Voce foi descoberto roubando!!\n");
                return (duplaAtual == 1) ? 2 : 1; // Retorna a dupla oposta
            }
        }
        else { //Caso o jogador digite uma entrada inválida, apresenta erro e faz a chamada recursiva da função
            printf("Comando invalido, digite um comando valido\n");
            return vezJogador(Jogadores, mesa, vira, numJogadorAtual, trucoAtual, placar, Caderneta, baralho);
        }
        break;

    case 0: //Caso pedir truco não seja possível:
        printf("Nao eh possivel pedir truco\n");
        escolheuJogar(Jogadores, mesa, numJogadorAtual);
        break;
    }
    return 0;
}

void atualizaOrdem(tp_fila* Jogadores, tp_jogador jogInicial) {
    tp_jogador aux;
    topoFila(Jogadores, &aux);
    while (strcmp(aux.nome, jogInicial.nome) != 0) {
        prox_jogador(Jogadores); //anda a fila de jogadores ate que o jogador que iniciou a mao fique em primeiro de novo
        topoFila(Jogadores, &aux);
    }
    prox_jogador(Jogadores); //anda uma posicao para que na proxima mao o jogador depois do jogador que iniciou a mao passe a iniciar
}


#endif 