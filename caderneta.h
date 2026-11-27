/*======================= BIBLIOTECA DE DETERMINAÇÃO DA CADERNETA =======================*/
#ifndef caderneta_h
#define caderneta_h

#include <stdio.h>
#include <string.h>
#include "tipos.h"
#include "arvoreavl.h"
#include "fila.h"


#define arquivoCaderneta "caderneta.txt"

void carregarCaderneta(ArvAVL* Caderneta) {
    FILE* arquivo = fopen(arquivoCaderneta, "r"); //abre o arquivo e le os dados existentes

    if (arquivo == NULL) { //se nao tiver conseguido abrir o arquivo
        return;
    }

    tp_caderneta aux;

    while (fscanf(arquivo, " %[^\t]\t%d\t%d\t%d\t%d", aux.nome, &aux.partidasJogadas, &aux.maosGanhas, &aux.trucosPedidos, &aux.quantasCorridas) == 5) {
        //a condicional significa que foi possivel acessar as 5 informacoes referentes a caderneta
        inserir(Caderneta, aux.nome);//insere o jogador salva na arvore, remontando a com as informacoes salvas

        struct NO* noAtual;
        noAtual = retornaNO(Caderneta, aux.nome); //busca a pessoa que esta sendo reinserida na arvore a fim de colocar suas informacoes novamenta

        if (noAtual != NULL) {
            noAtual->info = aux; // restaura as informacoes salvas no disco(aux) para o no da arvore que esta sendo remontada
        }
    }

    fclose(arquivo);
}

void salvarCaderneta(ArvAVL* Caderneta) {
    FILE* arquivo = fopen(arquivoCaderneta, "w"); //abre o arquivo e le os dados existentes

    if (arquivo == NULL) { //se nao tiver conseguido abrir o arquivo
        printf("Erro ao salvar a caderneta!\n");
        return;
    }

    percorretd(Caderneta, arquivo); //percorre todos os nos da arvore/ caderneta, salvando-os no disco

    fclose(arquivo);
}

void acoesCaderdeta(ArvAVL Caderneta) { //contem as acoes possiveis dentro da caderneta
    printf("O que voce deseja fazer?\n1 - Buscar jogador\n2 - Mostrar caderneta completa\n");
    char resp;
    scanf(" %c", &resp);
    if (resp == '1') {
        printf("Digite o nome do jogador (cuidado com letras maisculas e minusculas!): ");
        char nomeBusca[100];
        scanf(" %s", nomeBusca);
        struct NO* pessoaBusca; //no auxiliar para acessar as informacoes de uma pessoa especifica
        pessoaBusca = retornaNO(&Caderneta, nomeBusca);//busca o NO referente à pessoa buscada na arvore/caderneta a partir do nome(string digitada)
        if (pessoaBusca == NULL) { //retonaNO retorna NULL caso o nome nao esteja presente na arvore/caderneta
            printf("Pessoa nao encontrada.\n");
        }
        else {
            printaNO(pessoaBusca);//se a pessoa existir na arvore, o programa printa suas informacoes
        }

    }
    else if (resp == '2') {
        int contPrint = 0;
        emOrd(&Caderneta, contPrint); //exibe a caderneta completa em ordem alfabetica (arvores de busca quando exibidas em ordem infixa exibem os elementos em ordem crescente)
    }
    else if (resp != '1' && resp != '2') {
        printf("Digite um comando valido\n");
        acoesCaderdeta(Caderneta);
        return;
    }
}

void abrirCaderneta(ArvAVL Caderneta) { //funcao que permite varios acessos na caderneta, criando um loop com acoes caderneta
    printf("\n===============================\nDeseja acessar a caderneta?\n1 - Sim\n0 - Nao\n");
    char comando;
    scanf(" %c", &comando);
    if (comando != '0' && comando != '1') {
        printf("Digite um comando valido\n");
        abrirCaderneta(Caderneta);
        return;
    }
    while (comando == '1') { //esse loop permite que varias consultas sejam feitas na caderneta, ate que seja digitado 0
        acoesCaderdeta(Caderneta); //chama a funcao com todas as opcoes da caderneta
        printf("Continuar navegando na caderneta?\n1 - Sim\n0 - Nao\n");//o 0 fecha a caderneta, encerrando o loop, enquando o 1 permite uma outra chamada da funcao acoesCaderneta, iniciando uma nova consulta
        scanf(" %c", &comando);
        if (comando != '0' && comando != '1') {
            printf("Digite um comando valido\n");
            abrirCaderneta(Caderneta);
            return;
        }
    }
    printf("\nCaderneta guardada!\n=================================\n");
}

