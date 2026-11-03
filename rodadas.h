/*======================= BIBLIOTECA DE DETERMINAÇÃO DA RODADA =======================*/
#ifndef rodadas_h
#define rodadas_h

#include <stdio.h>
#include <string.h>
#include "tipos.h"
#include "pilha.h"
#include "fila.h"
#include "baralho.h"
#include "jogador.h"
#include "truco.h"
#include "var.h"

int forcaValor(int numero) {
    //Retorna a força da carta baseada no índice do vetor referencia
    int referencia[10] = {4, 5, 6, 7, 11, 12, 13, 14, 2, 3};
    for (int i = 0; i < 10; i++) {
        if (numero == referencia[i])
            return i;
    }
    return -1;  //Em caso de erro (se o valor da carta não for nenhum dos valores válidos), retorna -1
}

int forcaNaipe(char naipe) {
    //Retorna a força da carta baseada em seu naipe, seguindo: O < E < C < P
    switch (naipe) {
        case 'O': return 1;
        case 'E': return 2;
        case 'C': return 3;
        case 'P': return 4;
        default:  return 0; //Em caso de erro (se o naipe da carta não for nenhum dos naipes válidos), retorna 0
    }
}

int comparaForca(carta a, carta b, carta manilha) {
    //Compara a força de 2 cartas, considerando a manilha
    // 1: carta A é a mais forte  |  -1: carta B é a mais forte  |  0: empate

    //Booleanos que verificam se cartas são manilha:
    int aManilha = (a.numero == manilha.numero) ? 1 : 0;
    int bManilha = (b.numero == manilha.numero) ? 1 : 0;

    //Casos em que 1 das cartas é manilha:
    if (aManilha && !bManilha) return 1;
    if (!aManilha && bManilha) return -1;

    //Caso as 2 são manilhas:
    if (aManilha && bManilha) {
        int Forca = forcaNaipe(a.naipe) - forcaNaipe(b.naipe);
        return (Forca < 0) ? -1 : (Forca > 0 ? 1 : 0);
    }

    //Caso as 2 tenham o mesmo número, compara os naipes:
    if (a.numero == b.numero) {
        int Forca = forcaNaipe(a.naipe) - forcaNaipe(b.naipe);
        return (Forca < 0) ? -1 : (Forca > 0 ? 1 : 0);
    } 
    //Caso contrário, compara os seus números:
    else { 
        int Forca = forcaValor(a.numero) - forcaValor(b.numero);
        return (Forca < 0) ? -1 : (Forca > 0 ? 1 : 0);
    }
}

void insertionSort(carta_jogada mesa[4], carta manilha) {
    //Ordena por força crescente das 4 cartas jogadas na mesa, considerando a manilha
    //POR QUE INSERTION SORT? Para um vetor pequeno (tamanho 4), esse algoritmo é de fácil implementação e com funcionamento análogo ao ordenamento real de cartas
    for (int i = 1; i < 4; i++) {
        carta_jogada chave = mesa[i];
        int j = i - 1;
        while (j >= 0 && comparaForca(chave.cartaJogada, mesa[j].cartaJogada, manilha) < 0) {
            mesa[j + 1] = mesa[j];
            j--;
        }
        mesa[j + 1] = chave;
    }
}

int resultadoRodada(carta_jogada mesa[4], carta manilha) {
    //Determina a dupla vencedora da rodada (1 ou 2, ou 0 em caso de empate)
    carta_jogada ordenadas[4];
    for (int i = 0; i < 4; i++)
        ordenadas[i] = mesa[i]; //Faz uma cópia da mesa para não alterar sua ordem original

    insertionSort(ordenadas, manilha); //Ordeno as cartas da mesa da menor para maior força
    int duplaVencedora = ordenadas[3].dupla; //Pego a dupla da última carta (a mais forte)

    //Verifico se há cartas empatadas com a vencedora:
    for (int i = 2; i >= 0; i--) {
        if (comparaForca(ordenadas[i].cartaJogada, ordenadas[3].cartaJogada, manilha) != 0)
            break;
        if (ordenadas[i].dupla != duplaVencedora) //Se as cartas mais fortes forem de duplas diferentes e de mesma força:
            return 0; // Empate!
    }
    return duplaVencedora;
}

int resultadoMao(int resultados[3], int duplaMao) {
    //Determina a dupla vencedora da rodada (1 ou 2) por meio do vetor que armazena os resultados das rodadas
    int rod1 = resultados[0];
    int rod2 = resultados[1];
    int rod3 = resultados[2];

    //Caso 1: primeira rodada empatada
    if (rod1 == 0) {
        if (rod2 != 0) return rod2; //se a segunda não empatar, ela é a decisiva:
        if (rod3 != 0) return rod3; //Se a segunda empatar, a terceira será a decisiva:
        return duplaMao; //Em caso de 3 empates, a primeira dupla a jogar é a vencedora
    }
    if (rod2 == 0) return rod1; //Se a segunda rodada empatar, a primeira é decisiva:
    if (rod1 == rod2) return rod1; //Se uma dupla vencer as 2 primeiras:
    if (rod3 != 0) return rod3; //Se a terceira rodada for a decisiva:
    return rod1; //Se a terceira empatar e essa é a rodada decisiva, a primeira dupla a ganhar 1 rodada vence
}

