#include <stdio.h>
#include <ctype.h>
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

#define MAX_ITEMS 66 // Define o número máximo de itens

int items = 0;
 
typedef struct buffHandler2 {
    int alvo; // 0 = Inimigo, 1 = O propio
    int tipo; // 0 = Vazio, 1 = Ataque, 2 = Ataque Especial (Inteligencia), 3 = HP, 4 = Prot, 5 = Stun
    int duracao; // Duração do buff em turnos
    float valor; // Valor do buff
} BUFFHANDLER;

typedef struct abilityHandler2{
    char nome[51];
    char descricao[201];
    int tipo; // 0 = Vazio, 1 = Ataque, 2 = Ataque Especial (Magia), 3 = Cura Fixa, 4 = Prot, 5 = Buff/Debuff
    BUFFHANDLER buff; // Buff que a habilidade pode causar
    float qtdmg; // Dano ou cura
    int custo; // Custo de mana
    int chance; // Chance de acerto da habilidade
    int requisito[2][3]; // Requisitos para usar a habilidade (0 = Nenhum, 1 = Forca, 2 = Inteligencia, 3 = Carisma, 4 = Protecao, Agilidade) [tipo de requisito][quant. de requisito]

} ABILITYHANDLER;

typedef struct itemHandler2{
    char nome[51];
    int tipo;
    int preco;
    int raridade;
    int bonus[2][3];
} ITEMHANDLER;
ITEMHANDLER item[MAX_ITEMS];

typedef struct dados{
    char nome[51];
    int protecao;
    int vidaMax;
    int hp;
    int mp;
    int manaMax;
    int forca;
    int agilidade;
    int inteligencia;
    int carisma;
    int inventario[2][20];
    int moedas;
    int equipado[4]; 
    int exp;
    int level;
    int expMax;
    int skillPoints;
} DADOS;

DADOS player;
int sala = 0;


int idAleatorio(int raridade); // Gera um id aleatorio dado a raridade do item
int aleatorizaChance(int tamanho, float chance[tamanho]); // Dado o tamanho do vetor chance, rola um dado de 0 ate o tamanho do vetor menos - 1 e retorna o valor sorteado considerando as chances de cada valor
int localInv(int id); // Retorna a posição mais proxima de zero de um item com determinado id no inventário
int qtdInv(int id); // Retorna a quantidade do item no inventário
int numAle(int range);
int espacoInv(int id); // Retorna o espaco em que um item pode ser alocado no inventario
int digitos(int n); // Retorna a quantidade de dígitos do número
void vitrine(int a[]);
void cross_platform_sleep(int ms);
void limparTerminal();
void gerarPasta();
void aleatJogador(char *txt);
void setItens(int q);
void readItems();
void save(DADOS player);
void load(DADOS *player, int *sala);
void textoTela(const char *texto, int seg);
void cabecaTela(char* x);
void loja(int level);
void histInic();
void tipoItem(char *tipo, int n);
void addItem(int id);
void strip(char *nome);
void verifica_nome_player(char *nome);
void verInventario(int modo);
void bonusItem(char *bonus, int n);
void lixo(int espaco, int qtd);
void descreverItem(int id);
void usarItem(int espaco);
void ajustaBonus(int id, int sinal);
void mostrarStatus();
void consumivel(int id);
void limparBuffer();
void limparLinhas(int qtd);
<<<<<<< HEAD
void calculoXp(int expAt, int expMax, int lvl);
void aleatStatus(int status[], int pontos);
void tomadaDecisao();
=======
void calculoExp();
void aleatStatus(int pontos);
>>>>>>> 8b26f14b88701ae0e10e45670eb83077ddeeaab2

void limparLinhas(int qtd)
{
    for(; qtd > 0; qtd--)
    {
        printf("\033[1A");
        printf("\033[2K");
    }
}

void limparBuffer()
{
    int clear;
    while((clear = getchar()) != '\n' && clear != EOF);
}

