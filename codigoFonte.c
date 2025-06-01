#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>   // mkdir for Windows
#else
#include <unistd.h> // usleep
#include <sys/stat.h> // mkdir
#endif


typedef struct itemHandler2{
    char nome[50];
    int tipo;
    int preco;
    int raridade;
    int bonus;
    int quantBonus;
} ITEMHANDLER;

typedef struct dados{
    char nome[50];
    int protecao;
    int vida_max;
    int hp;
    int mp;
    int mana_max;
    int forca;
    int agilidade;
    int inteligencia;
    int carisma;
    int inventario[20];
} DADOS;

DADOS player;

void cross_platform_sleep(int ms);
void limparTerminal();
void gerarPasta();
void aleatJogador(char *txt);
void setItens(int q);
void readItems();
void save(DADOS player);
void load(DADOS *player);
void textoTela(const char *texto, int seg);
void cabecaTela(char* x);

void setItens(int q){
    ITEMHANDLER item[q*6];
    FILE *arq = fopen("Dados do Jogo/items.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    for (int i = 0; i < q; i++) {
        fgets(item[i*6].nome, sizeof(item[i*6].nome), arq);
        item[i*6].nome[strcspn(item[i*6].nome, "\n")] = 0; // Remove newline character
        fscanf(arq, "%d\n%d\n%d\n%d\n%d\n", 
               &item[i*6].tipo, 
               &item[i*6].preco, 
               &item[i*6].raridade, 
               &item[i*6].bonus, 
               &item[i*6].quantBonus);
        cross_platform_sleep(3000);
        limparTerminal();
    }
    fclose(arq);
}

