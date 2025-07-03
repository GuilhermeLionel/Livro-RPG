#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>   // mkdir for Windows
#else
#include <unistd.h> // usleep
#include <sys/stat.h> // mkdir
#endif

#define MAX_ITEMS 100 // Define o número máximo de itens

int items = 0;
 
typedef struct buffHandler2 {
    int tipo; // 0 = Vazio, 1 = Ataque, 2 = Inteligencia, 3 = Protecao, 4 = Agilidade, 5 - Dano por turno, 6 - Cura por turno    qualquer outro alem é caso especial
    int alvo; // 0 = Inimigo, 1 = O propio, 2 = Variavel
    int duracao; // Duração do buff em turnos // 0 se for para sempre
    int valor; // Valor do buff, aumenta em estados, se for 1, aumenta o status em uma porcentagem, se 2, aumenta em 2x porcentagem
    //Utilizar o golpe mais vezes aumenta a porcentagem em quantidade (2 vezes uma habilidae de 1x porcentagem é igual a 2x porcentagem)
    int chance; // Chance de o buff ser aplicado (entre 0 e 100)
} BUFFHANDLER;

typedef struct abilityHandler2 {
    char nome[51];
    char descricao[201];
    int tipo; // 0 = Vazio, 1 = Ataque, 2 = Dano fixo, 3 = Cura percentual, 4 = Cura Fixa, 5 = Prot, 6 = Buff/Debuff
    int efeitoSecundario; // 0 = Nenhum, 1 = Queimar, 2 = Estunar, 3 = Envenenar, 4 = Paralizar, 5 = Desarmar, 6 = Dano continuo (vai funcionar como status (status 5))
    int chanceDeEfeito; // Chance de o efeito acontecer em uso (chance entre 0 e 100)
    BUFFHANDLER buff; // Buff que a habilidade pode causar
    float qtdmg; // Dano ou cura (Para cura percentual, este valor é um float entre 0 e 1)
    int custo; // Custo de mana
    int chance; // Chance de acerto da habilidade
    int status; // Status que a habilidade vai usar para o calculo
    // 0 = Nenhum, 1 = Forca, 2 = Inteligencia, 3 = Protecao, 4 = Agilidade, 5 = Carisma
    // Cura percentual nao importa o status e a fixa usa a inteligencia
    // Buffs e debuffs podem utilizar os status para aumentar as suas chances de acerto e de efeito secundario
} HABILIDADE;

HABILIDADE h[4]; // vetor de habilidades do personagem

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
    int hpMax;
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
    int habilidades[4]; // Habilidades que o jogador possui
    int buffs[3][15]; // Buffs que o jogador possui    [0] = Id do buff, [1] = Qtd, [2] = Turnos restantes
    int efeito; // 0 = Normal, 1 = Queimando, 2 = Envenenado, 3 = Atordoado, 4 = Paralisado, 5 = Desarmado
} DADOS;

DADOS player;

typedef struct ranking2 {
    char nome[41];
    int level;
    int pontuacao;
} RANKING;

RANKING jogador;

int sala = 0;


int idAleatorio(int raridade); // Gera um id aleatorio dado a raridade do item
int aleatorizaChance(int tamanho, float chance[tamanho]); // Dado o tamanho do vetor chance, rola um dado de 0 ate o tamanho do vetor menos - 1 e retorna o valor sorteado considerando as chances de cada valor
int localInv(int id); // Retorna a posição mais proxima de zero de um item com determinado id no inventário
int qtdInv(int id); // Retorna a quantidade do item no inventário
int numAle(int range);
int espacoInv(int id); // Retorna o espaco em que um item pode ser alocado no inventario
int digitos(int n); // Retorna a quantidade de dígitos do número
int salaAleatoria();
int dificuldadeAleatoria();
int raridadeAleatoria();
int bonusAplicado(int n);
int statusRequisitado(int status, DADOS usuario);
int quantosBuffs(int sinal);
float buffEfetivo(DADOS usuario, int status); // Retorna o percentual de alteração no status do usuário, considerando os buffs ativos


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
void loja();
void histInic();
void telaInicial();
void tipoItem(char *tipo, int n);
void addItem(int id);
void strip(char *nome);
void verificaNomePlayer(char *nome);
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
void calculoExp();
void aleatStatus(int pontos);
void tomadaDecisao();
void checkInput(int * n, int min, int max);
void cura(DADOS *usuario, int qtd);
void ranking();
void atualizaRanking(char *nomeArquivo, RANKING player);
void inimigoAleatorio(DADOS *inimigo, int dificuldade);
void batalharInimigo(DADOS *inimigo, int qtd);
void hpInimigo(DADOS inimigo, int modo, int num);
void moveCursor(int x, int y);
void usarHabilidade(DADOS *atacante, DADOS *defensor, HABILIDADE habilidade);
void getHabilidade(HABILIDADE *habilidade, int id);
void mana(int qtd);
void mostrarBuffs();
void inimigoMorreu(DADOS * inimigo, int n, int *qtd);
void salaDoPanico(DADOS *inimigo, int qtd);
void aplicaBuff(DADOS *usuario, BUFFHANDLER buff);
void aplicaEfeito(DADOS *alvo, int efeito);
void calculaPontuacao(DADOS *player, RANKING jogador, int sala);
int somaItens(DADOS *player);
void limparAte(int q, int x);
void telaInicial();

int sumItens(DADOS *player) {
    int i, j, soma = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 20; j++) {
            switch(item[player->inventario[i][j]].raridade) {
                case 1:
                    soma++;
                    break;
                case 2:
                    soma += 15;
                    break;                    
                case 3:
                    soma += 30;
                    break;                    
                case 4:
                    soma += 50;
                    break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        switch(item[player->equipado[i]].raridade) {
            case 1:
                soma++;
                break;
            case 2:
                soma += 15;
                break;                    
            case 3:
                soma += 30;
                break;                    
            case 4:
                soma += 50;
                break;
        }
    }
    return soma;
}

int bossesF(int sala) {
    int quantidade = 0, r = sala % 10;
    quantidade = (sala - r) / 10;
    int soma = (50 + 50 * quantidade) * quantidade;
    return soma;
}

void calculaPontuacao(DADOS *player, RANKING jogador, int sala) {
    int bossesQuant = 0, XPTotal;
    strcpy(jogador.nome, player->nome);
    jogador.level = player->level;
    bossesQuant = bossesF(sala);
    XPTotal = 100 * (((int) pow(1.5, player->level - 1)) - 1) + player->exp;
    // cálculo final: 
    jogador.pontuacao = 0.0018 * XPTotal + 1.75 * sumItens(player) + bossesQuant + 20*sala;
    printf("\nPontuacao final: %d\n", jogador.pontuacao); 
    atualizaRanking("Dados-do-Jogo/pontuacao.txt", jogador); 
    printf("Pressione [ENTER] para sair\n");
    getchar();
    telaInicial();
}

void limparAte(int q, int x)
{
    int i, j;
    for(i = 0; i <= x; i++)
    {
        printf("\033[1A");
        printf("\033[%dX", q);
    }
}

void aplicaEfeito(DADOS *alvo, int efeito)
{
    if(alvo->efeito == 0)
    {
        alvo->efeito = efeito; // Aplica o efeito no usuário
        printf("%s ", alvo->nome);
        switch(efeito)
        {
            case 1:
                printf("esta queimando!");
                break;
            case 2:
                printf("foi envenenado!");
                break;
            case 3:
                printf("foi atordoado!");
                break;
            case 4:
                printf("esta paralisado!");
                break;
            case 5:
                printf("foi desarmado!");
                break;
            case 6:
                printf("esta sangrando!");
                break;
        }
        printf("\n\n");
        printf("(Pressione [ENTER] para continuar...)\n");
        limparBuffer();
    }
}

