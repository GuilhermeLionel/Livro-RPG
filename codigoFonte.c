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
    char nome[51];
    int tipo;
    int preco;
    int raridade;
    int bonus[2][3];
} ITEMHANDLER;
ITEMHANDLER item[20];

typedef struct dados{
    char nome[51];
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
    int moedas;
} DADOS;

DADOS player;

int numAle(int range);
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
void loja(int level);
void histInic();
void tipoItem(char *tipo, int n);

void tipoItem(char *tipo, int n)
{
    switch(n)
    {
        case 0:
            strcpy(tipo, "Consumivel");
            break;
        case 1:
            strcpy(tipo, "Arma");
            break;
        case 2:
            strcpy(tipo, "Armadura");
            break;
        case 3:
            strcpy(tipo, "Colecionavel");
            break;
        case 4:
            strcpy(tipo, "Reliquia");
            break;
        default:
            strcpy(tipo, "Desconhecido");
    }
}

void bonusItem(char *bonus, int n)
{
    switch(n)
    {
        case 1:
            strcpy(bonus, "Frc");
            break;
        case 2:
            strcpy(bonus, "Agl");
            break;
        case 3:
            strcpy(bonus, "Int");
            break;
        case 4:
            strcpy(bonus, "Car");
            break;
        case 5:
            strcpy(bonus, "Prt");
            break;
        case 6:
            strcpy(bonus, "HP");
            break;
        case 7:
            strcpy(bonus, "MP");
            break;
        case 8:
            strcpy(bonus, "Dano");
            break;
    }
}

void loja(int level)
{
    char tipo[20], nomeBonus[20];
    int espacos, resto;
    int i, j;
    int x, tamanhoNumero, tamanhoString, a[3], quantBonus, y[3];
    int largura = 30; // Tamanho de caracteres reservado na horizontal para cada item

    for(i = 1; i <= 3; i++) // Imprime os numeros de 1 a 3 espaçados igualmente
    {
        for(j = 0; j < (largura - 1)/2; j++) printf(" ");
        printf("%d", i);
        for(j = 0; j < (largura - 1)/2 + 1; j++) printf(" ");
    }
    printf("\n");

    a[0] = numAle(3);  // Gera 3 numeros aleatorios de 1 a 3 que seriam os ids dos itens
    a[1] = numAle(3);
    a[2] = numAle(3);
    while(1)  // Garante que os 3 numeros sejam diferentes
    {
        if(a[0] == a[1]) a[1] = numAle(3);
        if(a[0] == a[2] || a[1] == a[2]) a[2] = numAle(3);
        if(a[0] != a[1] && a[0] != a[2] && a[1] != a[2]) break;
    }

    for(i = 0; i < 3; i++) //Imprime os nomes dos itens espaçados igualmente
    {
        espacos = (largura - strlen(item[a[i]].nome)) / 2; // Calcula o tamanho do espaço a ser impresso
        resto = (largura - strlen(item[a[i]].nome)) % 2; // Calcula o resto para saber se precisa de mais um espaço
        for(j = 0; j < espacos; j++) printf(" ");  
        printf("%s", item[a[i]].nome);
        for(j = 0; j < espacos + resto; j++) printf(" ");
    }
    printf("\n");
    
    for(i = 0; i < 3; i++)
    {
        x = item[a[i]].preco;
        for(tamanhoNumero = 0; x > 0; tamanhoNumero++) x /= 10; //Calcula o tamanho de caracteres do numero do preco
        espacos = (largura - tamanhoNumero - 14)/2; // -14 é por casua do "Preco: " e " Moedas"
        resto = (largura - tamanhoNumero - 14) % 2; // Calcula o resto para saber se precisa de mais um espaço
        for(j = 0; j < espacos; j++) printf(" ");
        printf("Preco: %d Moedas", item[a[i]].preco);
        for(j = 0; j < espacos + resto; j++) printf(" ");

    }
    printf("\n");

    for(i = 0; i < 3; i++)
    {
        tipoItem(tipo, item[a[i]].tipo); //Pega o tipo do item e coloca na string tipo
        espacos = (largura - strlen(tipo)) / 2;
        resto = (largura - strlen(tipo)) % 2;
        for(j = 0; j < espacos; j++) printf(" "); 
        printf("%s", tipo);
        for(j = 0; j < espacos + resto; j++) printf(" "); 
    }
    printf("\n");
    
    for(i = 0; i < 3; i++)
    {
        for(j = 0, tamanhoNumero = 0, quantBonus = 0; j < 3; j++) // Imprime os bonus do item
        {
            if(item[a[i]].bonus[0][j] != 0) // Verifica se o bonus é diferente de 0
            {
                quantBonus++;
                x = item[a[i]].bonus[1][j];
                for(; x != 0; tamanhoNumero++) x /= 10; 
            }
        }
        tamanhoNumero += quantBonus; // +1 para o sinal de cada bonus
        
        for(j = 0, tamanhoString = 0; j < quantBonus; j++) // Imprime os bonus do item
        {
            bonusItem(nomeBonus, item[a[i]].bonus[0][j]); 
            tamanhoString += strlen(nomeBonus);
        }
        espacos = (largura - tamanhoString - 1 - tamanhoNumero - (2 * (quantBonus - 1))) / 2;
        resto = (largura - tamanhoString - 1 - tamanhoNumero - (2 * (quantBonus - 1))) % 2;
        for(x = 0; x < espacos; x++) printf(" ");
        for(j = 0; j < quantBonus; j++) // Imprime os bonus do item
        {
            bonusItem(nomeBonus, item[a[i]].bonus[0][j]);
            if(item[a[i]].bonus[1][j] > 0)printf("+%d %s", item[a[i]].bonus[1][j], nomeBonus);
            if(item[a[i]].bonus[1][j] < 0)printf("%d %s", item[a[i]].bonus[1][j], nomeBonus);
            if(j < quantBonus - 1) printf("  ");
        }
        for(x = 0; x < espacos + resto; x++) printf(" ");
    }
    printf("\n");
}