void mostrarStatus()
{
    limparTerminal();

    int largura = 50;
    int espacos;
    int i;
    char txt[71];

    sprintf(txt, "%s Lvl %d", player.nome, player.level);
    printf("%s", txt);
    espacos = largura - strlen(txt);
    for(i = 0; i < espacos; i++) printf(" ");

    sprintf(txt, "Equipados:");
    espacos = (largura - strlen(txt)) / 2;
    for(i = 0; i < espacos; i++) printf(" ");  
    printf("%s", txt);

    printf("\n");

    sprintf(txt, "HP: %d/%d", player.hp, player.vidaMax);
    printf("%s", txt);
    espacos = largura - strlen(txt);
    for(i = 0; i < espacos; i++) printf(" ");

    printf("\n");    

    sprintf(txt, "Mana: %d/%d", player.mp, player.manaMax);
    printf("%s", txt);
    espacos = largura - strlen(txt);
    for(i = 0; i < espacos; i++) printf(" ");

    sprintf(txt, "%s", item[player.equipado[3]].nome);
    espacos = (largura - strlen(txt)) / 2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s", txt);

    printf("\n");

    sprintf(txt, "Forca: %d", player.forca);
    printf("%s", txt);
    espacos = largura - strlen(txt);
    for(i = 0; i < espacos; i++) printf(" ");

    sprintf(txt, "%s", item[player.equipado[1]].nome);
    espacos = (largura - strlen(txt)) / 2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s", txt);

    printf("\n");

    sprintf(txt, "Agilidade: %d", player.agilidade);
    printf("%s", txt);
    espacos = largura - strlen(txt);
    for(i = 0; i < espacos; i++) printf(" ");

    sprintf(txt, "%s", item[player.equipado[2]].nome);
    espacos = (largura - strlen(txt)) / 2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s", txt);

    printf("\n");

    sprintf(txt, "Inteligencia: %d", player.inteligencia);
    printf("%s", txt);
    espacos = largura - strlen(txt);
    for(i = 0; i < espacos; i++) printf(" ");

    sprintf(txt, "%s", item[player.equipado[0]].nome);
    espacos = (largura - strlen(txt)) / 2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s", txt);

    printf("\n");
    printf("Carisma: %d\n", player.carisma);
    printf("Protecao: %d\n", player.protecao);
    printf("\nSkill Points: %d\n", player.skillPoints);

    int barSize = 60;

    float media = (float)player.exp / (float)player.expMax;
    if (media > 1) media = 1;

    for(i = 0; i < media * barSize; i++) printf("=");
    for(i = 0; i < (barSize - media * barSize); i++) printf("-");
    printf("  %d/%d", player.exp, player.expMax);
    printf("\nEXP\n\n");
    
    int escolha;

    scanf("%d", &escolha);

    if(escolha == 1) 
    {
        player.exp += (float)player.expMax / 5.0;
        mostrarStatus();
    }
    if(escolha == 2)
    {
        calculoExp();
        save(player);
        mostrarStatus();
    }
}

void ajustaBonus(int id, int sinal)
{
    int i;
    for(i = 0; i < 3; i++)
    {
        if(item[id].bonus[0][i] != 0) 
        {
            switch(item[id].bonus[0][i])
            {
                case 1: // Forca
                    player.forca += item[id].bonus[1][i] * sinal;
                    break;
                case 2: // Agilidade
                    player.agilidade += item[id].bonus[1][i] * sinal;
                    break;
                case 3: // Inteligencia
                    player.inteligencia += item[id].bonus[1][i] * sinal;
                    break;
                case 4: // Carisma
                    player.carisma += item[id].bonus[1][i] * sinal;
                    break;
                case 5: // Protecao
                    player.protecao += item[id].bonus[1][i] * sinal;
                    break;
                case 6: // HP
                    player.vidaMax += item[id].bonus[1][i] * sinal;
                    player.hp += item[id].bonus[1][i] * sinal; // Ajusta o HP atual
                    break;
                case 7: // MP
                    player.manaMax += item[id].bonus[1][i] * sinal;
                    player.mp += item[id].bonus[1][i] * sinal; // Ajusta o MP atual
                    break;
                case 8: // Dano
                    // mudar depois
                    break;
                case 9: // % HP Max
                    player.vidaMax += (player.vidaMax * item[id].bonus[1][i] / 100) * sinal;
                    player.hp += (player.vidaMax * item[id].bonus[1][i] / 100) * sinal; // Ajusta o HP atual
                    break;
                case 10: // % MP Max
                    player.manaMax += (player.manaMax * (item[id].bonus[1][i] + 100) / 100) * sinal;
                    player.mp += (player.manaMax * (item[id].bonus[1][i] + 100) / 100) * sinal;
                    break;
            }
        }
    }
}

void consumivel(int id)
{
    int i;
    for(i = 0; i < 3; i++)
    {
        if(item[id].bonus[0][i] != 0) 
        {
            switch(item[id].bonus[0][i])
            {
                case 6: // HP
                    player.hp += item[id].bonus[1][i];
                    break;
                case 7: // MP
                    player.mp += item[id].bonus[1][i];
                    break;
                case 9: // % HP Max
                    player.hp += (player.vidaMax * item[id].bonus[1][i] / 100);
                    if(player.hp > player.vidaMax) player.hp = player.vidaMax;
                    break;
                case 10: // % MP Max
                    player.mp += (player.manaMax * item[id].bonus[1][i] / 100);
                    if (player.mp > player.manaMax) player.mp = player.manaMax;
                    break;
            }
        }
    }
}