void aplicaBuff(DADOS *usuario, BUFFHANDLER buff)
{
    // Aplica o buff no usuário / alvo
    int i;
    for(i = 0; i < 15; i++)
    {
        if(usuario->buffs[0][i] == 0) // Se o buff estiver vazio
        {
            printf("A ");
            switch(buff.tipo)
            {
                case 1:
                    printf("Forca");
                    break;
                case 2:
                    printf("Inteligencia");
                    break;
                case 3:
                    printf("Protecao");
                    break;
                case 4:
                    printf("Agilidade");
                    break;
                default:
                    printf("buff desconhecido");
                    return; // Se o tipo do buff for inválido, não aplica
            }
            printf(" de %s foi ", usuario->nome);
            if(buff.valor > 0) printf("aumentada!");
            else if(buff.valor < 0) printf("diminuida!");

            usuario->buffs[0][i] = buff.tipo; // Tipo do buff
            usuario->buffs[1][i] = buff.valor; // Valor do buff
            usuario->buffs[2][i] = buff.duracao; // Duração do buff
            return;
        }
    }
    if(i == 15) printf("%s atingiu o limite maximo de buffs!\n", usuario->nome);
}

void hpPlayer()
{
    int espacos, resto;
    int largura = 50;
    int barsize = 30;
    int i;
    char txt[41];
    moveCursor(65, 11);
    printf("%c", 218);
    sprintf(txt, "%s  ", player.nome);
    espacos = largura - strlen(txt) - 2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s%c", txt, 191);
    moveCursor(65, 12);
    printf("  ");
    sprintf(txt, "HP: %d / %d", player.hp, player.hpMax);
    espacos = ((float)player.hp / (float)player.hpMax) * barsize;
    if(espacos == 0 && player.hp != 0) espacos = 1;
    resto = barsize - espacos;
    for(i = 0; i < espacos; i++) printf("\033[31m=\033[0m");
    for(i = 0; i < resto; i++) printf("-");
    espacos = largura - barsize - 4 - strlen(txt);
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s  ", txt);
    moveCursor(65, 13);
    printf("  ");
    sprintf(txt, "MP: %d / %d", player.mp, player.manaMax);
    espacos = ((float)player.mp / (float)player.manaMax) * barsize;
    if(espacos == 0 && player.mp != 0) espacos = 1;
    resto = barsize - espacos;
    for(i = 0; i < espacos; i++) printf("\033[34m=\033[0m");
    for(i = 0; i < resto; i++) printf("-");
    espacos = largura - barsize - 4 - strlen(txt);
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s  ", txt);
    moveCursor(65, 14);
    printf("%c", 192);
    for(i = 0; i < largura - 2; i++) printf(" ");
    printf("%c", 217);
}

float buffEfetivo(DADOS usuario, int status)
{
    // Retorna o percentual de alteração no status do usuário, considerando os buffs ativos
    int i;
    float result, a = 2, b = 2;
    int total = 0;
    for(i = 0; i < 15; i++)
    {
        if(usuario.buffs[0][i] == status) // Se o buff for do tipo requisitado
        {
            total += usuario.buffs[1][i];
        }
    }
    if(total > 0) a += total;
    else if(total < 0) b += total; // Se for negativo, diminui o valor de b
    result = a/b;
    return result;
}

int quantosBuffs(int sinal)
{   
    //retorna a quantidade de buff se sinal > 0 e debuffs se < 0
    int i = 0;
    int result = 0;
    if(sinal > 0) for(i = 0; i < 15; i ++)
        {
            if(player.buffs[1][i] > 0) result++;
        }
    else for(i = 0; i < 15; i ++)
        {
            if(player.buffs[1][i] < 0) result++;
        }
    return result;
}

void mana(int qtd)
{
    player.mp += qtd;
    if(player.mp > player.manaMax) player.mp = player.manaMax;
}

void getHabilidade(HABILIDADE *habilidade, int id)
{
    FILE *fp;
    fp = fopen("Dados-do-Jogo/habilidades.txt", "r");
    int i, j;
    for(i = 0; i < id; i++)
    {
        for(j = 0; j < 13; j++) fgets(habilidade->descricao, 200, fp); // Pula as linhas até chegar na habilidade desejada
    }
    fgets(habilidade->nome, 50, fp);
    fgets(habilidade->descricao, 200, fp);
    fscanf(fp, "Tipo: %d\n", &habilidade->tipo);
    fscanf(fp, "Efeito: %d %d%%\n", &habilidade->efeitoSecundario, &habilidade->chanceDeEfeito);
    fscanf(fp, "Buff: %d %d%%\n", &habilidade->buff.tipo, &habilidade->buff.chance);
    fscanf(fp, "Buff Alvo: %d\n", &habilidade->buff.alvo);
    fscanf(fp, "Buff Duracao: %d\n", &habilidade->buff.duracao);
    fscanf(fp, "Quant Buff: %d\n", &habilidade->buff.valor);
    fscanf(fp, "Qtd: %f\n", &habilidade->qtdmg);
    fscanf(fp, "Custo: %d\n", &habilidade->custo);
    fscanf(fp, "Chance: %d\n", &habilidade->chance);
    fscanf(fp, "Status: %d\n\n", &habilidade->status);
    fclose(fp);
    habilidade->nome[strcspn(habilidade->nome, "\n")] = 0; // Remove o '\n' do final da string
}

int statusRequisitado(int status, DADOS usuario)
{
    switch(status)
    {
        case 0: // Nenhum
            return 0;
        case 1: // Forca
            return usuario.forca * buffEfetivo(usuario, 1);
        case 2: // Inteligencia
            return usuario.inteligencia * buffEfetivo(usuario, 2);
        case 3: // Agilidade
            return usuario.agilidade * buffEfetivo(usuario, 3);
        case 4: // Protecao
            return usuario.protecao * buffEfetivo(usuario, 4);
        case 5: // Carisma
            return usuario.carisma * buffEfetivo(usuario, 5);
        default:
            return 0; // Caso não seja nenhum dos status válidos, retorna 0
    }
}

void ataque(DADOS *atacante, DADOS *defensor)
{
    limparTerminal();
    int dano;
    int a;
    a = (float) (statusRequisitado(1, *atacante) * (3.0/4.0) + 1.0);
    dano = statusRequisitado(1, *atacante) - (numAle(a) - 1);
    if(atacante == &player) dano += bonusAplicado(8); // Adiciona o bônus de dano do jogador
    printf("%s atacou %s e desferiu %d de dano!\n\n", atacante->nome, defensor->nome, dano);
    a = (float) (statusRequisitado(3, *defensor) * (3.0/4.0) + 1.0);
    dano -= (statusRequisitado(3, *defensor) - (numAle(a) - 1));
    if(dano < 0) dano = 0; // Se o dano for negativo, não causa dano
    if(atacante == &player) dano += 2; // Se o atacante for o jogador, ele recebe 2 de dano fixo
    else dano += 1; // Se o atacante for um inimigo, recebe 1 de dano fixo
    defensor->hp -= dano; // Adiciona 2 de dano fixo
    if(defensor->hp < 0) defensor->hp = 0; // Se o HP do defensor ficar negativo, zera
    printf("(Pressione [ENTER] para continuar...)\n");
    limparBuffer();
}