void readItems(){
    int count = 0;
    FILE *arq = fopen("Dados do Jogo/items.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    char linha[256];
    while (fgets(linha, sizeof(linha), arq) != NULL) {
        count++;
    }
    fclose(arq);
    count = count / 6;
    setItens(count);
}


void gerarPasta() {
    #ifdef _WIN32
        _mkdir("Dados do Jogo");
    #else
        mkdir("Dados do Jogo", 0777);
    #endif
    FILE *arq = fopen("Dados do Jogo/save.txt", "w");
    if (arq == NULL) {
        printf("Erro ao criar o arquivo.\n");
    }
    fclose(arq);
}

void save(DADOS player)
{
    FILE *arq = fopen("Dados do Jogo/save.txt", "w");
    if (arq == NULL) {
        printf("Erro ao salvar.\n");
        return;
    }

    char nomeLimpo[50];
    strcpy(nomeLimpo, player.nome);
    char* newline = strchr(nomeLimpo, '\n');
    if (newline) *newline = '\0';

    fprintf(arq, "%s\n", nomeLimpo);
    // CORRIGIDO: 9 especificadores %d para 9 variáveis
    fprintf(arq, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
            player.vida_max, player.hp, player.mana_max, player.mp,
            player.protecao, player.forca, player.agilidade,
            player.inteligencia, player.carisma);

    for (int i = 0; i < 20; i++) {
        fprintf(arq, "%d\n", player.inventario[i]);
    }

    fclose(arq);
}


void load(DADOS *player){
    FILE *arq = fopen("Dados do Jogo/save.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fgets(player->nome, sizeof(player->nome), arq);
    player->nome[strcspn(player->nome, "\n")] = 0;

    // CORRIGIDO: 9 especificadores %d para 9 variáveis
    fscanf(arq, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
           &player->vida_max, &player->hp, &player->mana_max, &player->mp,
           &player->protecao, &player->forca, &player->agilidade,
           &player->inteligencia, &player->carisma);

    for (int i = 0; i < 20; i++) {
        fscanf(arq, "%d\n", &player->inventario[i]);
    }

    fclose(arq);
}


int numAle(int range){
    //Gera um numero de 1 a range.
    int n = (rand()%range)+1;
    
    return n;
}

void aleatJogador(char *txt){
    gerarPasta();
    limparTerminal();

    //no total são 30 pontos de status, 20 RNG e 10 fixo
    int status[5] = {2, 2, 2, 2, 2};
    for (int i = 0 ; i < 20 ; i++)
    {
        status[rand()%5]++;
    }
    
    txt[strcspn(txt, "\n")] = 0; // Tira o caractere "\n" da string txt e coloca "\0"

    strcpy(player.nome, txt);
    player.protecao = status[0];
    player.forca = status[1];
    player.agilidade = status[2];
    player.inteligencia = status[3];
    player.carisma = status[4];
    player.vida_max = player.protecao * 5;
    player.hp = player.vida_max;
    player.mana_max = player.inteligencia * 5;
    player.mp = player.mana_max;

    printf("Nome: %s hello\n", player.nome);
    printf("Protecao: %d\n", player.protecao);
    printf("Forca: %d\n", player.forca);
    printf("Agilidade: %d\n", player.agilidade);
    printf("Inteligencia: %d\n", player.inteligencia);
    printf("Carisma: %d\n", player.carisma);
    printf("Vida Maxima: %d\n", player.vida_max);
    printf("HP: %d\n", player.hp);
    printf("Mana Maxima: %d\n", player.mana_max);
    printf("MP: %d\n", player.mp);
    cross_platform_sleep(5000);
    limparTerminal();

    // Adicione esta inicialização para o inventário:
    for (int i = 0; i < 20; i++) {
        player.inventario[i] = 0; // Inicializa cada slot do inventário com 0
    }
    save(player);
    return;
}

void limparTerminal(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
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


void histInic(){
    char nome[100] = {0};
    /*textoTela("Anos no passado, nossos ancestrais viviam tranquilamente...\n", 200);
    textoTela("Quer dizer\n", 300);
    textoTela(". . .\n", 1000);
    textoTela("No limite, do possivel!", 200);
    cross_platform_sleep(2000);
    printf("\n\n(Pressione ENTER para continuar...)\n");
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
    textoTela("Seus exercitos marcharam por desertos escaldantes,", 300);
    textoTela("atravessaram selvas malditas e conquistaram fortalezas tidas como impenetraveis.\n", 300);
    cross_platform_sleep(100);
    textoTela("Reis se ajoelharam. Imperios cairam.\n", 300);
    textoTela("Valdoran nao pedia permissao...\n", 400);
    textoTela("Eles queriam...\n", 400);
    textoTela("Eles \033[31mtomavam...\033[0m\n", 600);
    printf("\n\n(Pressione ENTER para continuar...)\n");
    fgets(nome, 100, stdin);


    limparTerminal();
    textoTela("A Cidade-Corona, capital do reino, resplandece como um farol de poder e conhecimento.\n", 200);
    textoTela("Torres encantadas tocam os ceus, e seus sabios dominam a propria essencia da magia.\n", 200);
    textoTela("Seus forjadores moldam armas que sussurram lendas a cada golpe.\n", 200);
    textoTela("E no trono de obsidiana, repousa o soberano mais temido do mundo...\n", 300);
    textoTela("...e talvez o mais odiado tambem.\n", 200);
    printf("\n\n(Pressione ENTER para continuar...)\n");
    fgets(nome, 100, stdin);

    limparTerminal();
    textoTela("Lugar incrivel, ne?\n", 300);
    textoTela("Mas . . .\n", 500);
    textoTela("Nossa aventura comeca aqui:\n", 300);
    cross_platform_sleep(1500);
    printf("\n\n(Pressione ENTER para continuar...)\n");
    fgets(nome, 100, stdin);

    limparTerminal();
    textoTela("\033[1;32mTAMARELANDIA DO NORTE.\033[0m\n", 300);
    cross_platform_sleep(2000);
    textoTela("Uma aldeia esquecida por todos os mapas decentes.\n", 200);
    textoTela("Talvez por ser considerado gasto de tinta . . .  talvez. . . \n", 200);
    textoTela("Localizada entre o Pantano Cheiroso e a Colina dos Bodes Falantes.\n", 200);
    printf("\n\n(Pressione ENTER para continuar...)\n");
    fgets(nome, 100, stdin);

    limparTerminal();
    textoTela("Longe de toda a gloria, da riqueza e da magia de Valdoran,\n", 200);
    textoTela("Aqui a aventura mais comum e desentupir o poco da praca.\n", 200);
    textoTela("E o maior guerreiro da vila? . . . . .\n", 500);
    textoTela("Um velho que jura ter lutado com uma galinha possuida.\n", 300);
    printf("\n\n(Pressione ENTER para continuar...)\n");
    fgets(nome, 100, stdin);*/

    //história comentada para ser mais rápido.

    /*limparTerminal();
    textoTela("Mas todo destino grandioso comeca com passos pequenos.\n", 200);
    textoTela("E hoje . . .\n", 500);
    textoTela("Errrn . . . Hoje! . . .\n", 500);
    textoTela("Desculpe, mas qual seu nome mesmo?\n\n", 300);*/
    getchar() != '\n'; 
    fgets(nome, 100, stdin);
    aleatJogador(nome);
    readItems();

    limparTerminal();
    textoTela("Realmente . . .\n", 200);
    int tamanho = strlen(nome);
    if (tamanho > 1 && nome[tamanho - 1] == '\n') {
        nome[tamanho - 1] = '\0'; // Remove o caractere de nova linha
    }
    for (int i = 0; i < tamanho; i++){
        printf("%c", nome[i]);
        fflush(stdout);
        cross_platform_sleep(2000 / tamanho); // Divide o tempo de espera pelo tamanho do nome 
    }
    textoTela(" .  .  .  \n", 200);
    textoTela("Um verdadeiro nome de guerreiro . . .\n\nQuer comprar algo na minha loja?", 400);
}


void gerarPers(){
    histInic();
    readItems();
}

void telaInicial(){
    limparTerminal();
    cabecaTela("A ascensao na torre!");
    printf("1 - Iniciar um novo jogo.\n");
    printf("2 - Continuar um jogo gravado.\n");
    printf("3 - Ranking de pontos.\n");
    printf("4 - Listar integrantes do grupo.\n");
    printf("5 - Fechar jogo.\n\n");

    char n;
    scanf("%s", &n);

    switch (n) {
        case '1':
            limparTerminal();
            gerarPers();
            break;
        case '2':

            break;
        case '3':
            limparTerminal();
            cabecaTela("Ranking");
            break;
        case '4':

            limparTerminal();
            cabecaTela("Interantes");
            printf("\n\033[31mGuilherme Lionel de Souza\033[0m\n");
            printf("\n\033[34mAlexandre Gabriel Angelo de Souza Blandino\033[0m\n");
            printf("\n\033[32mSamuel Pereira da Silva\n\033[0m\n");
            printf("[1] - Voltar para a tela inicial.\n");
            
            scanf("%s", &n);
            
            telaInicial();
            break;
        case '5':
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
