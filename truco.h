/*======================= BIBLIOTECA DE PONTUAÇÃO DO TRUCO =======================*/

#ifndef truco_h
#define truco_h
#include "tipos.h"
#include "fila.h"
#include "mural.h"
#include <stdbool.h> //permite o uso do bool, true e false
#include "arvoreavl.h"
#include "caderneta.h"

#define ptv 12 // pontos para ganhar a partida

int pontuaMao(int placar[], int dupla, int pontos) { //Adiciona os pontos ao placar da dupla vencedora
    //Retorna 1: a dupla pontua e é válida || Retorna 0: dupla inválida
    if (dupla != 1 && dupla != 2)
        return 0;
    placar[dupla - 1] += pontos;
    return 1;
}

int vencedorPartida(int placar[]) { //Função que verifica se alguma dupla atingiu a pontuação necessária (12) para vencer a partida
    //Retorna 1 ou 2: número da dupla vencedora  ||  Retorna 0: não há vencedor ainda
    if (placar[0] >= ptv)
        return 1;
    else if (placar[1] >= ptv)
        return 2;

    return 0;
}

void inicializaPlacar(int placar[2]) { //Função que inicializa o placar da partida
    placar[0] = 0;
    placar[1] = 0;
}

void inicializaTruco(tp_truco* t) { //Função que inicializa as informações de negociação do truco no começo da mão
    t->valormao = 1;
    t->duplapediu = 0;
    t->esperandoAceitar = false;
    t->maoEncerrada = -1;
}

int proximoValor(int valor) {
    //Função que retorna qual será o próximo valor da mão quando uma dupla pedir truco
    //Caso seja 1, retorna 3 || Caso seja 3, retorna 6 || Caso seja 6, retorna 12 || Caso seja 12, retorna 0.
    if (valor == 1)
        return 3;
    if (valor == 3)
        return 6;
    if (valor == 6)
        return 12;

    return 0;
}

int duplaRival(int dupla) {
    //Função que retorna a dupla rival
    //Caso seja 1, retorna 2 || Caso seja 2, retorna 1.
    return 3 - dupla;
}

int podeTruco(tp_truco* t, int dupla) {
    //Função que determina se a dupla tem permissão para pedir truco nesse momento:
    if (dupla != 1 && dupla != 2)
        return 0;
    if (t->esperandoAceitar) //Já existe um truco pendente, então não é possível pedir outro
        return 0;
    if (dupla == t->duplapediu) //A mesma dupla não pode pedir duas vezes seguidas
        return 0;
    if (proximoValor(t->valormao) == 0) //Não é possível pedir truco se o valor da mão já for 12
        return 0;
    return 1;
}

int aceitaTruco(tp_truco* t, int dupla) { // Função que retorna 1 caso a dupla aceite o truco, caso contrário retorna 0 (e nada é alterado).
    //OBS: a mão continua 
    if (!t->esperandoAceitar) //Não existe um truco pendente, então não é possível aceitar
        return 0;
    if (dupla != duplaRival(t->duplapediu)) //Apenas a dupla rival pode aceitar o truco
        return 0;

    t->valormao = proximoValor(t->valormao); //Atualiza o valor da mão para o próximo valor
    t->esperandoAceitar = false;
    t->quemRetrucou = dupla;
    printf("A dupla %d aceitou o truco!! Agora essa mao esta valendo %d pontos!\n", dupla, t->valormao);
    return 1;
}



int correTruco(tp_truco* t, int dupla, int placar[]) { //Função que retorna 1 caso a dupla corra do truco, caso contrário retorna 0 (e nada é alterado).
    //OBS: a mão é encerrada
    if (!t->esperandoAceitar)   //Caso não exista um truco pendente, então não é possível correr
        return 0;
    if (dupla != duplaRival(t->duplapediu)) //Apenas a dupla rival pode correr do truco
        return 0;

    pontuaMao(placar, t->duplapediu, t->valormao);  //Pontua a dupla que pediu o truco com o valor da mão atual
    t->esperandoAceitar = false;
    t->quemRetrucou = dupla;
    t->maoEncerrada = t->duplapediu; //Indica a dupla que ganhou, ou seja, a que pediu o truco
    printf("A dupla %d correu! Mao encerrada\n", dupla);
    return 1;
}

int podeAumentar(tp_truco* t, int dupla) { //Função que verifica se a dupla que recebeu o pedido pode aumentar o valor da mão
    //Retorna 1: é possível aumentar || Retorna 0: não é possível
    if (!t->esperandoAceitar)
        return 0;
    if (dupla != duplaRival(t->duplapediu))
        return 0;
    if (proximoValor(proximoValor(t->valormao)) == 0) // não é possível aumentar o truco se o valor da mão já for 12
        return 0;
    return 1;
}

