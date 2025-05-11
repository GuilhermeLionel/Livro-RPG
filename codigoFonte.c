#include <stdio.h>

void limparTerminal(){
    printf("\033[H\033[J");
}

void cabecaTela(char* x){
    printf("----------------------\n");
    printf("-------- %s --------\n", x);
    printf("----------------------\n");
}

void telaInicial(){
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