void usarItem(int espaco)
{
    int id = player.inventario[0][espaco]; // ID do item a ser usado
    int tipo = item[id].tipo; // Tipo do item a ser usado
    if(id == 0) 
    {
        printf("Esse item nao pode ser usado.\n");
        cross_platform_sleep(2000);
        return;
    }
    lixo(espaco, 1); // Remove o item do inventário
    if(tipo >= 1 && tipo <= 4)
    {
        tipo = tipo - 1; // Ajusta o tipo para o índice do vetor equipado
        if(player.equipado[tipo] != 0) 
        {
            addItem(player.equipado[tipo]); // Adiciona o item equipado de volta ao inventário
            ajustaBonus(player.equipado[tipo], -1); // Remove os bônus do item equipado
            player.equipado[tipo] = 0; // Remove o item equipado
        }
        player.equipado[tipo] = id; // Equipa o novo item
        ajustaBonus(id, 1); // Adiciona os bônus do novo item equipado
    }
    if(tipo == 5) // Se o item for do tipo Consumível
    {
        consumivel(id);
        lixo(espaco, 1);
    }
}

int localInv(int id)
{
    for(int i = 0; i < 20; i++)
    {
        if(player.inventario[0][i] == id)
        {
            return i;
        }
    }
    return -1;
}

int qtdInv(int id)
{
    int qtd = 0, i;
    int local = localInv(id);
    if(local >= 0)
    {
        for(i = local; i < 20; i++)
        {
            if(player.inventario[0][i] == id)
            {
                qtd += player.inventario[1][i];
            }
        }
    }
    return qtd;
}

void descreverItem(int id)
{
    int i;
    int largura = 60;
    int espaco, resto;
    int qtdBonus, qtdDigitos, strings;
    char tipo[20], bonus[20];
    espaco = (largura - strlen(item[id].nome)) / 2;
    resto = (largura - strlen(item[id].nome)) % 2;
    for(i = 0; i < espaco; i++) printf(" ");
    printf("%s", item[id].nome);
    for(i = 0; i < espaco + resto; i++) printf(" ");
    printf("\n");

    espaco = (largura - (digitos(item[id].preco)) - 14) / 2;
    resto = (largura - (digitos(item[id].preco)) - 14) % 2;
    for(i = 0; i < espaco; i++) printf(" ");
    if(item[id].preco != 1) printf("Valor: %d Moedas", item[id].preco);
    else printf("Valor: %d Moeda", item[id].preco);
    for(i = 0; i < espaco + resto; i++) printf(" ");
    printf("\n");

    tipoItem(tipo, item[id].tipo);
    espaco = (largura - strlen(tipo)) / 2;
    resto = (largura - strlen(tipo)) % 2;
    for(i = 0; i < espaco; i++) printf(" ");
    printf("%s", tipo);
    for(i = 0; i < espaco + resto; i++) printf(" ");
    printf("\n");

    for(i = 0, qtdBonus = 0, qtdDigitos = 0, strings = 0; i < 3; i++)
    {
        if(item[id].bonus[0][i] != 0) 
        {
            qtdBonus++;
            qtdDigitos += digitos(item[id].bonus[1][i]);
            bonusItem(bonus, item[id].bonus[0][i]);
            strings += strlen(bonus);
        }
    }
    strings += qtdBonus;
    espaco = (largura - strings - qtdDigitos - (2 * (qtdBonus - 1))) / 2;
    resto = (largura - strings - qtdDigitos - (2 * (qtdBonus - 1))) % 2;
    for(i = 0; i < espaco; i++) printf(" ");
    for(i = 0; i < qtdBonus; i++)
    {
        bonusItem(bonus, item[id].bonus[0][i]);
        if(item[id].bonus[1][i] > 0) printf("+%d %s", item[id].bonus[1][i], bonus);
        else printf("%d %s", item[id].bonus[1][i], bonus);
        if(i < qtdBonus - 1) printf("  ");
    }
    for(i = 0; i < espaco + resto; i++) printf(" ");
    printf("\n");
    int local;
    local = localInv(id); // Verifica se o item está no inventário
    if(local >= 0)
    {
        printf("Voce tem: %d\n", qtdInv(id)); // Mostra a quantidade do item no inventário
    }
    else
    {
        printf("Voce nao tem esse item.\n");
    }
    printf("\n");
}


void lixo(int espaco, int qtd)
{
    int local;
    int total = qtdInv(player.inventario[0][espaco]); // Total de itens no inventário
    int id = player.inventario[0][espaco]; // ID do item a ser removido
    if(espaco >= 0 && espaco <= 19)
    {
        if(qtd <= total)
        { // Verifica se a quantidade a ser removida é maior ou igual a quantidade do item no inventário
            while(qtd > 0)
            {
                local = localInv(id); // Verifica a posição do item no inventário
                if(qtd - player.inventario[1][local] > 0) 
                {
                    qtd -= player.inventario[1][local]; // Subtrai a quantidade do item no inventário
                    player.inventario[0][local] = 0; // Remove o item do inventário
                    player.inventario[1][local] = 0; // Zera a quantidade do item no inventário
                }
                else 
                {
                    player.inventario[1][local] -= qtd;
                    qtd = 0; // Zera a quantidade a ser removida
                }
                if(player.inventario[1][local] <= 0) 
                {
                    player.inventario[0][local] = 0; // Se a quantidade do item for menor ou igual a 0, remove o item do inventário
                    player.inventario[1][local] = 0; // Zera a quantidade do item no inventário
                }
            }
        }
        else printf("Quantidade invalida. Nao foi possivel remover o item.\nTentativa de remover %d de %d\n", qtd, total);
    }
}

