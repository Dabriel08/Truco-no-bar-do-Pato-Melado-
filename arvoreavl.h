#ifndef ARVOREAVL_H
#define ARVOREAVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "tipos.h"

typedef struct NO* ArvAVL;

struct NO { //essa arvore é ordenada pela ordem alfabeta dos nomes, alem de carregar as outras informacoes de cardeneta de cada jogador
	tp_caderneta info; //nessa versao da arvore avl, o tipo da informacao é o tp_caderneta, e nao tp_jogador pois as informacoes do jogador e da caderneta sao diferentes e atualizadas em momentos diferentes, compartilhando apenas o nome, essa separacao visa deixar o codigo mais organizado e facil de manusear 
	int alt; //altura da sub-�rvore (facilita o c�lculo do fator de balanceamento!)
	struct NO* esq;
	struct NO* dir;
};

ArvAVL* criarAVL();
void preOrd(ArvAVL* raiz);
void emOrd(ArvAVL* raiz, int contPrint);
void posOrd(ArvAVL* raiz);
int inserir(ArvAVL* raiz, char Jogador[]);
//int remover(ArvAVL* raiz, tp_caderneta dadosJog); // Respons�vel pela busca do n� a ser removido
struct NO* buscarMenor(struct NO* atual); // Busca o n� mais a esquerda de uma �rvore
int consultarValorAVL(ArvAVL* raiz, char Jogador[]);
int maior(int x, int y);
int alt_NO(struct NO* no);
int fb_NO(struct NO* no);
void RotacaoLL(ArvAVL* raiz);
void RotacaoRR(ArvAVL* raiz);
void RotacaoRL(ArvAVL* raiz);
void RotacaoLR(ArvAVL* raiz);

ArvAVL* criarAVL() {
	ArvAVL* raiz = (ArvAVL*)malloc(sizeof(ArvAVL));
	if (raiz != NULL) {
		*raiz = NULL;
	}
	return raiz;
}

void printaNO(struct NO* noEscolhido) {
	printf("%s\n%d partidas jogadas\n", noEscolhido->info.nome, noEscolhido->info.partidasJogadas);
	printf("%d maos ganhas\n%d trucos pedidos\n", noEscolhido->info.maosGanhas, noEscolhido->info.trucosPedidos);
	printf("Correu %d vezes\n", noEscolhido->info.quantasCorridas);

}

void printSave(struct NO* noAtual, FILE* arquivo) { //funcao de salvar um no no disco
	fprintf(arquivo, "%s\t%d\t%d\t%d\t%d\n", noAtual->info.nome, noAtual->info.partidasJogadas, noAtual->info.maosGanhas, noAtual->info.trucosPedidos, noAtual->info.quantasCorridas);

}

void percorretd(ArvAVL* Caderneta, FILE* arquivo) { //funcao que percorre td a arvore e vai salvando no disco
	if (Caderneta == NULL) return;
	if (*Caderneta != NULL) {
		percorretd(&((*Caderneta)->esq), arquivo);
		printSave(*Caderneta, arquivo);
		percorretd(&((*Caderneta)->dir), arquivo);
	}
}

void emOrd(ArvAVL* raiz, int contPrint) {
	if (raiz == NULL) return;
	if (*raiz != NULL) {
		//printf("Contador:%d", contPrint);
		emOrd(&((*raiz)->esq), contPrint);
		if (contPrint != 0) {
			printf("\n\n");
		}
		printaNO(*raiz);
		contPrint++;
		emOrd(&((*raiz)->dir), contPrint);
	}
}

int alturaAVL(ArvAVL* raiz) {
	if (raiz == NULL) return 0;
	if (*raiz == NULL) return 0;
	int alt_esq = alturaAVL(&((*raiz)->esq));
	int alt_dir = alturaAVL(&((*raiz)->dir));
	if (alt_esq > alt_dir) {
		return alt_esq + 1;
	}
	else {
		return alt_dir + 1;
	}
}

int totalNOsABB(ArvAVL* raiz) {
	if (raiz == NULL) return 0;
	if (*raiz == NULL) return 0;
	int tot_esq = totalNOsABB(&((*raiz)->esq));
	int tot_dir = totalNOsABB(&((*raiz)->dir));
	return (tot_esq + tot_dir + 1);
}

int estah_vaziaABB(ArvAVL* raiz) {
	if (raiz == NULL) return 1;
	if (*raiz == NULL) return 1;
	return 0;
}

