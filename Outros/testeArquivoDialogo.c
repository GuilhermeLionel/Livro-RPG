#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>   // mkdir for Windows
#else
#include <unistd.h> // usleep
#include <sys/stat.h> // mkdir
#endif

void cross_platform_sleep(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000); // ms -> microsegundos
#endif
}

void textoTela(const char *texto, int seg) {
    char menssagem[256];
    strcpy(menssagem, texto);
    char *palavra = strtok(menssagem, " ");
    while (palavra != NULL) {
        printf("%s ", palavra);
        fflush(stdout);
        cross_platform_sleep(seg); // Usando função multiplataforma
        palavra = strtok(NULL, " ");
    }
    printf("\n");
}

void imprimeDialogo(char *nomeArquivo, int min, int max) {
    FILE *arquivo = fopen(nomeArquivo, "rt");
    char texto[401];
    int idNum = 0, duracao;
    while (idNum < max) {
        fscanf(arquivo, "dialogo: ");
        fgets(texto, 400, arquivo);
        fscanf(arquivo, "id: %d\n", &idNum);
        fscanf(arquivo, "duracao: %d\n\n", &duracao);
        if (idNum >= min) 
            textoTela(texto, duracao);
    }
    fclose(arquivo);
}

int main() {
    char nomeArquivo[100]; int min, max;
    strcpy(nomeArquivo, "Dados do Jogo/testeArquivoDialogo.txt");
    min = 1; max = 1;
    // scanf("%s", nomeArquivo);
    // scanf(" %d", &min);
    // scanf(" %d",&max);
    imprimeDialogo(nomeArquivo, min, max);
    return 0;
}