int digitos(int n)
{
    int count = 0;
    if(n == 0) return 1; // Se o numero for 0, retorna 1
    while(n > 0)
    {
        n /= 10;
        count++;
    }
    return count; // Retorna a quantidade de digitos do numero
}

void verInventario(int modo)
{
    limparTerminal();
    char txt[50];
    int i, j = 0, k;
    int largura = 30;
    int espaco, escolha, escolha2, qtd, erro = 0;
    int tamanho;
    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 4; j++)
        {
            sprintf(txt, "%d. %s(x%d)", i * 4 + j + 1, item[player.inventario[0][4 * i + j]].nome, player.inventario[1][4 * i + j]);
            if(strlen(txt) > 28)
            {
                tamanho = 28 - (strlen(txt) - strlen(item[player.inventario[0][4 * i + j]].nome)) - 4;
                sprintf(txt, "%d. %.*s... (x%d)", i * 4 + j + 1, tamanho ,item[player.inventario[0][4 * i + j]].nome, player.inventario[1][4 * i + j]);
            }
            printf("%s", txt);
            espaco = largura - strlen(txt);
            for(k = 0; k < espaco; k++) printf(" ");
        }
        printf("\n");
    }
    switch(modo)
    {
        case 1: // Inventario no modo ver inventario na loja
            printf("\n[0] SAIR\n");
            printf("\nDigite o numero do item para ver informacoes:\n");
            scanf("%d", &escolha);
            if(escolha == 0) {
                // Sai imediatamente
                return;
            }
            if(escolha < 0 || escolha > 20 || player.inventario[0][escolha - 1] == 0) 
            {
                limparTerminal();
                printf("Escolha invalida. Digite novamente.\n");
                cross_platform_sleep(2000);
                verInventario(1);
            }
            else 
            {
                limparTerminal();
                getchar();
                descreverItem(player.inventario[0][escolha - 1]); // Descreve o item escolhido
                printf("\n");
                for(i = 0; i < ((60 - 32) / 2); i++) printf(" ");
                printf("Pressione [ENTER] para continuar\n\n");
                getchar(); // Espera o ENTER do usuário
                verInventario(1);
            }
            break;

        case 2: // Inventario no modo vender item
            printf("\n[0] SAIR\n");
            printf("Digite o numero do item que deseja vender:\n");

            while(1)
            {
                if((scanf("%d", &escolha)) != 1 || escolha < 0 || escolha > 20) 
                {
                    limparLinhas(1);
                    if(erro) limparLinhas(1);
                    printf("Escolha invalida. Digite novamente.\n");
                    erro = 1;
                    limparBuffer();
                }
                else if(player.inventario[0][escolha - 1] == 0)
                {
                    limparLinhas(1);
                    if(erro) limparLinhas(1);
                    printf("Esse item nao pode ser vendido. Digite novamente.\n");
                    erro = 1;
                }
                else break;
            }

            if(escolha > 0)
            {
                erro = 0;
                limparTerminal();
                descreverItem(player.inventario[0][escolha - 1]); // Descreve o item escolhido
                printf("\nDeseja vender esse item?\n");
                printf("[1] SIM     [0] NAO\n");
                while(1) // Garante que a escolha seja valida
                {
                    if((scanf("%d", &escolha2)) != 1 || escolha2 < 0 || escolha2 > 1)
                    {
                        limparLinhas(1);
                        if(!erro) printf("Escolha invalida. Digite novamente.\n");
                        erro = 1;
                        limparBuffer();
                    }
                    else break; // Sai do loop se a escolha for valida
                }
                if(escolha2 == 1)
                {
                    limparLinhas(3);
                    if(erro) limparLinhas(1);
                    erro = 0;
                    printf("Digite a quantidade que deseja vender:\n");
                    while(1)
                    {
                        if((scanf(" %d", &qtd)) != 1 || (qtd > qtdInv(player.inventario[0][escolha - 1])))
                        {
                            limparLinhas(1);
                            if(!erro) printf("Escolha invalida. Digite novamente.\n");
                            erro = 1;
                            limparBuffer();
                        }
                        else 
                        {
                            break;
                        }
                    }

                    player.moedas += item[player.inventario[0][escolha - 1]].preco * qtd; // Adiciona o preco do item as moedas do jogador
                    lixo(escolha - 1, qtd);
                    limparTerminal();
                    printf("Item(s) vendido(s) com sucesso!\n\n\n");
                    cross_platform_sleep(1000);
                    
                }
                verInventario(2);
            }
            break;
        default:
            printf("Modo invalido.\n");
            break;
    }
}

