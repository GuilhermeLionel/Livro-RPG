#include <stdio.h>
#include <string.h>

void limparTerminal(){
    printf("\033[H\033[J");
}

void cabecaTela(char* x) {
    int largura = 30; // Largura total da moldura
    int textoLen = strlen(x);
    int espacos = largura - textoLen - 2; // -2 por causa dos espaços antes e depois do texto
    int antes = espacos / 2;
    int depois = espacos - antes;

    // Linha superior
    for (int i = 0; i < largura; i++) printf("-");
    printf("\n");

    // Linha do texto centralizado
    for (int i = 0; i < antes; i++) printf("-");
    printf(" %s ", x);
    for (int i = 0; i < depois; i++) printf("-");
    printf("\n");

    // Linha inferior
    for (int i = 0; i < largura; i++) printf("-");
    printf("\n");
}

void telaInicial(){
    limparTerminal();
    cabecaTela("jogo");
    printf("1 - Iniciar um novo jogo.\n");
    printf("2 - Continuar um jogo gravado.\n");
    printf("3 - Ranking de pontos.\n");
    printf("4 - Listar integrantes do grupo.\n\n");

    //exibição da tela inicial.

    int n;
   scanf("%d", &n);

    switch (n)
    {
    case 1:
        limparTerminal();
        printf("teste");
        break;
    
    case 2:
        break;

    case 3:
        limparTerminal();
        cabecaTela("Ranking");
        break;

    case 4:
        limparTerminal();
        cabecaTela("Interantes");
        printf("\n\033[31mGuilherme Lionel de Souza\033[0m\n");
        printf("\n\033[34mAlexandre Gabriel Angelo de Souza Blandino\033[0m\n");
        printf("\n\033[32mSamuel Pereira da Silva\n\033[0m\n");

        printf("[1] - Voltar para a tela inicial.\n");

        while (1)
        {
            scanf("%d", &n);
            if (n==1)
            {
                break;
            }
        }
        telaInicial();
        
        
        break;

    default:
        printf("Resposta invalida. Digite novamente.\n");
        printf("----------------------\n");
        telaInicial();

        break;
    }
}

int main(){
    telaInicial();
    
    return 0;
}
