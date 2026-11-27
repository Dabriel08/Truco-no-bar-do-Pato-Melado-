/*======================= BIBLIOTECA DE DETERMINAÇÃO DA RODADA =======================*/
#ifndef rodadas_h
#define rodadas_h

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include "tipos.h"
#include "pilha.h"
#include "fila.h"
#include "baralho.h"
#include "jogador.h"
#include "truco.h"
#include "var.h"
#include "mural.h"
#include "arvoreavl.h"
#include "caderneta.h"

int forcaValor(int numero) {
    //Retorna a força da carta baseada no índice do vetor referencia
    int referencia[10] = { 4,5,6,7,11,12,13,14,2,3 };

    for (int i = 0; i < 10; i++) {
        if (numero == referencia[i])
            return i;
    }

    return -1; //Em caso de erro (se o valor da carta não for nenhum dos valores válidos), retorna -1
}

int forcaNaipe(char naipe) {
    //Retorna a força da carta baseada em seu naipe, seguindo: O < E < C < P
    switch (naipe) {
    case 'O':
        return 1;  // O = Ouros
    case 'E':
        return 2;  // E = Espadas
    case 'C':
        return 3;  // C = Copas
    case 'P':
        return 4;  // P = Paus
    default:
        return 0; //Em caso de erro (se o naipe da carta não for nenhum dos naipes válidos), retorna 0
    }
}

int comparaForca(carta a, carta b, carta manilha) {
    //Compara a força de 2 cartas, considerando a manilha
    // 1: carta A é a mais forte  |  -1: carta B é a mais forte  |  0: empate

    int aManilha = 0, bManilha = 0;  //Booleanos que verificam se cartas são manilha

    if (a.numero == manilha.numero)
        aManilha = 1;
    if (b.numero == manilha.numero)
        bManilha = 1;

    //Casos em que 1 das cartas é manilha:
    if (aManilha && !bManilha)
        return 1;

    else if (!aManilha && bManilha)
        return -1;

    //Caso as 2 são manilhas:
    if (aManilha && bManilha) {
        int Forca = forcaNaipe(a.naipe) - forcaNaipe(b.naipe);

        if (Forca < 0)
            return -1;
        else if (Forca > 0)
            return 1;
        else
            return 0;
    }

    //Caso as 2 tenham o mesmo número, compara os naipes:
    if (a.numero == b.numero) {
        int Forca = forcaNaipe(a.naipe) - forcaNaipe(b.naipe);

        if (Forca < 0)
            return -1;
        else if (Forca > 0)
            return 1;
        else
            return 0;
    }
    //Caso contrário, compara os seus números:
    else {
        int Forca = forcaValor(a.numero) - forcaValor(b.numero);

        if (Forca < 0)
            return -1;
        else if (Forca > 0)
            return 1;
        else
            return 0;
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
            return 0;
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
        //se a segunda não empatar, ela é a decisiva:
        if (rod2 != 0)
            return rod2;

        //Se a segunda empatar, a terceira será a decisiva:
        if (rod3 != 0)
            return rod3;

        return duplaMao; //Em caso de 3 empates, a primeira dupla a jogar é a vencedora
    }

    if (rod2 == 0) //Se a segunda rodada empatar, a primeira é decisiva:
        return rod1;

    if (rod1 == rod2) //Se uma dupla vencer as 2 primeiras:
        return rod1;

    if (rod3 != 0) //Se a terceira rodada for a decisiva:
        return rod3;

    return rod1; //Se a terceira empatar e essa é a rodada decisiva, a primeira dupla a ganhar 1 rodada vence
}