void addItem(int id)
{
    int espaco = espacoInv(id);
    if(espaco != -1)
    {
        player.inventario[0][espaco] = id;
        player.inventario[1][espaco]++;
    }
}

void tipoItem(char *tipo, int n)
{
    switch(n)
    {
        case 0:
            strcpy(tipo, "Vazio");
            break;
        case 1:
            strcpy(tipo, "Arma");
            break;
        case 2:
            strcpy(tipo, "Armadura");
            break;
        case 3:
            strcpy(tipo, "Reliquia");
            break;
        case 4:
            strcpy(tipo, "Capacete");
            break;
        case 5:
            strcpy(tipo, "Consumivel");
            break;
        case 6:
            strcpy(tipo, "Item de Batalha");
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
            strcpy(bonus, "Protec");
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
        case 9:
            strcpy(bonus, "%% HP");
            break;
        case 10:
            strcpy(bonus, "%% MP");
            break;

    }
}

int espacoInv(int id)
{
    int i, stack = 1, semId = 0; // Variavel a define se o item é stackavel ou nao, com 0 para nao e 1 para sim
    if(item[id].tipo >= 0 && item[id].tipo <= 3) stack = 0; // Se o item for do tipo Vazio, Arma, Armadura ou Reliquia, a variavel a recebe 1, caso contrario recebe 0
    switch(stack)
    {
        case 1: // Se o item for stackavel, procura o id do item no inventario
            for(i = 0; player.inventario[0][i] != id; i++) 
            {
                if(i == 19) 
                {
                    semId = 1;
                    break;
                }
            }
            if(!semId) break; // Se o id estiver presente, não precisa procurar por espaço vazio
        case 0: // Se o item não for stackavel ou é stackavel, mas nao esta presente no inventario, procura o primeiro espaço vazio no inventário
            for(i = 0; player.inventario[0][i] != 0; i++) if(i == 19) break;
    }
    if(i <= 19) return i; // Retorna o índice do primeiro espaço com o id desejado no inventário
    else return -1; // Se não encontrar retorna -1
}

void vitrine(int a[])
{
    char tipo[20], nomeBonus[20];
    int espacos, resto;
    int i, j;
    int x, tamanhoNumero, tamanhoString, quantBonus;
    int largura = 30;
    limparTerminal();
    for(i = 1; i <= 3; i++) // Imprime os numeros de 1 a 3 espaçados igualmente
    {
        for(j = 0; j < (largura - 1)/2; j++) printf(" ");
        printf("%d", i);
        for(j = 0; j < (largura - 1)/2 + 1; j++) printf(" ");
    }
    printf("\n");


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
        if(item[a[i]].tipo != 0){
            x = item[a[i]].preco;
            if(x != 1){
                for(tamanhoNumero = 0; x > 0; tamanhoNumero++) x /= 10; //Calcula o tamanho de caracteres do numero do preco
                espacos = (largura - tamanhoNumero - 14)/2; // -14 é por casua do "Preco: " e " Moedas"
                resto = (largura - tamanhoNumero - 14) % 2; // Calcula o resto para saber se precisa de mais um espaço
                for(j = 0; j < espacos; j++) printf(" ");
                printf("Preco: %d Moedas", item[a[i]].preco);
                for(j = 0; j < espacos + resto; j++) printf(" ");
            }
            else{  //caso o preco seja igual a 1 retira o s no moeda
                for(j = 0; j < 8; j++)printf(" ");
                printf("Preco: 1 Moeda");
                for(j = 0; j < 8; j++)printf(" ");
            }
        }
        else for(j = 0; j < largura; j++) printf(" ");
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
        if(item[a[i]].tipo != 0){
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
            tamanhoString += quantBonus; // + 1 para cada espaco entre o numero e o bonus
            espacos = (largura - tamanhoString - tamanhoNumero - (2 * (quantBonus - 1))) / 2;
            resto = (largura - tamanhoString - tamanhoNumero - (2 * (quantBonus - 1))) % 2;
            for(j = 0; j < espacos; j++) printf(" ");
            for(j = 0; j < quantBonus; j++) // Imprime os bonus do item
            {
                bonusItem(nomeBonus, item[a[i]].bonus[0][j]);
                if(item[a[i]].bonus[1][j] > 0)printf("+%d %s", item[a[i]].bonus[1][j], nomeBonus);
                if(item[a[i]].bonus[1][j] < 0)printf("%d %s", item[a[i]].bonus[1][j], nomeBonus);
                if(j < quantBonus - 1) printf("  ");
            }
            for(x = 0; x < espacos + resto; x++) printf(" ");
        }
        else for(j = 0; j < largura; j++) printf(" ");
    }
    printf("\n");
    printf("\nSuas moedas: %d\n", player.moedas);
    printf("\n");
    printf("Digite o numero do item que deseja comprar: \n[0] SAIR;\n[4] VENDER ITENS;\n[5] INVENTARIO.\n\n");
    int escolha, erro = 1;
    while (1) 
    {
        if(scanf("%d", &escolha) != 1 || escolha < 0 || escolha > 6)
        {
            printf("\033[1A");
            printf("\033[2K");
            if(erro)printf("Entrada invalida. Por favor, digite um numero entre 0 e 5.\n");
            erro = 0;
            limparBuffer();
        }
        else break; // entrada válida, sai do loop
    }
 
    if(escolha == 0) 
    {
        limparTerminal();
    }
    if(escolha == 4)
    {
        verInventario(2);
        vitrine(a);
    }
    if(escolha == 5)
    {
        verInventario(1);
        vitrine(a);
    }
    if(escolha == 6) loja(sala);
    if(escolha >= 1 && escolha <= 3)
    {
        if(item[a[escolha-1]].tipo == 0) // Verifica se o item é vazio
        {
            limparTerminal();
            printf("Esse item não pode ser comprado.\n");
            cross_platform_sleep(2000);
            vitrine(a);
        }
        else
        {
            if(item[a[escolha - 1]].preco > player.moedas) 
            {
                limparTerminal();
                printf("Voce nao tem moedas suficientes para comprar esse item.\n");
                cross_platform_sleep(2000);
                vitrine(a);
            }
            else
            {
                if(espacoInv(a[escolha - 1]) < 0)  // Verifica se o inventario tem espaco para o item
                {
                    limparTerminal();
                    printf("Voce nao tem espaco suficiente no inventario para comprar esse item.\n");
                    cross_platform_sleep(2000);
                    vitrine(a);
                }
                else
                {
                    limparTerminal();
                    addItem(a[escolha - 1]); // Adiciona o item ao inventario
                    player.moedas -= item[a[escolha - 1]].preco; // Subtrai o preco do item das moedas do jogador
                    printf("Item comprado com sucesso!\n");
                    //if(item[a[escolha-1]].tipo != 4) a[escolha-1] = 0; //Retira o item da loja
                    cross_platform_sleep(1000);
                    vitrine(a); // Atualiza a vitrine
                }
            }
        }
    }
}

