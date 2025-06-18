#include <stdio.h>
#include <string.h>

typedef struct _RANKING {
    char nome[41];
    int level;
    int pontuacao;
} RANKING;

RANKING player;

// criar struct _RANKING

void atualizaRanking(char *nomeArquivo, RANKING player) {
    RANKING playerRanking[8]; // vetor de structs dos jogadores do ranking atual
    FILE *ranking = fopen(nomeArquivo, "r"), *novoRanking = fopen("novoRanking_teste.txt", "w");
    int posicao, posicaoPlayer, colocacao, i = 0, OK = 1; // OK - verifica se o player já foi inserido no ranking
    while(!feof(ranking)) {
        fscanf(ranking, "%d - %s - lvl: %d - %d\n", &posicao, playerRanking[i].nome, &playerRanking[i].level, &playerRanking[i].pontuacao); // lê os players do ranking
        if (playerRanking[i].pontuacao > player.pontuacao) 
            fprintf(novoRanking, "%d - %s - lvl: %d - %d\n", posicao, playerRanking[i].nome, playerRanking[i].level, playerRanking[i].pontuacao); // imprime os jogadores à frente do player
        else { 
            if (OK) {
                posicaoPlayer = i;
                fprintf(novoRanking, "%d - %s - lvl: %d - %d\n", posicao, player.nome, player.level, player.pontuacao); // imprime o player na sua colocação
                OK = 0; // player está no ranking
            }
        }
        i++;
    } 
    for (i = posicaoPlayer, colocacao = posicaoPlayer + 2; colocacao <= 8; i++, colocacao++) // imprime os players restantes, começando na próxima colocação e terminando na 8ª
    // i = posicaoPlayer: pega o índice para o playerRanking
        fprintf(novoRanking, "%d - %s - lvl: %d - %d\n", colocacao, playerRanking[i].nome, playerRanking[i].level, playerRanking[i].pontuacao);
    fclose(ranking);
    fclose(novoRanking);
}

int main () {
    strcpy(player.nome, "Goku");
    player.level = 17;
    player.pontuacao = 9800;
    atualizaRanking("pontuacao_copy.txt", player);
    return 0;
}