void mostrarBuffs()
{
    int i;
    int x = 50, y = 0;
    int qtd = 0;
    for(i = 0; i < 15; i++) 
    {
        if(player.buffs[0][i] != 0)
        {
            qtd++;
            if(player.buffs[1][i] > 0 && player.buffs[0][i] < 6)printf("+ %d ", player.buffs[1][i]);
            else if(player.buffs[1][i] < 0) printf("- %d ", -player.buffs[1][i]);
            switch(player.buffs[0][i])
            {
                case 1:
                    printf("Forca");
                    break;
                case 2:
                    printf("Inteligencia");
                    break;
                case 3:
                    printf("Protecao");
                    break;
                case 4:
                    printf("Agilidade");
                    break;
                default:
                    break;
            }
            if(player.buffs[2][i] < 0) printf(" | - ");
            else printf(" | %d turnos restantes", player.buffs[2][i]);
            if(qtd <= 8)printf("\n\n");
            if(qtd > 8)
            {
                moveCursor(x, y);
                y += 2;
            }
        }
    }
    if(qtd <= 8) moveCursor(0, qtd * 2 + 3);
    else moveCursor(0, 19);
    printf("Total:");
    float buff[5];
    buff[0] = buffEfetivo(player, 1);
    buff[1] = buffEfetivo(player, 2);
    buff[2] = buffEfetivo(player, 3);
    buff[3] = buffEfetivo(player, 4);
    buff[4] = buffEfetivo(player, 5);

    printf("Forca: %.1f X | Inteligencia: %.1f X | Protecao: %.1f X | Agilidade: %.1f X | Carisma: %.1f X\n\n", buff[0], buff[1], buff[2], buff[3], buff[4]);

    printf("(Pressione [ENTER] para continuar...)\n");
    limparBuffer();
}

void inimigoMorreu(DADOS * inimigo, int n, int *qtd)
{
    int i;
    limparTerminal();
    printf("%s foi derrotado!\n\n", inimigo[n].nome);
    player.exp += inimigo[n].exp;
    printf("Voce ganhou %d de experiencia!\n\n", inimigo[n].exp);
    calculoExp();
    printf("(Pressione [ENTER] para continuar...)\n");
    limparBuffer();
    if(*qtd > 1)
    {
        for(i = n; i < *qtd - 1; i++)
        {
            inimigo[i] = inimigo[i + 1]; // Remove o inimigo derrotado do array
        }
        (*qtd)--; // Decrementa a quantidade de inimigos
    }
}

void usarHabilidade(DADOS *atacante, DADOS *defensor, HABILIDADE habilidade)
{
    int statusAtk = 0, statusDefesa = 0;
    int hit1 = 0, hit2 = 0, hit3 = 0;
    int dado;
    DADOS *a;
    dado = numAle(101) - 1;
    if(dado <= habilidade.chance) hit1 = 1; // Verifica se a habilidade acerta
    if(hit1 == 1)
    {
        dado = numAle(101) - 1;
        if(dado <= habilidade.chanceDeEfeito) hit2 = 1;
        dado = numAle(101) - 1;
        if(dado <= habilidade.buff.chance) hit3 = 1;
    }
    else
    {
        printf("%s tentou usar %s. . . \n\n Mas errou!\n\n", atacante->nome, habilidade.nome);
        printf("(Pressione [ENTER] para continuar...)\n");
        limparBuffer();
        return; // Se a habilidade não acerta, não faz nada
    }
    float dano;
    int qtd = 0;
    switch(habilidade.tipo)  // Para o efeito principal da habilidade
    {
        case 1: // Ataque
            statusAtk = statusRequisitado(habilidade.status, *atacante);
            statusDefesa = statusRequisitado(4, *defensor);
            dano = (habilidade.qtdmg) * (statusAtk / statusDefesa) + 2.0;
            break;
        case 2: // Dano fixo
            if (habilidade.qtdmg == 0) dano = 0;
            else dano = habilidade.qtdmg - defensor->protecao;
            defensor->hp -= dano;  
            break;
        case 3: // Cura percentual
            dano = (float) (habilidade.qtdmg * atacante->hpMax);
            qtd = dano;
            cura(atacante, qtd);
            break;
        case 4: // Cura fixa
            dano = habilidade.qtdmg * atacante->inteligencia / 40.0;
            cura(atacante, dano);
            break;
        case 5: // Protecao
            break;
        case 6: // Buff / Debuff
            if(habilidade.buff.alvo == 0) a = defensor; // Se o alvo for o inimigo, aplica o buff no inimigo
            else if(habilidade.buff.alvo == 1) a = atacante; // Se o alvo for o atacante, aplica o buff no atacante
            dado = numAle(101) - 1;
            statusAtk = statusRequisitado(habilidade.status, *atacante) * (1.0 / 5.0);
            if(dado <= habilidade.buff.chance + statusAtk) hit3 = 1; // Verifica se o buff é aplicado
            break;
        default:
            break;
    }
    if(hit2)
    {
        aplicaEfeito(defensor, habilidade.efeitoSecundario);
        if(hit3 && habilidade.tipo != 6) limparTerminal();
    }
    if(hit3 && habilidade.tipo != 6)
    {
        if(habilidade.buff.alvo == 0) a = defensor; // Se o alvo for o inimigo, aplica o buff no inimigo
        else if(habilidade.buff.alvo == 1) a = atacante; // Se o alvo for o atacante, aplica o buff no atacante
        aplicaBuff(a, habilidade.buff);
    }
}

int bonusAplicado(int n)
{
    int id[4];
    int i, j;
    int result = 0;
    for(i = 0; i < 4; i++) id[i] = player.equipado[i];
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 3; j++) if(item[id[i]].bonus[0][j] == n) result += item[id[i]].bonus[1][j];
    }
    return result;
}

int raridadeAleatoria()
{
    float chance[5] = {0.0, 0.0, 0.0, 0.0, 0.0}; // Vetor que guarda as chances de raridade de item ser escolhido
    float peso1 = 0.05, peso2 = 0.15, peso3 = 0.5, peso4 = 2.0;
    if(sala <= 10) // Se o sala for entre 0 e 10, a chance de um item de raridade 1 é 90% e 2 é 10%
    {
        chance[1] = 90.0 + player.carisma * peso1;
        chance[2] = 10.0 + player.carisma * peso2;
    }
    if(sala >= 11 && sala <= 30) 
    {
        chance[1] = 80.0 + player.carisma * peso1;
        chance[2] = 20.0 + player.carisma * peso2;
    }
    if(sala >= 31 && sala <= 40)
    {
        chance[1] = 60.0 + player.carisma * peso1;
        chance[2] = 35.0 + player.carisma * peso2;
        chance[3] = 5.0 + player.carisma * peso3;
    }
    if(sala >= 41 && sala <= 50) 
    {
        chance[1] = 50.0 + player.carisma * peso1;
        chance[2] = 40.0 + player.carisma * peso2;
        chance[3] = 10.0 + player.carisma * peso3;
        chance[4] = 1.0 + player.carisma * peso4; // Chance de 1% de sair um item de raridade 4
    }
    if(sala >=51 && sala <= 60) 
    {
        chance[1] = 33.0 + player.carisma * peso1;
        chance[2] = 45.0 + player.carisma * peso2;
        chance[3] = 20.0 + player.carisma * peso3;
        chance[4] = 2.0 + player.carisma * peso4;
    }
    if(sala >= 61 && sala <= 70) 
    {
        chance[1] = 17.0 + player.carisma * peso1;
        chance[2] = 50.0 + player.carisma * peso2;
        chance[3] = 30.0 + player.carisma * peso3;
        chance[4] = 3.0 + player.carisma * peso4;
    }
    if(sala >= 71 && sala <= 80) 
    {
        chance[1] = 6.0 + player.carisma * peso1;
        chance[2] = 50.0 + player.carisma * peso2;
        chance[3] = 40.0 + player.carisma * peso3;
        chance[4] = 4.0 + player.carisma * peso4;
    }
    if(sala >= 81 && sala <= 90) 
    {
        chance[1] = 5.0 + player.carisma * peso1;
        chance[2] = 35.0 + player.carisma * peso2;
        chance[3] = 55.0 + player.carisma * peso3;
        chance[4] = 5.0 + player.carisma * peso4;
    }
    if(sala >= 91 && sala <= 100) 
    {
        chance[2] = 10.0;
        chance[3] = 80.0;
        chance[4] = 10.0;
    }

    return aleatorizaChance(5, chance);
}