void setItens(int q){
    int i;
    FILE *arq = fopen("Dados do Jogo/items.txt", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    for (i = 0; i < q; i++) 
    {
        fgets(item[i].nome, sizeof(item[i].nome), arq);
        item[i].nome[strcspn(item[i].nome, "\n")] = 0; // Substitui caractere "\n" por "\0"

        fscanf(arq, "Tipo: %d\nPreco: %d\nRaridade: %d\nBonus: %d, %d, %d\nQuantB: %d, %d, %d\n\n", 
               &item[i].tipo, 
               &item[i].preco, 
               &item[i].raridade, 
               &item[i].bonus[0][0], 
               &item[i].bonus[0][1], 
               &item[i].bonus[0][2],
               &item[i].bonus[1][0],
               &item[i].bonus[1][1],
               &item[i].bonus[1][2]);
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
    player.nome[strcspn(player.nome, "\n")] = 0;

    fprintf(arq, "%s\n", player.nome);
    
    fprintf(arq, "HP: %d/%d\n", player.hp, player.vida_max);
    fprintf(arq, "MP: %d/%d\n", player.mp, player.mana_max);
    fprintf(arq, "Forca: %d\n", player.forca);
    fprintf(arq, "Protecao: %d\n", player.protecao);
    fprintf(arq, "Agilidade: %d\n", player.agilidade);
    fprintf(arq, "Inteligencia: %d\n", player.inteligencia);
    fprintf(arq, "Carisma: %d\n", player.carisma);
    fprintf(arq, "Moedas: %d\n", player.moedas);

    for (int i = 0; i < 20; i++) {
        fprintf(arq, "%d\n", player.inventario[i]);
    }
    fclose(arq);
}


void load(DADOS *player){
    FILE *arq = fopen("Dados do Jogo/save.txt", "rt");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fgets(player->nome, sizeof(player->nome), arq);
    player->nome[strcspn(player->nome, "\n")] = 0; //Transforma caractere "\n" em "\0"

    
    fscanf(arq, "HP: %d/%d\n", &player->hp, &player->vida_max);
    fscanf(arq, "MP: %d/%d\n", &player->mp, &player->mana_max);
    fscanf(arq, "Forca: %d\n", &player->forca);
    fscanf(arq, "Protecao: %d\n", &player->protecao);
    fscanf(arq, "Agilidade: %d\n", &player->agilidade);
    fscanf(arq, "Inteligencia: %d\n", &player->inteligencia);
    fscanf(arq, "Carisma: %d\n", &player->carisma);
    fscanf(arq, "Moedas: %d\n", &player->moedas);

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
    int i;
    for (i = 0 ; i < 20 ; i++)
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
    player.moedas = 10;

    // Adicione esta inicialização para o inventário:
    for (i = 0; i < 20; i++) {
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

void verifica_nome_player(char *nome) {
    int i = 0, OK = 0; // OK: verifica se o nome original sofreu alguma alteração
    char nome_corrigido[40] = {0}, nome_novo[100]; 
    // nome_corrigido = altera o nome (sem os casos não permitidos)
    // nome_novo = pede um novo nome para a recursão
    limparTerminal();
    while (nome[i] != '\n') {
        if (nome[i] == ' ') { // + de uma palavra
            OK = 1;
            for(i = 0; nome[i] != ' '; i++) 
                nome_corrigido[i] = nome[i]; // copia a string até o caractere de espaço para nome_corrigido
            strcpy(player.nome, nome_corrigido); // transfere o texto corrigido para nome.player
            textoTela("Nome maneiro, mas vou te chamar apenas de...", 200);
            printf("%s\n", player.nome); // não consegui colocar essa parte na textoTela() :(
            textoTela("...", 600);
            textoTela("Beleza?", 800);
            break; 
        }
        else if (!(nome[i] >= 'A' && nome[i] <= 'Z') && !(nome[i] >= 'a' && nome[i] <= 'z')) { // números e caracteres especiais
            OK = 1;
            textoTela("Que legal, uma pessoa com nome igual aos da internet...\n", 200);
            textoTela("Saudades do meu amigo Herobrine123.\n", 400);
            textoTela("Enfim, tem algum outro nome que eu possa te chamar?\n", 200);
            fgets(nome_novo, 100, stdin);
            verifica_nome_player(nome_novo); // recursiva para verificar o novo nome;
            break;
        }
        
        i++;
    }
    if (OK == 0) { // OK == 0: nome original permaneceu
        strcpy(player.nome, nome);
        
        textoTela("Realmente . . .\n", 200);
        int tamanho = strlen(player.nome);
        if (tamanho > 1 && player.nome[tamanho - 1] == '\n') {
            player.nome[tamanho - 1] = '\0'; // Remove o caractere de nova linha
        }
        for (int i = 0; i < tamanho; i++){
            printf("%c", player.nome[i]);
            fflush(stdout);
            cross_platform_sleep(1500 / tamanho); // Divide o tempo de espera pelo tamanho do nome 
        }
        textoTela(" .  .  .  \n", 200);
        textoTela("Um verdadeiro nome de guerreiro . . .\n\n", 400);
    }
    aleatJogador(player.nome); // player.nome vai para a função
    readItems();
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
    textoTela("Errrn . . . Hoje! . . .\n", 500); */
    textoTela("Desculpe, mas qual seu nome mesmo?\n\n", 300);
    getchar() != '\n';
    fgets(nome, 100, stdin);
    verifica_nome_player(nome);
    
    // movi essa parte pra função verifica_nome_player()
    textoTela("Quer comprar algo na minha loja?", 400); 
    printf("\n(Pressione ENTER para continuar...)\n");
    getchar(); // Espera o usuário pressionar ENTER
    limparTerminal();
    loja(1);
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