int aleatorizaChance(int tamanho, float chance[tamanho])
{
    int i;
    int total = 0;
    for(i = 0; i < tamanho; i++) total += chance[i];
    if(total == 0) // Se a soma das chances for 0, não há itens para sortear
    {
        printf("soma 0");
        cross_platform_sleep(2000);
        return 0;
    }
    if(total != 100)
    {
        printf("Erro: A soma das chances deve ser 100%%.\n");
        cross_platform_sleep(2000);
        return 0;
    }	
    int sorteio = numAle(101) - 1;
    for(i = 0; i < tamanho; i++)
    {
        if(sorteio > chance[i]) sorteio -= chance[i];
        else if(chance[i] == 0) continue; // Se a chance for 0, pula para o próximo
        else break; // Se a chance for maior que 0, sai do loop
    }
    return i;
}

int idAleatorio(int raridade)
{
    int i;
    int quantidade = 0;
    for(i = 0; i < items; i++) quantidade++;
    int id[quantidade]; // Vetor que guarda os ids dos itens
    quantidade = 0;
    for(i = 0; i < items; i++)
    {
        if(item[i].raridade == raridade) 
        {
            id[quantidade++] = i;
        }
    }
    return id[numAle(quantidade) - 1];
}


void loja(int level) // Sempre antes de boss. Boss a cada 10 fases
{
    int a[3], id[3]; // Vetor que guarda os ids dos itens
    float chance[5] = {0.0, 0.0, 0.0, 0.0, 0.0}; // Vetor que guarda as chances de raridade de item ser escolhido
    if(level <= 10) // Se o level for entre 0 e 10, a chance de um item de raridade 1 é 90% e 2 é 10%
    {
        chance[1] = 90.0;
        chance[2] = 10.0;
    }
    if(level >= 11 && level <= 30) 
    {
        chance[1] = 80.0;
        chance[2] = 20.0;
    }
    if(level >= 31 && level <= 40)
    {
        chance[1] = 60.0;
        chance[2] = 35.0;
        chance[3] = 5.0;
    }
    if(level >= 41 && level <= 50) 
    {
        chance[1] = 50.0;
        chance[2] = 40.0;
        chance[3] = 10.0;
        chance[4] = 1.0; // Chance de 1% de sair um item de raridade 4
    }
    if(level >=51 && level <=60) 
    {
        chance[1] = 35.0;
        chance[2] = 45.0;
        chance[3] = 20.0;
        chance[4] = 2.0;
    }
    if(level >= 61 && level <= 70) 
    {
        chance[1] = 17.0;
        chance[2] = 50.0;
        chance[3] = 30.0;
        chance[4] = 3.0;
    }
    if(level >= 71 && level <= 80) 
    {
        chance[1] = 6.0;
        chance[2] = 50.0;
        chance[3] = 40.0;
        chance[4] = 4.0;
    }
    if(level >= 81 && level <= 90) 
    {
        chance[1] = 5.0;
        chance[2] = 35.0;
        chance[3] = 55.0;
        chance[4] = 5.0;
    }
    if(level >= 91 && level <= 100) 
    {
        chance[2] = 10.0;
        chance[3] = 80.0;
        chance[4] = 10.0;
    }

    a[0] = aleatorizaChance(5, chance);
    a[1] = aleatorizaChance(5, chance);
    a[2] = aleatorizaChance(5, chance);


    if(level >= 91 && level <= 100) // Se o level for entre 91 e 100
    {
        a[1] = 4; // Força um item lendario
    }

    id[0] = idAleatorio(a[0]);
    id[1] = idAleatorio(a[1]);
    id[2] = idAleatorio(a[2]);

    int quantidade[3] = {0}, i, j;  // Tem que saber da quantidade para nao dar problema de repeticao quando for diferenciar os ids
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < items; j++) if(item[j].raridade == a[i]) quantidade[i]++;  // Conta quantos itens existem com a raridade escolhida
    }

    if((id[0] == id[1] || id[0] == id[2] || id[1] == id[2]) && (quantidade[0] >= 3 || quantidade[1] >= 3 || quantidade[2] >= 3)) // Se os ids forem iguais, gera novos ids
    {
        while(id[0] == id[1] || id[0] == id[2] || id[1] == id[2])
        {
            if(id[0] == id[1] && quantidade[1] >= 3) id[1] = idAleatorio(a[1]);
            if(id[0] == id[2] && quantidade[2] >= 3) id[2] = idAleatorio(a[2]);
            if(id[1] == id[2] && quantidade[2] >= 3) id[2] = idAleatorio(a[2]);
        }
    }
    player.exp = 50;
    calculoExp(player.exp, player.expMax, player.level);
    vitrine(id);
}