void moveCursor(int x, int y) 
{
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coord = {x, y};
        SetConsoleCursorPosition(hConsole, coord);
    #else
        printf("\033[%d;%dH", y, x);
        fflush(stdout);
    #endif
}

void hpInimigo(DADOS inimigo, int modo, int num)
{
    int largura = 30;
    int espacos, resto;
    int x, y;
    x = (num - 1) * 30 + 5 * num;
    y = (num - 1) * 2;
    char txt[41];
    int i;
    moveCursor(x, y);
    if(modo == 1) sprintf(txt, "%d - %s", num, inimigo.nome);
    else sprintf(txt, "%s", inimigo.nome);
    printf("%s", txt);
    moveCursor(x, y + 1);

    espacos = largura - 2;
    printf("%c", 218);
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%c", 191);
    moveCursor(x, y + 2);

    float media = (float)inimigo.hp / (float)inimigo.hpMax;
    if(media > 1.0) media = 1.0;
    espacos = media * (largura - 4);
    if(espacos == 0 && media > 0.0) espacos = 1;
    resto = largura - espacos - 4;
    printf("  ");
    for(i = 0; i < espacos; i++) printf("\033[31m=\033[0m");
    for(i = 0; i < resto; i++) printf("-");
    printf("  ");
    moveCursor(x, y + 3);


    printf("%c", 192);
    sprintf(txt, "%d/%d  ", inimigo.hp, inimigo.hpMax);
    espacos = largura - 2 - strlen(txt);
    if(media <= 0.3) sprintf(txt, "\033[31m%d\033[0m/%d  ", inimigo.hp, inimigo.hpMax);
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s", txt);
    printf("%c", 217);
}

void salaDoPanico(DADOS *inimigo, int qtd) {
    limparTerminal();
    textoTela("O que voce ira fazer?\n", 300);
    printf("[1] Desistir  [2] Retornar a batalha  [3] Abrir inventario  [4] Correr para o proximo andar\n\n");
    int escolha, OK = 1;
    checkInput(&escolha, 1, 4);
    switch(escolha) {
        case 1:
            limparTerminal();
            textoTela("Voce resolve fugir de maneira covarde.", 400);
            textoTela("Sua aventura termina aqui...", 500);
            calculaPontuacao(&player, jogador, sala);
            break;
        case 2:
            batalharInimigo(inimigo, qtd);
            break;
        case 3:
            verInventario(1);
            salaDoPanico(inimigo, qtd);
            break;
        case 4:
            limparTerminal();
            for (int i = 0; i < qtd; i++) 
                if (numAle(player.agilidade) <= numAle(inimigo[i].agilidade) * 1.3) OK = 0;
            if (OK) {
                sala++;
                tomadaDecisao();
            }
            else {
                printf("Os inimigos te alcancaram.\nResista e lute, seu covarde!\n\n");
                printf("(Pressione [ENTER] para continuar...)\n");
                getchar();
                batalharInimigo(inimigo, qtd);
            }
            break;
    }
}

void batalharInimigo(DADOS *inimigo, int qtd)
{
    limparTerminal();
    int b1 = quantosBuffs(1);
    int b2 = quantosBuffs(-1);
    if(qtd == 0)
    {
        sala++;
        tomadaDecisao();
    }
    int i;
    for(i = 0; i < qtd; i++)
    {
        hpInimigo(inimigo[i], 0, i + 1);
    }
    hpPlayer();
    moveCursor(0, 9);
    for(i = 0; i < 5 + 35 * qtd; i++) printf("=");
    printf("\n\nO que voce deseja fazer?\n\n");
    printf("[1] Atacar  [2] Habilidade  [3] Item  [4] Fugir  ");
    if(b1 || b2) 
    {
        printf("[5] ");
        if(b1) printf("Buff");
        if(b1 > 1) printf("s");
        if(b1 && b2) printf(" / ");
        if(b2) printf("Debuff");
        if(b2 > 1) printf("s");
        printf(" (");
        if(b1 != 0) printf("\033[32m%d\033[0m", b1);
        if(b1 != 0 && b2 != 0) printf(" / ");
        if(b2 != 0) printf("\033[31m%d\033[0m", b2);
        printf(")");
    }
    printf("\n\n");
    int escolha, OK = 1;
    if(b1 || b2) checkInput(&escolha, 1, 7);
    else checkInput(&escolha, 1, 7);
    limparAte(60, 3);
    moveCursor(0, 16);
    limparAte(60, 5);
    moveCursor(0, 11);
    switch(escolha)
    {
        case 1:
            for(i = 0; i < qtd; i++) hpInimigo(inimigo[i], 1, i + 1);
            moveCursor(0, 11);
            printf("Qual o inimigo que deseja atacar?\n\n");
            int inimigoEscolhido;
            checkInput(&inimigoEscolhido, 1, qtd);
            ataque(&player, &inimigo[inimigoEscolhido - 1]);
            if(inimigo[inimigoEscolhido - 1].hp <= 0) inimigoMorreu(inimigo, inimigoEscolhido - 1, &qtd);
            batalharInimigo(inimigo, qtd);
            break;
        case 2: 
            for (i = 0; i < 4; i++) {
                getHabilidade(&h[i], player.habilidades[i]);
                printf("Habilidade %d: %s\n\n", i + 1, h[i].nome);
            }
            printf("[1] Usar 1  [2] Usar 2  [3] Usar 3  [4] Usar 4  \n\n[0] Sair\n\n\n");
            int escolha;
            checkInput(&escolha, 0, 4);
            while(h[escolha].tipo == 0 && escolha != 0)
            {
                limparLinhas(1);
                moveCursor(0, 23);
                limparLinhas(2);
                moveCursor(0, 21);
                printf("Habilidade invalida, escolha outra.\n\n");
                checkInput(&escolha, 0, 4);
            }
            if (escolha != 0) 
            {
                usarHabilidade(&player, inimigo, h[escolha]);
            } 
            else batalharInimigo(inimigo, qtd);
            break;
        case 3:
            verInventario(3);
            batalharInimigo(inimigo, qtd);
            break;
        case 4:
            for (i = 0; i < qtd; i++) 
                if (numAle(player.agilidade) <= numAle(inimigo[i].agilidade)) OK = 0;
            if (OK) {
                limparTerminal();
                textoTela("...", 500);
                textoTela("Voce conseguiu escapar...", 300);
                textoTela("Por muito pouco...", 400);
                salaDoPanico(inimigo, qtd);
            }
            else {
                printf("Os inimigos te alcancaram.\nResista e lute!\n\n");
                printf("(Pressione [ENTER] para continuar...)\n");
                limparBuffer();
                batalharInimigo(inimigo, qtd);
            }
            break;
        case 5:
            limparTerminal();
            mostrarBuffs();
            batalharInimigo(inimigo, qtd);
            break;
        case 6:
            for(i = 0; player.buffs[0][i]; i++);
            player.buffs[0][i] = 1;
            player.buffs[1][i] = 2;
            player.buffs[2][i] = 0;
            batalharInimigo(inimigo, qtd);
            break;
        case 7:
            for(i = 0; player.buffs[0][i]; i++);
            player.buffs[0][i] = 1;
            player.buffs[1][i] = -2;
            player.buffs[2][i] = 0;
            batalharInimigo(inimigo, qtd);
            break;
    }
}

void cura(DADOS *usuario, int qtd)
{
    usuario->hp += qtd;
    if(usuario->hp > usuario->hpMax) usuario->hp = usuario->hpMax;
}

