/*======================= BIBLIOTECA DO VAR (HISTÓRICO DUPLAMENTE ENCADEADO) =======================*/
#ifndef var_h
#define var_h

#include <stdio.h>
#include <string.h>
#include "tipos.h"
#include "listad.h"

void salvar_rodada(tp_listad *listaVAR, int mao, int numRodada, carta_jogada mesa[4], int vencedor, tp_jogador quemTrucou, tp_jogador quemRetrucou, int corre) {
    tp_salvar item;
    item.mao = mao;
    item.numRodada = numRodada;
    for (int i = 0; i < 4; i++) {
        item.cartas[i] = mesa[i];
    }
    item.vencedor = vencedor;
    item.quemTrucou = quemTrucou;     // Guarda apenas quem fez o ULTIMO pedido (3, 6 ou 12)
    item.quemRetrucou = quemRetrucou; // Guarda apenas quem aceitou ou correu desse ultimo pedido
    item.corre = corre;

    insere_listad_no_fim(listaVAR, item);
}

void imprime_no_VAR(tp_no *no) {
    if (no == NULL) return;
    tp_salvar info = no->info;

    printf("\n========================================\n");
    printf("        VAR - MAO %d | RODADA %d\n", info.mao, info.numRodada);
    printf("========================================\n");
    
    for (int i = 0; i < 4; i++) {
        if (info.cartas[i].cartaJogada.numero != 0) {
            printf("Jogador: %-12s (Dupla %d) -> Jogou: %d%c\n", 
                   info.cartas[i].jogador, 
                   info.cartas[i].dupla, 
                   info.cartas[i].cartaJogada.numero, 
                   info.cartas[i].cartaJogada.naipe);
        } else {
            printf("Jogador: Nao jogou\n");
        }
    }

    printf("----------------------------------------\n");
    if (strlen(info.quemTrucou.nome) > 0 && strcmp(info.quemTrucou.nome, "Ninguem") != 0) {
        printf("Ultimo pedido (Truco/6/12) feito por : %s\n", info.quemTrucou.nome);
        if (info.corre == 1) {
            printf("Quem recusou (correu) o pedido       : %s\n", info.quemRetrucou.nome);
        } else {
            printf("Quem aceitou o pedido                : %s\n", info.quemRetrucou.nome);
        }
    } else {
        printf("Truco: Ninguem pediu truco nesta rodada.\n");
    }

    printf("----------------------------------------\n");
    if (info.corre == 1) {
        printf("Resultado: Correram do truco! Vitoria da Dupla %d!\n", info.vencedor);
    } else if (info.vencedor == 0) {
        printf("Resultado: Rodada Empatada (Cangou)!\n");
    } else {
        printf("Resultado: Vitoria da Dupla %d!\n", info.vencedor);
    }
    printf("========================================\n");
}

void exibir_VAR(tp_listad *listaVAR) {
    if (listad_vazia(listaVAR)) {
        printf("\nO historico do VAR esta vazio!\n");
        return;
    }

    tp_no *atual = listaVAR->inicio;
    int opcao = -1;

    while (1) {
        imprime_no_VAR(atual);

        printf("\nControle do VAR:\n");
        if (atual->prox != NULL) {
            printf("[1] Passar para a proxima rodada\n");
        } else {
            printf("[1] (Fim do VAR - Nao ha proxima rodada)\n");
        }

        if (atual->ant != NULL) {
            printf("[2] Voltar para a rodada anterior\n");
        } else {
            printf("[2] (Primeiro no - Nao tem como voltar)\n");
        }
        printf("[0] Parar de ver o VAR (Sair)\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 0) {
            printf("\nSaindo do VAR...\n");
            break;
        } else if (opcao == 1) {
            if (atual->prox != NULL) {
                atual = atual->prox;
            } else {
                printf("\n>>> Voce ja esta na ultima rodada! Digite 2 para voltar ou 0 para sair. <<<\n");
            }
        } else if (opcao == 2) {
            if (atual->ant != NULL) {
                atual = atual->ant;
            } else {
                printf("\n>>> Voce esta no primeiro no! Nao tem como voltar! <<<\n");
            }
        } else {
            printf("\n>>> Opcao invalida! <<<\n");
        }
    }
}

#endif