int aumentaTruco(tp_truco* t, int dupla, int placar[]) { //Função que retorna 1 caso a dupla aumente o truco, caso contrário retorna 0 (e nada é alterado).
    if (!podeAumentar(t, dupla))
        return 0;

    t->valormao = proximoValor(t->valormao); //Aceita o truco e aumenta o valor da mão para o próximo valor
    t->duplapediu = dupla;   //Quem pediu o aumento do truco agora é a dupla rival(a que aceitou o truco)
    t->quemRetrucou = dupla;
    printf("A dupla %d aumentou o truco!\n", dupla);
    return 1;
}

void registraRespostaFregues(int correu, char nome1[], char nome2[]) {
    //Função que registra a resposta ao truco da dupla que recebe o pedido e o envia para o mural.
    registrarRespostaTruco(nome1, correu);
    registrarRespostaTruco(nome2, correu);
}

void respostaTruco(tp_truco* t, int dupla, int placar[], tp_fila* Jogadores, ArvAVL* Caderneta) { //Função que controla a interação entre as 2 duplas do truco
    //OBS: dupla é a equipe que tem q responder, ou seja, rival da que pediu o truco

    tp_fila copia = *Jogadores;
    tp_jogador atual;

    tp_jogador jogador1, jogador2;
    int encontrados = 0;

    while (encontrados < 2) {
        removeFila(&copia, &atual);

        if (atual.dupla == dupla) {
            if (encontrados == 0)
                jogador1 = atual;
            else
                jogador2 = atual;
            encontrados++;
        }
    }

    if (t->valormao < 6) {
        printf("Dupla %d, o que voces vao fazer? (A mao passara a valer %d pontos)\nAceitar truco: pressione 1\nCorrer: pressione 2\nAumentar truco: pressione 3\n", dupla, proximoValor(t->valormao));
        int resp;
        scanf("%d", &resp); //Identifica o que o jogador deseja fazer no momento atual
        // 1: Aceita o truco || 2: Corre do truco || 3: Aumenta a aposta

        switch (resp) {
        case 1:
            registraRespostaFregues(0, jogador1.nome, jogador2.nome);
            aceitaTruco(t, dupla);
            break;
        case 2:
            registraRespostaFregues(1, jogador1.nome, jogador2.nome);
            atualizaCorridas(Caderneta, jogador1.nome, jogador2.nome);
            correTruco(t, dupla, placar);
            break;
        case 3:
            if (!podeAumentar(t, dupla)) {
                printf("Nao eh possivel aumentar o truco!\n");
            }
            else {
                registraRespostaFregues(0, jogador1.nome, jogador2.nome);
                aumentaTruco(t, dupla, placar);
                respostaTruco(t, duplaRival(dupla), placar, Jogadores, Caderneta);
            }
            break;
        default:
            printf("Comando invalido, tente novamente\n");
            respostaTruco(t, dupla, placar, Jogadores, Caderneta);
            break;
        }

    }
    else { //Se a partida for passar a valer 12, o dupla não pode aumentar o truco, apenas aceitar ou correr
        printf("Dupla %d, o que voces vao fazer? (A mao passara a valer %d pontos)\nAceitar truco: pressione 1\nCorrer: pressione 2\n", dupla, proximoValor(t->valormao));
        int resp;
        scanf("%d", &resp);
        switch (resp) {
        case 1:
            registraRespostaFregues(0, jogador1.nome, jogador2.nome);
            aceitaTruco(t, dupla);
            break;
        case 2:
            registraRespostaFregues(1, jogador1.nome, jogador2.nome);
            atualizaCorridas(Caderneta, jogador1.nome, jogador2.nome);
            correTruco(t, dupla, placar);
            break;
        default:
            printf("Comando invalido, tente novamente\n");
            respostaTruco(t, dupla, placar, Jogadores, Caderneta);
            break;
        }
    }
}


int pedeTruco(tp_truco* t, int dupla, int placar[], tp_fila Jogadores, ArvAVL* Caderneta) { //Função que registra um pedido de truco/seis/doze. 
    //Retorna 1 se for válido, 0 se não for (e nada é alterado).
    if (!podeTruco(t, dupla)) return 0;

    tp_jogador quemPediu;
    topoFila(&Jogadores, &quemPediu); //a fim de salvar o jogador que pediu o truco para atualizar a caderneta
    t->quemTrucou = quemPediu;

    atualizaTrucoPedido(Caderneta, quemPediu.nome); //atualiza a caderneta

    t->esperandoAceitar = true;
    t->duplapediu = dupla;
    printf("A dupla %d pediu truco!!\n", dupla);
    respostaTruco(t, duplaRival(dupla), placar, &Jogadores, Caderneta); //Pede a resposta da próxima dupla, iniciando o ciclo de truco
    return 1;
}

#endif