int executaRodada(tp_fila *Jogadores, carta *vira, carta *manilha, tp_truco *trucoAtual, int placar[], tp_listad *listaVAR, int maoAtual, int numRodada) {
    //Função que executa uma rodada completa, permitindo que os quatro jogadores joguem uma carta e, em seguida, determina qual dupla é vencedor, salvando no VAR
    carta_jogada mesa[4];

    // Limpa a mesa antes de começar (caso alguém corra antes dos 4 jogarem)
    for (int i = 0; i < 4; i++) {
        mesa[i].cartaJogada.numero = 0;
        mesa[i].cartaJogada.naipe = ' ';
        strcpy(mesa[i].jogador, "Nao jogou");
        mesa[i].dupla = 0;
    }

    // Ciclo para os 4 jogadores jogarem na mesa
    for (int i = 0; i < 4; i++) {
        if (trucoAtual->maoEncerrada != -1) {
            break; // Alguém correu do truco!
        }
        vezJogador(Jogadores, mesa, vira, i, trucoAtual, placar);
        // REMOVIDO o prox_jogador(Jogadores) daqui!
        // Motivo: qual_carta_jogar() já remove o jogador da frente e insere no fim da fila!
    }

    int vencedor = 0;
    int alguem_correu = 0;

    // Verifica se a rodada foi até o fim ou se alguém correu
    if (trucoAtual->maoEncerrada == -1) {
        vencedor = resultadoRodada(mesa, *manilha);
        alguem_correu = 0;
        if (vencedor == 0) printf("\n>>> Rodada %d empatou (Cangou)!\n", numRodada);
        else printf("\n>>> Dupla %d venceu a Rodada %d!\n", vencedor, numRodada);
    } else {
        alguem_correu = 1;
        vencedor = trucoAtual->duplapediu;
        printf("\n>>> Correram do truco! Dupla %d leva a mao!\n", vencedor);
    }

    //O VAR: salva tudo que aconteceu nesta rodada dentro da lista
    salvar_rodada(listaVAR, maoAtual, numRodada, mesa, vencedor, trucoAtual->quemTrucou, trucoAtual->quemRetrucou, alguem_correu);

    return vencedor;
}

void realizar_mao(tp_fila *Jogadores, int placar[], tp_listad *listaVAR, int maoAtual) {
    //Função que executa as 3 rodadas de uma mão, determina a dupla vencedora e atualiza quem será a próxima mão.
    tp_pilha baralho;
    inicializapilha(&baralho);
    randomizador(&baralho);

    carta vira, manilha;
    vira_manilha(&baralho, &manilha, &vira);
    distribui_cartas(&baralho, Jogadores);

    tp_truco trucoAtual;
    trucoAtual.valormao = 1;
    trucoAtual.duplapediu = 0;
    trucoAtual.esperandoAceitar = false;
    trucoAtual.maoEncerrada = -1;
    strcpy(trucoAtual.quemTrucou.nome, "Ninguem");
    strcpy(trucoAtual.quemRetrucou.nome, "Ninguem");

    int resultados[3] = {-1, -1, -1};
    int duplaMao = qualDuplaAtual(Jogadores); //Dupla que inicia a mão

    for (int r = 0; r < 3; r++) {
        printf("\n--- Iniciando Rodada %d da Mao %d ---\n", r + 1, maoAtual);
        resultados[r] = executaRodada(Jogadores, &vira, &manilha, &trucoAtual, placar, listaVAR, maoAtual, r + 1);

        // Se alguém correu no truco, soma os pontos e encerra a mão imediatamente
        if (trucoAtual.maoEncerrada != -1) {
            printf("\n*** Dupla %d venceu a Mao %d (adversario correu) e ganhou %d ponto(s)! ***\n",
                   trucoAtual.duplapediu, maoAtual, trucoAtual.valormao);
            printf("Placar Atual: Dupla 1 [%d] x [%d] Dupla 2\n\n", placar[0], placar[1]);
            return;
        }

        // Verifica se a mão já foi decidida na 2ª rodada
        if (r == 1) {
            if ((resultados[0] == resultados[1] && resultados[0] != 0) ||
                (resultados[0] == 0 && resultados[1] != 0) ||
                (resultados[1] == 0 && resultados[0] != 0)) {
                break;
            }
        }
    }
    // Calcula quem levou a mão pelas 3 rodadas e atualiza o placar
    int duplaVencedoraMao = resultadoMao(resultados, duplaMao);
    placar[duplaVencedoraMao - 1] += trucoAtual.valormao;
    printf("\n*** Dupla %d venceu a Mao %d e ganhou %d ponto(s)! ***\n", duplaVencedoraMao, maoAtual, trucoAtual.valormao);
    printf("Placar Atual: Dupla 1 [%d] x [%d] Dupla 2\n\n", placar[0], placar[1]);
}

#endif