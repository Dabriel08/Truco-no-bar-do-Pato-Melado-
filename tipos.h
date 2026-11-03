/*======================= BIBLIOTECA DE TADs =======================*/
#ifndef tipos_h
#define tipos_h

#include <stdbool.h>

#define nCartas 3
#define nJogadores 4

typedef struct {
    //carta é uma struct caracterizada pela numeração da carta (numero) e pelo seu naipe
    int numero;
    char naipe;
} carta;

typedef struct {
    //tp_jogador é uma struct caracterizada pelo nome do jogador (string), sua mão (vetor de carta), por sua dupla (representada por um inteiro) e quantas cartas ainda restam em sua mão
    char nome[50];
    int dupla;
    carta mao[nCartas];
    int cartasRestantes;
} tp_jogador;

typedef struct {
    //carta_jogada é uma struct caracterizada pela carta jogada na mesa (tipo carta), assim como seus respectivos jogador e dupla (inteiros)
    carta cartaJogada;
    char jogador[50];
    int dupla;
} carta_jogada;

typedef struct {
    //tp_truco é uma struct que armazena os resultados da rodada pós-negociação
    int valormao;
    int duplapediu;
    bool esperandoAceitar;
    int maoEncerrada;
    tp_jogador quemTrucou;
    tp_jogador quemRetrucou;
} tp_truco;

typedef struct {
    //tp_salvar é uma struct que armazena os dados utilizados no VAR
    int mao;
    int numRodada;
    carta_jogada cartas[4];
    int vencedor;
    tp_jogador quemTrucou;
    tp_jogador quemRetrucou;
    int corre;
} tp_salvar;

#endif