void ranking()
{
    int largura = 50;
    int espacos, resto;
    char txt[55], nome[51];
    int i, j;
    int numero;
    FILE *fp;
    fp = fopen("Dados-do-Jogo/pontuacao.txt", "rt");
    if(fp == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    printf("Ranking\n\n");
    for(i = 0; i < largura; i++) printf(" ");

    espacos = largura - 30 - strlen("Level:") / 2;
    resto = largura - 30 - strlen("Level:") % 2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("Level:");
    for(i = 0; i < espacos + resto; i++) printf(" ");

    espacos = largura - 20 - strlen("Pontos:") / 2;
    resto = largura - 20 - strlen("Pontos:") % 2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("Pontos:");
    printf("\n");
    for(i = 1; i <= 8; i++)
    {
        fscanf(fp, " %d - %s ", &numero, nome);
        sprintf(txt, "%d - %s", numero, nome);
        printf("%s", txt);
        espacos = largura - strlen(txt);
        for(j = 0; j < espacos; j++) printf(" ");


        fscanf(fp, "- lvl: %d ", &numero);
        sprintf(txt, "%d", numero);
        espacos = largura - 30 - strlen(txt) / 2;
        resto = largura - 30 - strlen(txt) % 2;
        for(j = 0; j < espacos; j++) printf(" ");
        printf("%s", txt);
        for(j = 0; j < espacos + resto; j++) printf(" ");


        fscanf(fp, "- %d\n", &numero);
        sprintf(txt, "- = %d = -", numero);
        espacos = largura - 20 - strlen(txt) / 2;
        for(j = 0; j < espacos; j++) printf(" ");
        printf("%s", txt);
        printf("\n");
    }
    printf("\n");
    fclose(fp);
}


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

int loja0[3], checkLoja0 = 1;
void lojinhaInicial()
{
    loja0[0] = idAleatorio(1);
    loja0[1] = idAleatorio(1);
    loja0[2] = idAleatorio(1);
    while(1)
    {
        if(loja0[0] == loja0[1]) loja0[1] = idAleatorio(1);
        else if(loja0[0] == loja0[2]) loja0[2] = idAleatorio(1);
        else if(loja0[1] == loja0[2]) loja0[2] = idAleatorio(1);
        else break;
    }
    checkLoja0 = 0;
    vitrine(loja0);
}

void imprimeDialogo(char *nomeArquivo, int min, int max) 
{
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

void tomadaDecisao()
{
    char txt[400];
    limparTerminal();
    int escolha;

    if(sala == 0)
    {
        textoTela("Qual sera o seu primeiro passo?\n", 300);
        printf("[1] Loja\n[2] Conversar\n[3] Subir a torre\n\n");
        checkInput(&escolha, 1, 3);
        switch(escolha)
        {
            case 1:
                if(checkLoja0) lojinhaInicial();
                else vitrine(loja0);
                tomadaDecisao();
                break;
            case 2:
                //Dialogo de conversa
                limparTerminal();
                textoTela("Eu sou o vendedor mais confiavel da regiao, inclusive sou o unico, meu proposito na vida sempre foi engana... ajudar bravos guerreiros.", 300);
                textoTela("Existe uma grande torre onde voce pode provar seu valor como guerreiro e se morrer eu prometo valorizar seus itens pro proximo ota... digo, cliente", 300);
                textoTela("Se quiser algo bom e de qualidade, venha na minha loja.", 300);
                printf("(Pressione [ENTER] para continuar...)\n");
                getchar();
                tomadaDecisao();
                break;
            case 3:
                sala++;
                save(player);
                tomadaDecisao();
                break;
        }
    }
else 
{
    sprintf(txt, "Sala %d", sala);
    int i;
    for(i = 0; i < 70; i++) printf("-");
    printf("\n");
    int tamanho = strlen(txt);
    int espacos = (70 - 4 - tamanho) / 2;
    int resto = (70 - 4 - tamanho) % 2;
    for(i = 0; i < espacos; i++) printf("-");
    printf("  %s  ", txt);
    for(i = 0; i < espacos + resto; i++) printf("-");
    printf("\n");
    for(i = 0; i < 70; i++) printf("-");
    printf("\n\n");
    cross_platform_sleep(1000);
    limparTerminal();
    if(sala % 10 == 5)
    {
        textoTela("Voce encontrou uma fogueira . . .\n", 400);
        textoTela("Deseja parar e descansar?\n", 200);
        printf("[1] Sim [0] Nao\n\n");
        checkInput(&escolha, 0, 1);
        switch(escolha)
        {
            case 1:
                limparTerminal();
                textoTela("Voce decide parar para descansar . . .\n", 400);
                float a = numAle(26) + 4.0;
                int b = a / 100.0 * (float) player.hpMax, c = a;
                cura(&player, b);
                sprintf(txt, "Voce recuperou %d%% de HP\n", c);
                textoTela(txt, 200);
                printf("(Pressione [ENTER] para continuar...)\n");
                limparBuffer();
            case 0:
                limparTerminal();
                textoTela("Voce segue o seu caminho . . .\n\n", 400);
                sala=99;
                printf("(Pressione [ENTER] para continuar...)\n");
                limparBuffer();
                tomadaDecisao();
                break;
        }
    }
    else if(sala % 10 == 9)
    {
        textoTela("Voce encontrou uma loja\n", 200);
        textoTela("O vendedor acena para voce . . .\n", 400);
        textoTela("Deseja falar com ele?\n", 200);
        printf("[1] Sim [0] Nao\n\n");
        checkInput(&escolha, 0, 1);

        limparTerminal();
        switch(escolha)
        {
            case 1:
                textoTela("O mesmo vendedor do inicio . . .\n", 200);
                if(sala < 60) textoTela("Voce se pergunta como ele chegou aqui\n\n", 200);
                else textoTela("Voce ja aceitou que ele e onipresente\n\n", 300);
                printf("(Pressione [ENTER] para continuar...)\n");
                limparBuffer();
                loja();
                limparBuffer();
                textoTela("O vendedor te diz boa sorte antes de ir . . .\n", 300);
                if(sala < 10) textoTela("Voce se pergunta o porque . . .\n\n", 400);
                else if(sala < 20) textoTela("Agora voce sabe o porque . . .\n\n", 200);
                else if(sala < 60) textoTela("Pelo menos, voce sabe o que esta por vir\n\n", 300);
                else if(sala < 90) textoTela("E bom saber que alguem torce por voce\n\n", 200);
                else
                {
                    textoTela("Voce sente um peso na voz dele . . .\n", 300);
                    textoTela("Ao mesmo tempo voce sente um peso no seu peito\n", 300);
                    cross_platform_sleep(500);
                    textoTela("Essa vez e diferente . . .\n\n", 500);
                    printf("(Pressione [ENTER] para continuar...)\n");
                    limparBuffer();
                    limparTerminal();
                    textoTela(". . .\n", 500);
                    cross_platform_sleep(500);
                    textoTela("Voce se lembra depois de muito tempo . . .\n", 500);
                    cross_platform_sleep(500);
                    textoTela("Do que e sentir medo . . .\n", 400);
                    cross_platform_sleep(500);
                    textoTela("Nao\n", 200);
                    cross_platform_sleep(1000);
                    textoTela("Voce nao sabe o que esta por vir\n\n", 600);
                }
                printf("(Pressione [ENTER] para continuar...)\n");
                limparBuffer();
            case 0:
                limparTerminal();
                textoTela("Voce segue o seu caminho . . .\n\n", 400);
                sala++;
                printf("(Pressione [ENTER] para continuar...)\n");
                limparBuffer();
                tomadaDecisao();
                break;
        }
    }
    else if(sala % 10 == 0)
    {
        //codigo bosss
        

        //apos vitoria
        limparTerminal();
        textoTela("Voce se levanta e segue o seu caminho . . .\n\n", 400);
        printf("(Pressione [ENTER] para continuar...)\n");
        limparBuffer();
        sala++;
        save(player);
        limparTerminal();
        printf("Seu jogo foi salvo automaticamente\n\n");
        printf("(Pressione [ENTER] para continuar...)\n");
        limparBuffer();
        tomadaDecisao();
    }
    else
    {
        int caso, a[3];
        caso = salaAleatoria();
        caso = 3;
        DADOS inimigo[caso];
        switch(caso)
        {
            case 0:
                break;
            case 3:
                a[2] = dificuldadeAleatoria();
            case 2:
                a[1] = dificuldadeAleatoria();
            case 1:
                a[0] = dificuldadeAleatoria();
                for(int i = 0; i < caso; i++) 
                {
                    inimigoAleatorio(&inimigo[i], a[i]);
                }
                if(caso > 1)
                {
                    sprintf(txt, "Voce encontrou %d inimigos!\n", caso);
                    textoTela(txt, 200);
                }
                else
                {
                    textoTela("Voce encontrou um inimigo!\n", 200);
                }
                textoTela("Prepare-se para a batalha!\n\n", 200);
                printf("(Pressione [ENTER] para continuar...)\n");
                limparBuffer();
                
                batalharInimigo(inimigo, caso);
                break;
            case 4:
                break;
            case 5:
                textoTela("Voce achou um pergaminho de habilidade\n", 300);
                textoTela("Ao abrir voce ve uma mensagem escrita...\n", 300);
                textoTela("KKKKKKKK vc caiu na pegadinha do Mario Games KKKKKKK\n", 300);               
                break;
            default:
                printf("Aviso de Bug\n");
                getchar();
                break;
        }
        sala++;
    }
}
}

int dificuldadeAleatoria()
{
    float chance[5] = {0.0, 0.0, 0.0, 0.0, 0.0}; // Vetor que guarda as chances de raridade de item ser escolhido
    float peso1 = 0.05, peso2 = 0.15, peso3 = 0.5, peso4 = 2.0;
    if(sala <= 10) // Se o sala for entre 0 e 10, a chance de um item de raridade 1 é 90% e 2 é 10%
    {
        chance[1] = 65.0;
        chance[2] = 35.0;
    }
    if(sala >= 11 && sala <= 30) 
    {
        chance[1] = 50.0;
        chance[2] = 40.0;
        chance[3] = 8.0;
        chance[4] = 2.0;
    }
    if(sala >= 31 && sala <= 40)
    {
        chance[1] = 30.0;
        chance[2] = 45.0;
        chance[3] = 19.0;
        chance[4] = 5.0;
        chance[5] = 1.0;
    }
    if(sala >= 41 && sala <= 50) 
    {
        chance[1] = 25.0;
        chance[2] = 35.0;
        chance[3] = 25.0;
        chance[4] = 10.0;
        chance[5] = 5.0;
    }
    if(sala >=51 && sala <= 60) 
    {
        chance[2] = 30.0;
        chance[3] = 40.0;
        chance[4] = 20.0;
        chance[5] = 10.0;
    }
    if(sala >= 61 && sala <= 70) 
    {
        chance[2] = 20.0;
        chance[3] = 40.0;
        chance[4] = 30.0;
        chance[5] = 20.0;
    }
    if(sala >= 71 && sala <= 80) 
    {
        chance[2] = 5.0;
        chance[3] = 30.0;
        chance[4] = 40.0;
        chance[5] = 25.0;
    }
    if(sala >= 81 && sala <= 90) 
    {
        chance[3] = 20.0;
        chance[4] = 70.0;
        chance[5] = 10.0;

    }
    if(sala >= 91 && sala <= 100) 
    {
        chance[3] = 15.0;
        chance[4] = 50.0;
        chance[5] = 35.0;
    }

    return aleatorizaChance(6, chance);
}

void inimigoAleatorio(DADOS *inimigo, int objetivo)
{

    FILE *fp;
    fp = fopen("Dados-do-Jogo/inimigos.txt", "rt");
    int dificuldade;
    int quantidade = 0;
    int i, j;
    char nome[51], txt[101];
    while(!feof(fp))
    {
        fgets(txt, 100, fp);
        fgets(txt, 100, fp);
        fscanf(fp, "DIFICULDADE: %d\n", &dificuldade);
        if(dificuldade == objetivo) quantidade++;
        for(i = 0; i < 12; i++) fgets(txt, 100, fp);
    }

    int sorteio = numAle(quantidade);

    rewind(fp);

    while(!feof(fp))
    {
        fgets(txt, 100, fp);
        fgets(nome, 51, fp);
        nome[strcspn(nome, "\n")] = 0;


        fscanf(fp, "DIFICULDADE: %d\n", &dificuldade);

        if(dificuldade == objetivo) 
        {
            sorteio--;
        }
        if(sorteio != 0) 
        {
            for(i = 0; i < 12; i++) 
            {
                fgets(txt, 100, fp);
            }
        }
        else 
        {
            strcpy(inimigo->nome, nome);
            fscanf(fp, "\nATRIBUTOS:\nFORCA: %d\nPROTECAO: %d\nAGILIDADE: %d\nINTELIGENCIA: %d\nCARISMA: %d\n\nHP: %d\nMP: %d\n\nEXP: %d\n", &inimigo->forca, &inimigo->protecao, &inimigo->agilidade, &inimigo->inteligencia, &inimigo->carisma, &inimigo->hp, &inimigo->mp, &inimigo->exp);
            break;
        }
    }
    fclose(fp);
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 15; j++)
        {
            inimigo->buffs[i][j] = 0;
        }
    }
    inimigo->hpMax = inimigo->protecao * 3;
    inimigo->hpMax = inimigo->hp;
    inimigo->manaMax = inimigo->inteligencia * 3;
    inimigo->mp = inimigo->manaMax;
}