void perguntaNomeJogador(ArvAVL* Caderneta, char Nome[], int num) { //essa funcao serve para verificar se esse nome ja existe na caderneta, evitando que novos jogadores escolham nomes iguais e que salvem suas informcaoes de forma inadequada
    printf("Insira o nome do jogador %d: ", num);
    scanf(" %[^\n]", Nome);
    if (consultarValorAVL(Caderneta, Nome)) { //o nome ja foi cadastrado
        printf("\nEsse nome ja esta cadastrado!\n 1 - Sou eu\n0 - Escolher outro nome\n");
        int resp;
        scanf("%d", &resp);
        if (resp == 0) {
            perguntaNomeJogador(Caderneta, Nome, num); //a funcao entra em loop ate que um nome valido seja escolhido
        }
        else if (resp != 1) {
            printf("Digite um comando valido\n");
            perguntaNomeJogador(Caderneta, Nome, num);
        }
    }
    else { //nome nao cadastrado
        printf("\nNovo no Bar do Pato! Bem-vindo!!\n");
        inserir(Caderneta, Nome); //insere o nome caso ele ainda nao exista na caderneta
    }
    //observacao: caso o nome ja exista e a pessoa confirme que é ela, a insercao nao ocorre

    salvarCaderneta(Caderneta); //salva as alteracoes no disco
}
//observacao: perguntaNomeJogador tambem insere o nome valido na arvore/caderneta

void atualizaPartidasJogadas(ArvAVL* Caderneta, tp_fila* Jogadores) {
    for (int i = 0; i < nJogadores; i++) { //loop para percorrer todos os jogadores da partida atual
        tp_jogador JogAtual;
        topoFila(Jogadores, &JogAtual); //acessa o jogador na fila
        struct NO* cadernetaAtual;
        cadernetaAtual = retornaNO(Caderneta, JogAtual.nome); //busca o jogador atual na caderneta, retornando um ponteiro para seu no
        if (cadernetaAtual != NULL) {
            (cadernetaAtual->info.partidasJogadas)++; //adiciona um nas partidas jogadas
        }
        prox_jogador(Jogadores); //anda a fila, permitindo que a funcao passe pelos outros jogadores
    }
    salvarCaderneta(Caderneta); //salva as alteracoes no disco
}

void atualizaMaosGanhas(ArvAVL* Caderneta, tp_fila* Jogadores, int duplaVenc) {
    for (int i = 0; i < nJogadores; i++) { //loop para percorrer todos os jogadores da partida atual
        tp_jogador JogAtual;
        topoFila(Jogadores, &JogAtual);
        if (JogAtual.dupla == duplaVenc) { //verifica se o jogador atual é da dupla vencedora, caso contrario, nao adiciona nada no contador de maos ganhas
            struct NO* cadernetaAtual;
            cadernetaAtual = retornaNO(Caderneta, JogAtual.nome); //busca o jogador atual na caderneta, retornando um ponteiro para seu no
            if (cadernetaAtual != NULL) {
                (cadernetaAtual->info.maosGanhas)++;//adiciona um nas maos ganhas
            }
        }
        prox_jogador(Jogadores); //anda a fila, permitindo que a funcao passe pelos outros jogadores
    }
    salvarCaderneta(Caderneta); //salva as alteracoes no disco
}

void atualizaTrucoPedido(ArvAVL* Caderneta, char Nome[]) { //o nome é a pessoa que pediu o truco
    struct NO* cadernetaAtual;
    cadernetaAtual = retornaNO(Caderneta, Nome); //busca a pessoa que pediu o truco na caderneta, retornando um ponteiro para o no na caderneta
    if (cadernetaAtual != NULL) {
        cadernetaAtual->info.trucosPedidos++; //incrementa um no truco pedido
    }
    salvarCaderneta(Caderneta); //salva as alteracoes no disco
}
//obs: essa funcao é chamada dentro de pedetruco, entao, adiciona ao contador apenas da pessoa que inicia o pedido de truco

void atualizaCorridas(ArvAVL* Caderneta, char Jog1[], char Jog2[]) {
    struct NO* cader1, * cader2;
    cader1 = retornaNO(Caderneta, Jog1); //ponteiro do no do primeiro jogador da dupla
    cader2 = retornaNO(Caderneta, Jog2); //ponteiro do no do segundo jogador da dupla

    if (cader1 != NULL) {
        cader1->info.quantasCorridas++;
    }

    if (cader2 != NULL) {
        cader2->info.quantasCorridas++;
    }
    salvarCaderneta(Caderneta); //salva as alteracoes no disco
}
#endif