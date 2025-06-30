#include <stdio.h>
#include <string.h>

typedef struct _RANKING {
    char nome[41];
    int level;
    int pontuacao;
} RANKING;

RANKING player;

void atualizaRanking(char *nomeArquivo, RANKING player) {
    RANKING playerRanking[8]; // vetor de structs dos jogadores do ranking atual
    FILE *ranking = fopen(nomeArquivo, "r+");
    int posicao, i = 0, j;
    while(!feof(ranking)) {
        fscanf(ranking, "%d - %s - lvl: %d - %d", &posicao, playerRanking[i].nome, &playerRanking[i].level, &playerRanking[i].pontuacao); // lê os players do ranking
        i++;    
    }
    for (i = 0; i < 8; i++) 
        if (player.pontuacao > playerRanking[i].pontuacao) { // reordenação do vetor de players
            for (j = 7; j > i; j--) 
                playerRanking[j] = playerRanking[j - 1];
            playerRanking[i] = player;
            break;
        }
    rewind(ranking);
    for (i = 0; i < 8; i++) { // imprime os players no arquivo
        fprintf(ranking, "%d - %s - lvl: %d - %d\n", i + 1, playerRanking[i].nome, playerRanking[i].level, playerRanking[i].pontuacao);
    }
    fclose(ranking);
}

int main () {
    strcpy(player.nome, "Otis");
    player.level = 9;
    player.pontuacao = 4500;
    atualizaRanking("pontuacao_copy.txt", player);
    return 0;
}

/*

void atualizaRanking(char *nomeArquivo, RANKING player) { 
    RANKING playerRanking[8]; // vetor de structs dos jogadores do ranking atual
    FILE *ranking = fopen(nomeArquivo, "r"), *novoRanking = fopen("novoRanking.txt", "w");
    int posicao, i = 0, j;
    while(!feof(ranking)) {
        fscanf(ranking, "%d - %s - lvl: %d - %d\n", &posicao, playerRanking[i].nome, &playerRanking[i].level, &playerRanking[i].pontuacao); // lê os players do ranking
        i++;    
    }
    for (i = 0; i < 8; i++) 
        if (player.pontuacao > playerRanking[i].pontuacao) {
            for (j = 7; j > i; j--) 
                playerRanking[j] = playerRanking[j - 1];
            playerRanking[i] = player;
            break;
        }
    for (i = 0; i < 8; i++) // imprime os players no arquivo
        fprintf(novoRanking, "%d - %s - lvl: %d - %d\n", i + 1, playerRanking[i].nome, playerRanking[i].level, playerRanking[i].pontuacao);c
    fclose(ranking);
    fclose(novoRanking);
}
*/