int salaAleatoria()
{
    int caso;
    int a, b;
    int chance = numAle(101) - 1;
    int vazio = 10, inimigo = 70;
    caso = chance < vazio ? 0 :
            chance < vazio + inimigo ? 1 : 2;
    switch(caso)
    {
        case 1:  // Se for um inimigo vai calcular chance de ser um inimigo aleatorio
            a = 60, b = 30; // Inimigo 1 = 60%, Inimigo 2 = 30%, Inimigo 3 = 10%
            chance = numAle(101) - 1;
            caso = chance < a ? 1 : 
                chance < a + b ? 2 : 3;
            break;
        case 2:
            a = 80; // Se for item, vai calcular a chance de ser bau = 80% e pergaminho = 20%
            chance = numAle(101) - 1;
            caso = chance < a ? 4 : 5;
            break;
        default:
            break;
    }
    return caso;
}

void checkInput(int * n, int min, int max)
{
    int erro = 0;
    int a;
    while(1)
    {
        a = scanf(" %d", n);
        if(a != 1 || a == EOF || *n < min || *n > max)
        {
            limparLinhas(1);
            if(erro) limparLinhas(1);
            printf("Escolha ivalida, por favor digite um numero entre %d e %d\n", min, max);
            erro = 1;
            limparBuffer();
        }
        else break;
    }
    limparBuffer();
}

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
    int espacos, bonus;
    int i;
    char txt[71], txt2[71];

    sprintf(txt, "%s Lvl %d", player.nome, player.level);
    printf("%s", txt);
    espacos = largura - strlen(txt);
    for(i = 0; i < espacos; i++) printf(" ");

    sprintf(txt, "Equipados:");
    espacos = (largura - strlen(txt)) / 2;
    for(i = 0; i < espacos; i++) printf(" ");  
    printf("%s", txt);

    printf("\n");

    sprintf(txt, "HP: %d/%d", player.hp, player.hpMax);
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
    bonus = bonusAplicado(1);
    if(bonus != 0) 
    {
        sprintf(txt, "Forca: %d", player.forca - bonus);
        if (bonus > 0) sprintf(txt2, " (+%d)", bonus); 
        else sprintf(txt2, " (%d)", bonus);
        strcat(txt, txt2);
    }
    printf("%s", txt);
    espacos = largura - strlen(txt) - 3 - 15;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("[1]");
    espacos = largura - espacos - strlen(txt) - 3;
    for(i = 0; i < espacos; i++) printf(" ");

    sprintf(txt, "%s", item[player.equipado[1]].nome);
    espacos = (largura - strlen(txt)) / 2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s", txt);

    printf("\n");

    sprintf(txt, "Inteligencia: %d", player.inteligencia);
    bonus = bonusAplicado(3);
    if(bonus != 0) 
    {
        sprintf(txt, "Inteligencia: %d", player.inteligencia - bonus);
        if (bonus > 0) sprintf(txt2, " (+%d)", bonus); 
        else sprintf(txt2, " (%d)", bonus);
        strcat(txt, txt2);
    }
    printf("%s", txt);
    espacos = largura - strlen(txt) - 3 - 15;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("[2]");
    espacos = largura - espacos - strlen(txt) - 3;
    for(i = 0; i < espacos; i++) printf(" ");

    sprintf(txt, "%s", item[player.equipado[2]].nome);
    espacos = (largura - strlen(txt)) / 2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s", txt);

    printf("\n");

    sprintf(txt, "Protecao: %d", player.protecao);
    bonus = bonusAplicado(5);
    if(bonus != 0) 
    {
        sprintf(txt, "Protecao: %d", player.protecao - bonus);
        if (bonus > 0) sprintf(txt2, " (+%d)", bonus); 
        else sprintf(txt2, " (%d)", bonus);
        strcat(txt, txt2);
    }
    printf("%s", txt);
    espacos = largura - strlen(txt) - 3 - 15;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("[3]");
    espacos = largura - espacos - strlen(txt) - 3;
    for(i = 0; i < espacos; i++) printf(" ");

    sprintf(txt, "%s", item[player.equipado[0]].nome);
    espacos = (largura - strlen(txt)) / 2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s", txt);

    printf("\n");

    sprintf(txt, "Agilidade: %d", player.agilidade);
    bonus = bonusAplicado(2);
    if(bonus != 0) 
    {
        sprintf(txt, "Agilidade: %d", player.agilidade);
        if (bonus > 0) sprintf(txt2, " (+%d)", bonus); 
        else sprintf(txt2, " (%d)", bonus);
        strcat(txt, txt2);
    }
    espacos = largura - strlen(txt) - 3 - 15;
    printf("%s", txt);
    for(i = 0; i < espacos; i++) printf(" ");
    printf("[4]");
    espacos = largura - espacos - strlen(txt) - 3;
    for(i = 0; i < espacos; i++) printf(" ");

    printf("\n");

    sprintf(txt, "Carisma: %d", player.carisma);
    bonus = bonusAplicado(4);
    if(bonus != 0) 
    {
        sprintf(txt, "Carisma: %d", player.carisma - bonus);
        if (bonus > 0) sprintf(txt2, " (+%d)", bonus); 
        else sprintf(txt2, " (%d)", bonus);
        strcat(txt, txt2);
    }
    printf("%s", txt);
    espacos = largura - strlen(txt) - 3 - 15;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("[5]");
    espacos = largura - espacos - strlen(txt) - 3;
    for(i = 0; i < espacos; i++) printf(" ");

    sprintf(txt, "Moedas: \033[33m%d\033[0m", player.moedas);
    espacos = (largura + 9 - strlen(txt))/2;
    for(i = 0; i < espacos; i++) printf(" ");
    printf("%s\n", txt);
    

    printf("\nSkill Points: %d\n", player.skillPoints);

    int barSize = 60;

    float media = (float)player.exp / (float)player.expMax;
    if (media > 1) media = 1;

    for(i = 0; i < media * barSize; i++) printf("\033[32m=\033[0m");
    for(i = 0; i < (barSize - media * barSize); i++) printf("-");
    printf("  %d/%d", player.exp, player.expMax);
    printf("\nEXP\n\n");
    
    int escolha;

    checkInput(&escolha, 1, 10);

    if(escolha == 1) 
    {
        player.exp += (float)player.expMax /2.0;
        calculoExp();
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
                    player.hpMax += item[id].bonus[1][i] * sinal;
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
                    player.hpMax += (player.hpMax * item[id].bonus[1][i] / 100) * sinal;
                    player.hp += (player.hpMax * item[id].bonus[1][i] / 100) * sinal; // Ajusta o HP atual
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
                    player.hp += (player.hpMax * item[id].bonus[1][i] / 100);
                    if(player.hp > player.hpMax) player.hp = player.hpMax;
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
        printf("Esse item nao pode ser usado.\n\n");
        printf("(Pressione [ENTER] para continuar...)\n");
        return;
    }
    lixo(espaco, 1); // Remove o item do inventário
    if(tipo >= 1 && tipo <= 4)
    {
        printf("Voce usou %s\n\n", item[id].nome);
        tipo = tipo - 1; // Ajusta o tipo para o índice do vetor equipado
        if(player.equipado[tipo] != 0) 
        {
            printf("Voce desequipou %s\n\n", item[player.equipado[tipo]].nome);
            addItem(player.equipado[tipo]); // Adiciona o item equipado de volta ao inventário
            ajustaBonus(player.equipado[tipo], -1); // Remove os bônus do item equipado
            player.equipado[tipo] = 0; // Remove o item equipado
        }
        printf("\n");
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
    printf("\n\n");
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
        case 3:
        case 1:
            printf("\n[0] SAIR\n");
            printf("\nDigite o numero do item para ver informacoes:\n\n");
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
                limparBuffer();
                descreverItem(player.inventario[0][escolha - 1]); // Descreve o item escolhido
                printf("\n");
                sprintf(txt, "[1] USAR ITEM [0] VOLTAR");
                int t = strlen(txt);
                for(i = 0; i < ((60 - t) / 2); i++) printf(" ");
                printf("%s\n\n", txt);
                checkInput(&escolha2, 0, 1);
                if(escolha2 == 1) 
                {
                    if (modo == 3 && item[player.inventario[0][escolha - 1]].tipo == 6) 
                    {
                        printf("Esse item nao pode ser usado durante uma batalha!\n\n");
                        printf("(Pressione [ENTER] para continuar...)\n");
                        limparBuffer();
                        verInventario(3);
                        return;
                    }
                    limparTerminal();
                    usarItem(escolha - 1);
                    printf("(Pressione [ENTER] para continuar...)\n");
                    limparBuffer();
                }
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
            strcpy(tipo, "");
            break;
        case 1:
            strcpy(tipo, "Arma Cortante");
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
            strcpy(tipo, "Arma de Alcance");
            break;
        case 7:
            strcpy(tipo, "Arma de Impacto");
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
    if(escolha == 6) loja();
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
                    if(item[a[escolha-1]].tipo != 4) a[escolha-1] = 0; //Retira o item da loja
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
    int sorteio = numAle(total + 1) - 1;
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


void loja() // Sempre antes de boss. Boss a cada 10 fases
{
    int a[3], id[3]; // Vetor que guarda os ids dos itens
    
    a[0] = raridadeAleatoria();
    a[1] = raridadeAleatoria();
    a[2] = raridadeAleatoria();


    if(sala >= 91 && sala <= 100) // Se o sala for entre 91 e 100
    {
        for(int i = 0; i < 3; i++)
        {
            if(a[i] != 4) 
            {
                a[i] = 4;
                break;
            }
        }
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
    
    calculoExp();
    vitrine(id);
}

void setItens(int q){
    int i;
    if(q > MAX_ITEMS)
    {
        printf("Quantidade de itens invalida.\n");
        exit(1);
    }
    FILE *arq = fopen("Dados-do-Jogo/items.txt", "rt");
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
    FILE *arq = fopen("Dados-do-Jogo/items.txt", "rt");
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
        _mkdir("Dados-do-Jogo");
    #else
        mkdir("Dados-do-Jogo", 0777);
    #endif
    FILE *arq = fopen("Dados-do-Jogo/save.txt", "w");
    if (arq == NULL) {
        printf("Erro ao criar o arquivo.\n");
    }
    fclose(arq);
}

void save(DADOS player)
{
    int i, j;
    FILE *arq = fopen("Dados-do-Jogo/save.txt", "w");
    if (arq == NULL) {
        printf("Erro ao salvar.\n");
        return;
    }
    player.nome[strcspn(player.nome, "\n")] = 0;

    fprintf(arq, "%s\n", player.nome);
    fprintf(arq, "Sala %d\n", sala);
    fprintf(arq, "Level %d: %d/%d\n", player.level, player.exp, player.expMax);
    fprintf(arq, "Skill Points: %d\n", player.skillPoints);
    fprintf(arq, "HP: %d/%d\n", player.hp, player.hpMax);
    fprintf(arq, "MP: %d/%d\n", player.mp, player.manaMax);
    fprintf(arq, "Forca: %d\n", player.forca);
    fprintf(arq, "Protecao: %d\n", player.protecao);
    fprintf(arq, "Agilidade: %d\n", player.agilidade);
    fprintf(arq, "Inteligencia: %d\n", player.inteligencia);
    fprintf(arq, "Carisma: %d\n", player.carisma);
    fprintf(arq, "Moedas: %d\n", player.moedas);
    fprintf(arq, "Equipados: %d, %d, %d, %d\n", player.equipado[0], player.equipado[1], player.equipado[2], player.equipado[3]);

    for (i = 0; i < 20; i++) {
        fprintf(arq, "%d %d\n", player.inventario[0][i], player.inventario[1][i]);
    }

    for (i = 0; i < 3; i++)
    {
        for(j = 0; j < 15; j++)
        {
            fprintf(arq, "%d ", player.buffs[i][j]);
        }
        fprintf(arq, "\n");
    }

    fclose(arq);
}


void load(DADOS *player, int *sala) 
{
    int i, j;

    FILE *arq = fopen("Dados-do-Jogo/save.txt", "rt");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fgets(player->nome, sizeof(player->nome), arq);
    player->nome[strcspn(player->nome, "\n")] = 0; //Transforma caractere "\n" em "\0"
    
    fscanf(arq, "Sala %d\n", sala);
    fscanf(arq, "Level %d: %d/%d\n", &player->level, &player->exp, &player->expMax);
    fscanf(arq, "Skill Points: %d\n", &player->skillPoints);
    fscanf(arq, "HP: %d/%d\n", &player->hp, &player->hpMax);
    fscanf(arq, "MP: %d/%d\n", &player->mp, &player->manaMax);
    fscanf(arq, "Forca: %d\n", &player->forca);
    fscanf(arq, "Protecao: %d\n", &player->protecao);
    fscanf(arq, "Agilidade: %d\n", &player->agilidade);
    fscanf(arq, "Inteligencia: %d\n", &player->inteligencia);
    fscanf(arq, "Carisma: %d\n", &player->carisma);
    fscanf(arq, "Moedas: %d\n", &player->moedas);
    fscanf(arq, "Equipados: %d, %d, %d, %d\n", &player->equipado[0], &player->equipado[1], &player->equipado[2], &player->equipado[3]);

    for (i = 0; i < 20; i++) {
        fscanf(arq, "%d %d\n", &player->inventario[0][i], &player->inventario[1][i]);
    }
    for (i = 0; i < 3; i++)
    {
        for(j = 0; j < 10; j++)
        {
            fscanf(arq, "%d ", &player->buffs[i][j]);
        }
        fscanf(arq, "\n");
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
    while(player.exp >= player.expMax) 
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
    player.hpMax = player.protecao * 5;
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
    player.hpMax = player.protecao * 5;
    player.hp = player.hpMax;
    player.manaMax = player.inteligencia * 5;
    player.mp = player.manaMax;

    player.level =1;
    player.expMax = 50;
    player.exp=0;
    int i, j;

    aleatStatus(20);
    

    txt[strcspn(txt, "\n")] = 0; // Tira o caractere "\n" da string txt e coloca "\0"

    strcpy(player.nome, txt);
   
    player.moedas = 1000;

    // Adicione esta inicialização para o inventário:
    for (i = 0; i < 2; i++) 
    {
        for(j = 0; j < 20; j++)
        {
            player.inventario[i][j] = 0; // Inicializa cada slot do inventário com 0
        }
    }

    for(i = 0; i < 4; i++) {
        player.equipado[i] = 0; // Inicializa os itens equipados com 0
    }

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 15; j++)
        {
            player.buffs[i][j] = 0;
        }
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

void txtTela(char * mensagem, int seg)
{
    char *palavra = strtok(mensagem, " ");
    while (palavra != NULL) {
        printf("%s ", palavra);
        fflush(stdout);
        cross_platform_sleep(seg); // Usando função multiplataforma
        palavra = strtok(NULL, " ");
    }
    printf("\n");
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

void verificaNomePlayer(char *nome) {
    int i = 0, OK = 0; // OK: verifica se o nome original sofreu alguma alteração
    char nome_corrigido[101] = {0}, nome_novo[101] = {0}; 
    // nome_corrigido = altera o nome (sem os casos não permitidos)
    // nome_novo = pede um novo nome para a recursão
    limparTerminal();
    while (nome[i] != '\0') {
        if (!(nome[i] >= 'A' && nome[i] <= 'Z') && !(nome[i] >= 'a' && nome[i] <= 'z') && nome[i] != ' ') { // números e caracteres especiais
            OK = 1;
            textoTela("Que legal, uma pessoa com nome igual aos da internet...\n", 200);
            textoTela("Saudades do meu amigo Herobrine123.\n", 400);
            textoTela("Nunca vi um guerreiro tao valente como esse homem.\n", 300);
            textoTela("Enfim, tem algum outro nome que eu possa te chamar?\n", 200);
            fgets(nome_novo, 100, stdin);
            strip(nome_novo);
            verificaNomePlayer(nome_novo); // recursiva para verificar o novo nome;
            break;
        }
        else if (nome[i] == ' ') { // + de uma palavra
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
    char nome[101] = {0}; /*
    textoTela("Anos no passado, nossos ancestrais viviam tranquilamente...\n", 200);
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
    textoTela("\033[31mEles TOMAVAM...\033[0m\n", 600);
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
    textoTela("Errrn . . . Hoje! . . .\n", 500);
    textoTela("Desculpe, mas qual seu nome mesmo?\n\n", 300);*/
    limparBuffer();
    int OK = 1;
    while (OK) {  
        fgets(nome, 100, stdin);
        if (nome[0] == '\n') {
            strcpy(nome, ""); // reseta a string
            limparTerminal();
            textoTela("Nao quer falar seu nome??\n", 400);
            textoTela("Isso e falta de educacao, sabia?\n", 200);
            textoTela("Fala um nome qualquer entao, so pra eu registrar aqui:\n\n", 300);
        }
        else OK = 0; // nome foi digitado 
    }
    strip(nome);
    verificaNomePlayer(nome);
    
    // movi essa parte pra função verificaNomePlayer()
    textoTela("Quer comprar algo na minha loja?", 400); 
    printf("\n(Pressione ENTER para continuar...)\n");
    limparBuffer();
    limparTerminal();
    mostrarStatus();
    tomadaDecisao();
}

void telaInicial(){
    limparTerminal();
    cabecaTela("A ascenso na torre!");
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
            tomadaDecisao();
            break;
        case '3':
            limparTerminal();
            ranking();
            printf("[1] - Voltar para a tela inicial.\n");
            scanf("%s", &n);
            telaInicial();
            break;
        case '4':

            limparTerminal();
            cabecaTela("Integrantes");
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