void setItens(int q){
    int i;
    if(q > MAX_ITEMS)
    {
        printf("Quantidade de itens invalida.\n");
        exit(1);
    }
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
    count = (count + 1) / 7;
    items = count;
    setItens(items);
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
    fprintf(arq, "Sala: %d\n", sala);
    fprintf(arq, "Level %d: %d/%d\n", player.level, player.exp, player.expMax);
    fprintf(arq, "Skill Points: %d\n", player.skillPoints);
    fprintf(arq, "HP: %d/%d\n", player.hp, player.vidaMax);
    fprintf(arq, "MP: %d/%d\n", player.mp, player.manaMax);
    fprintf(arq, "Forca: %d\n", player.forca);
    fprintf(arq, "Protecao: %d\n", player.protecao);
    fprintf(arq, "Agilidade: %d\n", player.agilidade);
    fprintf(arq, "Inteligencia: %d\n", player.inteligencia);
    fprintf(arq, "Carisma: %d\n", player.carisma);
    fprintf(arq, "Moedas: %d\n", player.moedas);
    fprintf(arq, "Equipados: %d, %d, %d, %d\n", player.equipado[0], player.equipado[1], player.equipado[2], player.equipado[3]);

    for (int i = 0; i < 20; i++) {
        fprintf(arq, "%d %d\n", player.inventario[0][i], player.inventario[1][i]);
    }

    fclose(arq);
}


void load(DADOS *player, int *sala) {
    FILE *arq = fopen("Dados do Jogo/save.txt", "rt");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fgets(player->nome, sizeof(player->nome), arq);
    player->nome[strcspn(player->nome, "\n")] = 0; //Transforma caractere "\n" em "\0"
    
    fscanf(arq, "Sala: %d\n", &sala);
    fscanf(arq, "Level %d: %d/%d\n", &player->level, &player->exp, &player->expMax);
    fscanf(arq, "Skill Points: %d\n", &player->skillPoints);
    fscanf(arq, "HP: %d/%d\n", &player->hp, &player->vidaMax);
    fscanf(arq, "MP: %d/%d\n", &player->mp, &player->manaMax);
    fscanf(arq, "Forca: %d\n", &player->forca);
    fscanf(arq, "Protecao: %d\n", &player->protecao);
    fscanf(arq, "Agilidade: %d\n", &player->agilidade);
    fscanf(arq, "Inteligencia: %d\n", &player->inteligencia);
    fscanf(arq, "Carisma: %d\n", &player->carisma);
    fscanf(arq, "Moedas: %d\n", &player->moedas);
    fscanf(arq, "Equipados: %d, %d, %d, %d\n", &player->equipado[0], &player->equipado[1], &player->equipado[2], &player->equipado[3]);

    for (int i = 0; i < 20; i++) {
        fscanf(arq, "%d %d\n", &player->inventario[0][i], &player->inventario[1][i]);
    }

    fclose(arq);
}