int inserir(ArvAVL* raiz, char Jogador[]) {
	int res;
	if (*raiz == NULL) {
		struct NO* novo;
		novo = (struct NO*)malloc(sizeof(struct NO));
		if (novo == NULL) return 0;
		strcpy(novo->info.nome, Jogador); //copia o nome recebido para a string do no
		novo->info.partidasJogadas = 0; //como é um jogador novo, todas as variaveis comecam zeradas
		novo->info.maosGanhas = 0;
		novo->info.trucosPedidos = 0;
		novo->info.quantasCorridas = 0;
		novo->alt = 0;
		novo->dir = NULL;
		novo->esq = NULL;
		*raiz = novo;
		return 1;
	}
	else {
		struct NO* atual = *raiz;
		int comparacao = strcmp(Jogador, atual->info.nome);
		if (comparacao < 0) { //significa que o nome a ser inserido vem antes na ordem alfabetica do que o no atual
			if ((res = inserir(&(atual->esq), Jogador)) == 1) {
				if (fb_NO(atual) >= 2) {
					comparacao = strcmp(Jogador, (*raiz)->esq->info.nome);
					if (comparacao < 0) {//significa que o nome a ser inserido vem antes na ordem alfabetica do que o no atual
						RotacaoLL(raiz);
					}
					else {
						RotacaoLR(raiz);
					}
				}
			}
		}
		else {
			if (comparacao > 0) {//significa que o nome a ser inserido vem depois na ordem alfabetica do que o no atual
				if ((res = inserir(&(atual->dir), Jogador)) == 1) {
					if (fb_NO(atual) >= 2) {
						comparacao = strcmp(Jogador, (*raiz)->dir->info.nome);
						if (comparacao > 0) {//significa que o nome a ser inserido vem depois na ordem alfabetica do que o no atual
							RotacaoRR(raiz);
						}
						else {
							RotacaoRL(raiz);
						}
					}
				}
			}
			else {
				return 0; // Valor Duplicado
			}
		}
		atual->alt = maior(alt_NO(atual->esq), alt_NO(atual->dir)) + 1;
		return res;
	}
}

void liberaNO(struct NO* no) {
	if (no == NULL) return;
	liberaNO(no->esq);
	liberaNO(no->dir);
	free(no);
	no = NULL;
}

void liberaABB(ArvAVL* raiz) {
	if (raiz == NULL) return;
	liberaNO(*raiz);
	free(raiz);
}

int consultarValorAVL(ArvAVL* raiz, char Jogador[]) { //1 se o jogador ja existe, 0 se n
	if (raiz == NULL) return 0;
	if (*raiz == NULL) return 0;
	struct NO* atual = *raiz;
	while (atual != NULL) {
		int comparacao = strcmp(Jogador, atual->info.nome);
		if (comparacao == 0) {
			return 1;
		}
		else {
			if (comparacao < 0) {
				atual = atual->esq;
			}
			else {
				atual = atual->dir;
			}
		}
	}
	return 0;
}

struct NO* retornaNO(ArvAVL* raiz, char Jogador[]) { //"recupera" endereco de memoria de um no para que as infomrcaoes possam ser editadas
	if (raiz == NULL) return NULL;
	if (*raiz == NULL) return NULL;
	struct NO* atual = *raiz;
	while (atual != NULL) {
		int comparacao = strcmp(Jogador, atual->info.nome);
		if (comparacao == 0) {
			return atual;
		}
		else {
			if (comparacao < 0) {
				atual = atual->esq;
			}
			else {
				atual = atual->dir;
			}
		}
	}
	return NULL;
}

int alt_NO(struct NO* no) {
	if (no == NULL) return -1;
	else return no->alt;
}

int fb_NO(struct NO* no) {
	int esq = alt_NO(no->esq);
	int dir = alt_NO(no->dir);
	return labs(esq - dir);
}

int maior(int x, int y) {
	return (x > y) ? x : y;
}

void RotacaoLL(ArvAVL* raiz) {
	struct NO* no;
	no = (*raiz)->esq;
	(*raiz)->esq = no->dir;
	no->dir = *raiz;
	(*raiz)->alt = maior(alt_NO((*raiz)->esq), alt_NO((*raiz)->dir)) + 1;
	no->alt = maior(alt_NO(no->esq), (*raiz)->alt) + 1;
	*raiz = no;
}

void RotacaoRR(ArvAVL* raiz) {
	struct NO* no;
	no = (*raiz)->dir;
	(*raiz)->dir = no->esq;
	no->esq = *raiz;
	(*raiz)->alt = maior(alt_NO((*raiz)->esq), alt_NO((*raiz)->dir)) + 1;
	no->alt = maior(alt_NO(no->dir), (*raiz)->alt) + 1;
	*raiz = no;
}

void RotacaoLR(ArvAVL* raiz) {
	RotacaoRR(&(*raiz)->esq);
	RotacaoLL(raiz);
}

void RotacaoRL(ArvAVL* raiz) {
	RotacaoLL(&(*raiz)->dir);
	RotacaoRR(raiz);
}

struct NO* buscarMenor(struct NO* atual) {
	struct NO* no1 = atual;
	struct NO* no2 = atual->esq;
	while (no2 != NULL) {
		no1 = no2;
		no2 = no1->esq;
	}
	return no1;
}

#endif