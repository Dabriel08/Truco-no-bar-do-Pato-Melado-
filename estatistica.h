/*======================= BIBLIOTECA DE ESTATÍSTICAS EXTRAS =======================*/
#ifndef estatisticas_h
#define estatisticas_h
#define MAX_MAOS 30
#include <stdio.h>
#include "tipos.h"
#include "mural.h" 


float taxacorrida(int corridas, int trucorecebidos) {
	// A taxa (em porcentagem) de vezes que o jogador correu do truco quando ele recebeu um pedido de truco/6/12
	// Se o jogador nunca recebeu um pedido de truco, a função retorna -1 para indicar que a taxa não pode ser calculada
	if (trucorecebidos == 0)
		return -1.0f;


	return 100.0f * corridas / trucorecebidos; //100.0f força a divisão em float
}


void exibirtaxascorrida() {
	// Mostra o "mural da vergonha": a taxa de quem "correu do truco" de cada jogador, somando todas as partidas
	tp_mural mural;
	carregarMural(&mural); // lê do disco (mural.txt) os contadores de cada freguês

	printf("\n=========== MURAL DA VERGONHA: taxa de \"correu do truco\" ===========\n");

	if (mural.quantidade == 0) {
		printf("Nenhum jogador registrado.\n");
	}
	else {
		printf("%-20s %9s %9s %8s\n", "Jogador", "Recebidos", "Corridas", "Taxa");

		for (int i = 0; i < mural.quantidade; i++) {
			tp_fregues f = mural.fregueses[i]; // copia do jogador para facilitar a leitura do código
			float taxa = taxacorrida(f.vezesCorridas, f.trucos);

			printf("%-20.20s %9d %9d ", f.nome, f.trucos, f.vezesCorridas); //%-20.20s limita a string a 20 caracteres, evitando que nomes longos baguncem a tabela e alinha os nomes à esquerda

			if (taxa < 0)
				printf("%8s\n", "--"); //nunca recebeu pedido: não há taxa para mostrar
			else
				printf("%7.1f%%\n", taxa);
		}
	}

	printf("=====================================================================\n");
}
void exibirmaomaiscara(int valoresMao[], int duplasMao[], int quantidade) {
	// Mostra qual foi a mão mais cara da partida suando a função do rodadas.h
	int pos = determinaMaoMaisCara(valoresMao, quantidade); // posição da mão mais (-1 se nenhuma foi jogada)

	if (pos == -1) { // sem mãos jogadas 
		printf("\nNenhuma mao foi jogada.\n");
		return;
	}

	printf("\n=========== MAO MAIS CARA DA PARTIDA ===========\n");
	// pos + 1 porque as posições começam em 0, mas as mãos são numeradas a partir de 1
	printf("Mao %d: valeu %d ponto(s) e foi ganha pela dupla %d\n", pos + 1, valoresMao[pos], duplasMao[pos]);
	printf("================================================\n");
}

#endif