int numAle(int range){
    //Gera um numero de 1 a range.
    int n = (rand()%range)+1;
    
    return n;
}

void calculoExp()
{
    while(player.exp >= player.expMax) //posteriormente esse if será tirado, vamos colocar ele na condição de ganhar uma batalha
    {
        player.exp -= player.expMax;
        player.expMax*=1.5;
        player.level++;
        aleatStatus(15);
    }
}

void aleatStatus(int pontos)
{
    int status[5] = {0};
    for (int i = 0 ; i < pontos ; i++)
    {
        status[numAle(5)-1]++;
    }

    player.protecao += status[0];
    player.forca += status[1];
    player.agilidade += status[2];
    player.inteligencia += status[3];
    player.carisma += status[4];
    player.vidaMax = player.protecao * 5;
    player.hp += status[0] * 5;
    player.manaMax = player.inteligencia * 5;
    player.mp += status[3] * 5;
}


void aleatJogador(char *txt){
    gerarPasta();
    limparTerminal();

    //no total são 30 pontos de status, 20 RNG e 10 fixos
    player.protecao = 2;
    player.forca = 2;
    player.agilidade = 2;
    player.inteligencia = 2;
    player.carisma = 2;
    player.vidaMax = player.protecao * 5;
    player.hp = player.vidaMax;
    player.manaMax = player.inteligencia * 5;
    player.mp = player.manaMax;

    player.level = 1;
    player.expMax = 50;
    int i, j;

    aleatStatus(20);
    

    txt[strcspn(txt, "\n")] = 0; // Tira o caractere "\n" da string txt e coloca "\0"

    strcpy(player.nome, txt);
   
    player.moedas = 1000;

    // Adicione esta inicialização para o inventário:
    for (i = 0; i < 2; i++) {
        for(j = 0; j < 20; j++){
            player.inventario[i][j] = 0; // Inicializa cada slot do inventário com 0
        }
    }

    for(i = 0; i < 4; i++) {
        player.equipado[i] = 0; // Inicializa os itens equipados com 0
    }
    
    player.level = 1;
    player.exp = 0;
    player.expMax = 50;

    sala = 0; // Inicializa a sala como 0

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

void strip(char *nome) {
    int start = 0, end = strlen(nome) - 1, i;
    while (isspace((unsigned char)nome[start])) 
        start++;
    while (end >= start && isspace((unsigned char)nome[end])) 
        end--;
    for (i = 0; start <= end; start++, i++) 
        nome[i] = nome[start];
    nome[i] = '\0';
}

void verifica_nome_player(char *nome) {
    int i = 0, OK = 0; // OK: verifica se o nome original sofreu alguma alteração
    char nome_corrigido[101] = {0}, nome_novo[101] = {0}; 
    // nome_corrigido = altera o nome (sem os casos não permitidos)
    // nome_novo = pede um novo nome para a recursão
    limparTerminal();
    while (nome[i] != '\0') {
        if (nome[i] == ' ') { // + de uma palavra
            OK = 1;
            for(i = 0; nome[i] != ' '; i++) 
                nome_corrigido[i] = nome[i]; // copia a string até o caractere de espaço para nome_corrigido
            nome_corrigido[i] = '\0';
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
            strip(nome_novo);
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
        printf(" ");// Imprime o nome do jogador com um espaço no final
        textoTela(".  .  .  \n", 200);
        textoTela("Um verdadeiro nome de guerreiro . . .\n\n", 400);
    }
    aleatJogador(player.nome); // player.nome vai para a função
}

void histInic(){
    char nome[101] = {0};
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
    fgets(nome, 100, stdin);

    //história comentada para ser mais rápido.

    limparTerminal();
    textoTela("Mas todo destino grandioso comeca com passos pequenos.\n", 200);
    textoTela("E hoje . . .\n", 500);
    textoTela("Errrn . . . Hoje! . . .\n", 500); */
    textoTela("Desculpe, mas qual seu nome mesmo?\n\n", 300);
    getchar();
    int OK = 1;
    while (OK) {  
        fgets(nome, 100, stdin);
        if (nome[0] == '\n') {
            strcpy(nome, ""); // reseta a string
            limparTerminal();
            textoTela("Nao quer falar seu nome??", 400);
            textoTela("Isso e falta de educacao, sabia?", 200);
            textoTela("Fala um nome qualquer entao, so pra eu registrar aqui:\n\n", 300);
        }
        else OK = 0; // nome foi digitado 
    }
    strip(nome);
    verifica_nome_player(nome);
    
    // movi essa parte pra função verifica_nome_player()
    textoTela("Quer comprar algo na minha loja?", 400); 
    printf("\n(Pressione ENTER para continuar...)\n");
    getchar(); // Espera o usuário pressionar ENTER
    limparTerminal();
    loja(0);
    mostrarStatus();
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
            histInic();
            break;
        case '2':
            load(&player, &sala);
            mostrarStatus();
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
    readItems();
    telaInicial();
    return 0;
}