int executaRodada(tp_fila* Jogadores, carta* vira, carta* manilha, tp_truco* trucoAtual, int placar[], tp_listad* listaVAR, int maoAtual, int numRodada, ArvAVL* Caderneta, tp_pilha* baralho) {
    //Função que executa uma rodada completa, permitindo que os quatro jogadores joguem uma carta e, em seguida, determina qual dupla é vencedor, salvando no VAR
    carta_jogada mesa[4];

    strcpy(trucoAtual->quemTrucou.nome, ""); //inicializa as informacoes sobre pedidos de truco
    trucoAtual->quemRetrucou = -1;


    //Limpa a mesa antes de começar (caso alguém corra antes dos 4 jogarem)
    for (int i = 0; i < 4; i++) {
        mesa[i].cartaJogada.numero = 0;
        mesa[i].cartaJogada.naipe = ' ';
        strcpy(mesa[i].jogador, "Nao jogou");
        mesa[i].dupla = 0;
    }

    int perderamroubando = 0;

    //Loop para os 4 jogadores jogarem na mesa
    for (int i = 0; i < 4; i++) {
        if (trucoAtual->maoEncerrada != -1) {
            break; //Se alguém correr do truco, interrompe a rodada
        }
        perderamroubando = vezJogador(Jogadores, mesa, vira, i, trucoAtual, placar, Caderneta, baralho);
        if (perderamroubando != 0) {
            break; //Se alguem foi pego roubando, interrompe a rodada
        }
    }

    int vencedor = 0;
    int alguem_correu = 0;

    //Verifica se a rodada foi até o fim ou se alguém correu
    if (perderamroubando != 0) {
        vencedor = perderamroubando;
        alguem_correu = 0;
        printf(" Dupla %d venceu a Rodada %d (adversario pego roubando)!\n", vencedor, numRodada);
    }
    else if (trucoAtual->maoEncerrada == -1) {
        vencedor = resultadoRodada(mesa, *manilha); //Usa a sua função com Insertion Sort!
        alguem_correu = 0;
        if (vencedor == 0) printf("\n>>> Rodada %d empatou (Cangou)!\n", numRodada);
        else printf("\n>>> Dupla %d venceu a Rodada %d!\n", vencedor, numRodada);
    }
    else {
        alguem_correu = 1;
        // Se maoEncerrada guardou quem correu ou quem ganhou, garantimos a dupla vencedora
        vencedor = trucoAtual->duplapediu;
        printf("\n>>> Correram do truco! Dupla %d leva a mao!\n", vencedor);
    }

    //O VAR: Salva tudo que rolou nesta rodada dentro da lista
    salvar_rodada(listaVAR, maoAtual, numRodada, mesa, vencedor, trucoAtual->quemTrucou, trucoAtual->quemRetrucou, alguem_correu);

    return vencedor;
}

void registrarDuplaVencedora(tp_fila Jogadores, int duplaVencedora) {
    //Função que envia a dupla vencedora para o mural
    tp_jogador atual;

    for (int i = 0; i < nJogadores; i++) {
        removeFila(&Jogadores, &atual); //Percorro a fila de jogadores

        if (atual.dupla == duplaVencedora) { //Descubro os nomes dos integrantes da dupla vencedora
            registrarMaoGanha(atual.nome);
        }
    }
}

void realizar_mao(tp_fila* Jogadores, int placar[], tp_listad* listaVAR, int maoAtual, ArvAVL* Caderneta) {
    //Função que executa as 3 rodadas de uma mão, determina a dupla vencedora e atualiza quem será a próxima mão.
    tp_pilha baralho;
    inicializapilha(&baralho);
    randomizador(&baralho);

    carta vira, manilha;
    vira_manilha(&baralho, &manilha, &vira);
    distribui_cartas(&baralho, Jogadores);

    tp_jogador jogInicial;
    topoFila(Jogadores, &jogInicial); //salva o jogador que inicia a mao, evitando ordem equivocada na proxima mao caso algum jogador corra de um truco

    tp_truco trucoAtual;
    trucoAtual.valormao = 1;
    trucoAtual.duplapediu = 0;
    trucoAtual.esperandoAceitar = false;
    trucoAtual.maoEncerrada = -1;

    int resultados[3] = { -1, -1, -1 };
    int duplaMao = qualDuplaAtual(Jogadores); //Dupla que inicia a mão

    for (int r = 0; r < 3; r++) {
        printf("\n--- Iniciando Rodada %d da Mao %d ---\n", r + 1, maoAtual);
        resultados[r] = executaRodada(Jogadores, &vira, &manilha, &trucoAtual, placar, listaVAR, maoAtual, r + 1, Caderneta, &baralho);

        //Se alguém correu no truco, a mão acaba imediatamente (pedeTruco já soma os pontos ou somamos aqui) e salvamos no mural
        if (trucoAtual.maoEncerrada != -1) {
            int vencedora = trucoAtual.duplapediu;
            registrarDuplaVencedora(*Jogadores, vencedora);
            atualizaMaosGanhas(Caderneta, Jogadores, vencedora);
            printf("\n*** Dupla %d venceu a Mao %d e ganhou %d ponto(s)! ***\n", vencedora, maoAtual, trucoAtual.valormao);
            printf("Placar Atual: Dupla 1 [%d] x [%d] Dupla 2\n\n", placar[0], placar[1]);
            atualizaOrdem(Jogadores, jogInicial); //atualiza a ordem dos jjogadores para a proxima mao
            return;
        }

        //Verifica se a mão já foi decidida na 2ª rodada (ex: Dupla 1 ganhou as duas primeiras, ou empatou uma e ganhou outra)
        if (r == 1) {
            if ((resultados[0] == resultados[1] && resultados[0] != 0) ||
                (resultados[0] == 0 && resultados[1] != 0) ||
                (resultados[1] == 0 && resultados[0] != 0)) {
                break; // Não precisa jogar a 3ª rodada!
            }
        }
    }

    //Calcula quem levou a mão pelas 3 rodadas e atualiza o placar
    int duplaVencedoraMao = resultadoMao(resultados, duplaMao);
    registrarDuplaVencedora(*Jogadores, duplaVencedoraMao);
    atualizaMaosGanhas(Caderneta, Jogadores, duplaVencedoraMao);
    placar[duplaVencedoraMao - 1] += trucoAtual.valormao;
    printf("\n*** Dupla %d venceu a Mao %d e ganhou %d ponto(s)! ***\n", duplaVencedoraMao, maoAtual, trucoAtual.valormao);
    printf("Placar Atual: Dupla 1 [%d] x [%d] Dupla 2\n\n", placar[0], placar[1]);

    atualizaOrdem(Jogadores, jogInicial);//atualiza a ordem dos jogadores para a proxima mao
}

