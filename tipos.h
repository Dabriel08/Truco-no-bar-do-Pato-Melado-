/*======================= BIBLIOTECA DE TADs =======================*/
#ifndef tipos_h
#define tipos_h

#include <stdbool.h>

#define nCartas 3
#define nJogadores 4
#define MAX_FREGUESES 100

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
    int quemRetrucou;
} tp_truco;

typedef struct {
    //tp_salvar é uma struct que armazena os dados utilizados no VAR
    int mao;
    int numRodada;
    carta_jogada cartas[4];
    int vencedor;
    tp_jogador quemTrucou;
    int quemRetrucou; // deve ser a dupla que retrucou, pois qm corre ou aceita é a dupla
    int corre;
} tp_salvar;

typedef struct {
    //tp_caderneta armazena os dados utilizados na caderneta
    char nome[50];
    int partidasJogadas;
    int maosGanhas;
    int trucosPedidos;
    int quantasCorridas;

} tp_caderneta;

typedef struct {
    //tp_fregues é uma struct caracterizada pelo nome do jogador, seu n° de mãos ganhas, de trucos recebidos e de quantas vezes correu do truco
    char nome[100];
    int maosGanhas;
    int trucos;
    int vezesCorridas;
} tp_fregues;

typedef struct {
    //tp_mural é uma struct caracterizada por um array de fregueses (armazena cada freguês já cadastrado) e o n° de fregueses existentes
    tp_fregues fregueses[MAX_FREGUESES];
    int quantidade;
} tp_mural;

#endif