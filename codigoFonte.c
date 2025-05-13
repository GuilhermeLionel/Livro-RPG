#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h> // usleep
#endif

int numAle(int range){
    int n = (rand()%range)+1;
    
    return n;
}
void limparTerminal(){
    printf("\033[H\033[J");
}

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

void cabecaTela(char* x) {
    int largura = 40;
    int textoLen = strlen(x);
    int espacos = largura - textoLen - 2;
    int antes = espacos / 2;
    int depois = espacos - antes;

    for (int i = 0; i < largura; i++) printf("-");
    printf("\n");

    for (int i = 0; i < antes; i++) printf("-");
    printf(" %s ", x);
    for (int i = 0; i < depois; i++) printf("-");
    printf("\n");

    for (int i = 0; i < largura; i++) printf("-");
    printf("\n");
}

void gerarPers(){
    char nome[50] = {0};
    textoTela("Anos no passado, nossos ancestrais viviam tranquilamente...\n", 200);
    textoTela("Quer dizer", 300);
    textoTela(". . .\n", 1000);
    textoTela("No limite, do possivel!", 200);
    cross_platform_sleep(2000);
    fgets(nome, 100, stdin);

    limparTerminal();
    textoTela("Mas tudo mudou com a ascensao de um unico nome...\n", 200);
    cross_platform_sleep(1000);
    textoTela("\033[31mVALDORAN!\033[0m\n", 400);
    cross_platform_sleep(1000);
    textoTela("Um reino forjado em fogo, sangue e gloria.\n", 300);
    textoTela("Erguido entre os Montes Eternos e os Rios dourados\n", 300);
    textoTela("Valdoran cresceu ate se tornar o coracao pulsante do continente.\n", 300);
    printf("\n\n(Pressione ENTER para continuar...)\n");
    fgets(nome, 100, stdin);

    limparTerminal();
    textoTela("Seus exercitos marcharam por desertos escaldantes,\n", 300);
    textoTela("atravessaram selvas malditas e conquistaram fortalezas tidas como impenetraveis.\n", 300);
    textoTela("Reis se ajoelharam. Imperios cairam.\n", 300);
    textoTela("Valdoran nao pedia permissao...\n", 400);
    textoTela("Eles queriam...", 400);
    cross_platform_sleep(1500);
    textoTela("\033[1;31mEles Tomavam...\033[0m", 400);
    fgets(nome, 100, stdin);

    limparTerminal();
    textoTela("A Cidade-Corona, capital do reino, resplandece como um farol de poder e conhecimento.\n", 200);
    textoTela("Torres encantadas tocam os ceus, e seus sabios dominam a propria essencia da magia.\n", 200);
    textoTela("Seus forjadores moldam armas que sussurram lendas a cada golpe.\n", 200);
    textoTela("E no trono de obsidiana, repousa o soberano mais temido do mundo...\n", 300);
    textoTela("...e talvez o mais odiado tambem.\n", 200);
    fgets(nome, 100, stdin);

    limparTerminal();
    textoTela("Lugar incrivel, ne?\n", 300);
    textoTela("Mas . . .\n", 500);
    textoTela("Nossa aventura comeca aqui:\n", 300);
    cross_platform_sleep(1500);
    textoTela("\033[1;32mTAMARELANDIA DO NORTE.\033[0m\n", 300);
    cross_platform_sleep(2000);
    textoTela("Uma aldeia esquecida por todos os mapas decentes.\n", 200);
    fgets(nome, 100, stdin);

    limparTerminal();
    textoTela("Localizada entre o Pantano Cheiroso e a Colina dos Bodes Falantes.\n", 200);
    textoTela("Longe de toda a gloria, da riqueza e da magia de Valdoran,\n", 200);
    textoTela("Aqui a aventura mais comum e desentupir o poco da praca.\n", 200);
    textoTela("E o maior guerreiro da vila? . . . . .\n", 500);
    textoTela("Um velho que jura ter lutado com uma galinha possuida.\n", 300);
    textoTela("Mas todo destino grandioso comeca com passos pequenos.\n", 200);
    textoTela("E hoje . . .\n", 500);
    textoTela("Errrn . . . Hoje! . . .\n", 500);
    textoTela("Desculpe, mas qual seu nome mesmo?\n\n", 500);
    fgets(nome, 50, stdin);

    limparTerminal();
    textoTela("Realmente . . .", 200);
    printf("%s", nome);
    textoTela("Um verdadeiro nome de guerreiro . . .\n\nQuer comprar algo na minha loja?", 400);
}

void telaInicial(){
    limparTerminal();
    cabecaTela("A ascencao na torre!");
    printf("1 - Iniciar um novo jogo.\n");
    printf("2 - Continuar um jogo gravado.\n");
    printf("3 - Ranking de pontos.\n");
    printf("4 - Listar integrantes do grupo.\n");
    printf("5 - Fechar jogo.\n\n");

    int n;
    scanf("%d", &n);

    switch (n) {
        case 1:
            limparTerminal();
            gerarPers();
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
            }
            telaInicial();
            break;
        case 5:
            break;
        default:
            cabecaTela("Resposta invalida. Digite novamente.");
            cross_platform_sleep(1500);
            telaInicial();
            break;
    }
}

int main(){
    srand(time(NULL));
    telaInicial();
    return 0;
}