int determinaMaoMaisCara(int valoresMao[], int quantidade) {
    //Devolve a posição da mão mais cara no vetor, ou -1 se nenhuma mão foi jogada.
    if (quantidade <= 0)
        return -1; // nenhuma mão foi jogada, então não há como determinar a mais cara, utilizando -1 como valor de retorno para indicar isso

    int posMaior = 0; // começa assumindo que a primeira mão(posição 0) e a mais cara

    for (int i = 1; i < quantidade; i++) { // começa no 1 porque a posição 0 já foi assumida como a mais cara
        if (valoresMao[i] > valoresMao[posMaior])  // achou uma mão mais cara? então atualiza a posição da mão mais cara
            posMaior = i;                          // '>' e não '>=' porque se forem iguais, a primeira mão é a mais cara (regra do jogo)
    }

    return posMaior;
}

int skill_check() {
    double duracao_total = 5.0;
    double inicio_alvo = ((double)rand() / RAND_MAX) * 4.0;
    double fim_alvo = inicio_alvo + 1.0;

    //limpa o buffer de teclas pressionadas

    while (_kbhit()) {
        _getch();
    }

    printf("Para trocar sua carta não podia ser tão fácil assim.\nAlguem está de olho bem aberto para você\nConsegue pegar outra carta mesmo assim?\n");
    printf("Pressione Espaço quando aparecer '>>> AGORA <<<'\n");
    printf("Prepare-se...\n");

    clock_t tempo_inicio = clock();
    int aviso = 0;

    while (1) {
        double tempo_passou = (double)(clock() - tempo_inicio) / CLOCKS_PER_SEC;

        if (tempo_passou >= duracao_total) {
            return 0; // Falhou no teste por não ter apertado a tecla a tempo
        }

        if (tempo_passou >= inicio_alvo && tempo_passou <= fim_alvo && !aviso) {
            printf(">>> AGORA <<<\n");
            aviso = 1;
        }

        if (_kbhit()) {
            char tecla = _getch();
            if (tecla == ' ') {
                if (tempo_passou >= inicio_alvo && tempo_passou <= fim_alvo) {
                    return 1;
                }
                else {
                    return 0;
                }
            }
        }
    }
}


int tentar_a_sorte(tp_fila* Jogadores, tp_pilha* baralho) {

    //Função que pergunta qual carta o jogador deseja trocar, e atualiza a mão do jogador
    tp_jogador aux, atual;
    topoFila(Jogadores, &atual); //seleciona o jogador do topo
    int cartaEscolhida; //inteiro para guardar a carta escolhida pelo jogador
    printf("Qual carta deseja trocar? (Digite um numero entre 1 e %d)\n(Digite 0 para cancelar)\n", atual.cartasRestantes);
    scanf("%d", &cartaEscolhida);

    if (cartaEscolhida >= 0 && cartaEscolhida <= atual.cartasRestantes) { //Se a carta escolhida é válida, então atualiza a carta jogada na mesa:
        if (cartaEscolhida == 0) {
            return 1;
        }

        if (skill_check() == 0) {
            return 0;
        }

        int j = cartaEscolhida - 1;

        carta novaCarta;
        pop(baralho, &novaCarta); //Pega a nova carta do baralho

        removeFila(Jogadores, &atual);
        atual.mao[j] = novaCarta; //Atualiza a carta escolhida na mão do jogadorh
        insereFila(Jogadores, atual);

        for (int i = 0; i < nJogadores - 1; i++) {
            removeFila(Jogadores, &aux);
            insereFila(Jogadores, aux);
        }

        return 1;
    }

    printf("Carta invalida. Tente novamente.\n"); //Se a carta escolhida é inválida, então apresenta erro e faz chamada recursiva da função
    return tentar_a_sorte(Jogadores, baralho);
}


#endif