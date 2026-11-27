/*======================= BIBLIOTECA DE MURAL DA PARTIDA =======================*/
#ifndef mural_h
#define mural_h
#include "tipos.h"
#include "fila.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define arquivoMural "mural.txt"

/*OBS PRA ANDREZINGAMEPLAYS (apague depois): o mural precisa de:
1- placar da partida;
2- quantas mãos cada jogador ganhou (já ordenei do maior pro menor)
3- taxa de quantas vezes correu do truco (mural da vergonha)
4- mão mais cara da partida
5- implementar o mural na main

OBS 2: eu não entendi oq Soussa queria com "a mão mais cara da partida que acabou de terminar" (se era os pontos ou as cartas jogadas), então
       fiz uma função que retorna a pontuação da mão mais cara. amanda: acho q era isso msm kkkkkkk
*/


void carregarMural(tp_mural* mural) {
    //Função que carrega os registros existentes no arquivo txt (em disco):
    FILE* arquivo = fopen(arquivoMural, "r"); //abro o arquivo txt com o ponteiro arquivo e leio os dados existentes

    mural->quantidade = 0; //inicializando os fregueses

    if (arquivo == NULL) //Se houver erro na abertura, retorna
        return;

    //Enquanto eu tiver espaço no vetor de fregueses & houver leitura de dados sem erro ("==4"):
    while ((mural->quantidade < MAX_FREGUESES) && (fscanf(arquivo, " %99[^\t]\t%d\t%d\t%d",
        mural->fregueses[mural->quantidade].nome,
        &mural->fregueses[mural->quantidade].maosGanhas,
        &mural->fregueses[mural->quantidade].trucos,
        &mural->fregueses[mural->quantidade].vezesCorridas) == 4)) {
        mural->quantidade++; //Acrescento um freguês ao mural
    }

    fclose(arquivo);
}

void salvarMural(tp_mural mural) {
    //Função que salva as informações registradas para que não sejam perdidas (passagem por cópia):

    FILE* arquivo = fopen(arquivoMural, "w"); //abro o arquivo txt com o ponteiro arquivo para escrita de informações

    if (arquivo == NULL) { //Se houver erro na abertura:
        printf("Erro ao salvar os dados!\n");
        return;
    }

    for (int i = 0; i < mural.quantidade; i++) { //Percorrendo todos os fregueses cadastrados:
        //Escrevendo o nome, o n° de mãos ganhas, de trucos recebidos e de quantas vezes correu do truco (para cada freguês)
        fprintf(arquivo, "%s\t%d\t%d\t%d\n",
            mural.fregueses[i].nome,
            mural.fregueses[i].maosGanhas,
            mural.fregueses[i].trucos,
            mural.fregueses[i].vezesCorridas);
    }

    fclose(arquivo);
}

int buscarFregues(tp_mural* mural, char nome[]) {
    //Função que procura um freguês pelo seu nome e retorna sua posição no array de fregueses (-1 se não encontrar):
    for (int i = 0; i < mural->quantidade; i++) {
        if (strcmp(mural->fregueses[i].nome, nome) == 0) //strcmp: se o nome buscado é igual ao atual, retorna a posição
            return i;
    }

    return -1;
}

void cadastrarFregues(tp_mural* mural, char nome[]) {
    //Função que cadastra um novo freguês, inicializando seus dados:

    if (buscarFregues(mural, nome) != -1) //Se já existir, retorna
        return;

    if (mural->quantidade >= MAX_FREGUESES) //Se não houver espaço para adicionar, retorna erro
        return;

    int i = mural->quantidade;

    //inicializando seus dados:
    strcpy(mural->fregueses[i].nome, nome);
    mural->fregueses[i].maosGanhas = 0;
    mural->fregueses[i].trucos = 0;
    mural->fregueses[i].vezesCorridas = 0;

    mural->quantidade++;
}

void cadastrarJogadoresMural(tp_fila* jogadores) {
    //Função que registra os 4 jogadores da partida atual:

    tp_mural mural;
    carregarMural(&mural); //Declarando e carregando os dados já existentes

    tp_fila copia = *jogadores; //fila cópia da fila de jogadores original
    tp_jogador atual; //tp_jogador auxiliar

    for (int i = 0; i < nJogadores; i++) { //Enquanto houver jogadores não cadastrados no arquivo, remova um jogador da fila e cadastre-o
        removeFila(&copia, &atual);
        cadastrarFregues(&mural, atual.nome);
    }

    salvarMural(mural);
}

void registrarMaoGanha(char nome[100]) {
    //Função que registra que determinado jogador ganhou uma mão de truco, aumentando seu contador maosGanhas:

    tp_mural mural;
    carregarMural(&mural); //Declarando e carregando os dados já existentes

    cadastrarFregues(&mural, nome); //Garante que o jogador já está cadastrado no mural

    int posicao = buscarFregues(&mural, nome); //Encontrando o jogador no vetor de fregueses

    if (posicao != -1) //Se o jogador existir, aumento seu n° de mãos ganhas
        mural.fregueses[posicao].maosGanhas++;

    salvarMural(mural); //Salvo a alteração
}

void registrarRespostaTruco(char nome[], int correu) {
    //Função que registra quando um jogador recebe um pedido de truco e verifica se ele correu ou aceitou
    tp_mural mural;
    carregarMural(&mural); //Declarando e carregando os dados já existentes

    cadastrarFregues(&mural, nome); //Garante que o jogador já está cadastrado no mural

    int posicao = buscarFregues(&mural, nome);

    if (posicao != -1) { //Se o jogador existir, aumento seu n° de trucos recebidos
        mural.fregueses[posicao].trucos++;

        if (correu) //Se o jogador correu, aumento seu n° de vezes que fugiu do truco
            mural.fregueses[posicao].vezesCorridas++;
    }

    salvarMural(mural); //Salvo a alteração
}

void ordernarMural(tp_mural* mural) {
    //Função que ordena os fregueses por mãos ganhas do maior para o menor
    //INSERTION SORT (em ordem decrescente): esse algoritmo é de fácil implementação e atende bem a um pequeno número de valores num array

    for (int i = 1; i < mural->quantidade; i++) {
        tp_fregues chave = mural->fregueses[i];
        int j = i - 1;

        while (j >= 0 && mural->fregueses[j].maosGanhas < chave.maosGanhas) {
            mural->fregueses[j + 1] = mural->fregueses[j];
            j--;
        }

        mural->fregueses[j + 1] = chave;
    }
}

#endif