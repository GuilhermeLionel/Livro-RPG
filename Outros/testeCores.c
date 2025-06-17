#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

// --- ATENÇÃO: Adicione esta linha antes de incluir <windows.h> ---
// Define a versão mínima do Windows para Windows 10 (0x0A00)
// Isso é necessário para que ENABLE_VIRTUAL_TERMINAL_PROCESSING seja definido
#define _WIN32_WINNT 0x0A00 

#ifdef _WIN32
#include <windows.h>    // Essencial para ativar as cores ANSI no Windows
#include <direct.h>     // mkdir para Windows (não usado neste código)
#else
#include <unistd.h>     // usleep (não usado neste código)
#include <sys/stat.h>   // mkdir (não usado neste código)
#endif


int main() {
    // --- Configuração Específica para Windows para Cores ANSI ---
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Erro: Não foi possível obter o handle da console.\n");
        return 1;
    }
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        fprintf(stderr, "Erro: Não foi possível obter o modo da console.\n");
        return 1;
    }
    // Ativa o processamento de terminal virtual (suporte a ANSI)
    if (!SetConsoleMode(hOut, dwMode)) {
        fprintf(stderr, "Erro: Não foi possível definir o modo da console.\n");
        return 1;
    }
    #endif
    // --- Fim da Configuração para Windows ---

    printf("\n--- Cores de Texto ---\n");
    printf("\x1b[30mPreto\x1b[0m\n");
    printf("\x1b[31mVermelho\x1b[0m\n");
    printf("\x1b[32mVerde\x1b[0m\n");
    printf("\x1b[33mAmarelo\x1b[0m\n");
    printf("\x1b[34mAzul\x1b[0m\n");
    printf("\x1b[35mMagenta\x1b[0m\n");
    printf("\x1b[36mCiano\x1b[0m\n"); // Corrigido 'wCiano' para 'mCiano'
    printf("\x1b[37mBranco (ou cinza claro)\x1b[0m\n");
    printf("\x1b[90mCinza Escuro\x1b[0m\n");
    printf("\x1b[91mVermelho Claro\x1b[0m\n");
    printf("\x1b[92mVerde Claro\x1b[0m\n");
    printf("\x1b[93mAmarelo Claro\x1b[0m\n");
    printf("\x1b[94mAzul Claro\x1b[0m\n");
    printf("\x1b[95mMagenta Claro\x1b[0m\n");
    printf("\x1b[96mCiano Claro\x1b[0m\n");
    printf("\x1b[97mBranco\x1b[0m\n");

    printf("\n--- Cores de Fundo ---\n");
    printf("\x1b[40mFundo Preto\x1b[0m\n");
    printf("\x1b[41mFundo Vermelho\x1b[0m\n");
    printf("\x1b[42mFundo Verde\x1b[0m\n");
    printf("\x1b[43mFundo Amarelo\x1b[0m\n");
    printf("\x1b[44mFundo Azul\x1b[0m\n");
    printf("\x1b[45mFundo Magenta\x1b[0m\n");
    printf("\x1b[46mFundo Ciano\x1b[0m\n");
    printf("\x1b[47mFundo Branco (ou cinza claro)\x1b[0m\n");
    printf("\x1b[100mFundo Cinza Escuro\x1b[0m\n");
    printf("\x1b[101mFundo Vermelho Claro\x1b[0m\n");
    printf("\x1b[102mFundo Verde Claro\x1b[0m\n");
    printf("\x1b[103mFundo Amarelo Claro\x1b[0m\n");
    printf("\x1b[104mFundo Azul Claro\x1b[0m\n");
    printf("\x1b[105mFundo Magenta Claro\x1b[0m\n");
    printf("\x1b[106mFundo Ciano Claro\x1b[0m\n");
    printf("\x1b[107mFundo Branco\x1b[0m\n");

    printf("\n--- Estilos de Texto e Combinações ---\n");
    printf("\x1b[1mTexto em Negrito\x1b[0m\n");
    printf("\x1b[2mTexto Dim\x1b[0m\n");
    printf("\x1b[3mTexto Itálico (Nem sempre suportado)\x1b[0m\n");
    printf("\x1b[4mTexto Sublinhado\x1b[0m\n");
    printf("\x1b[5mTexto Piscando (Geralmente ignorado)\x1b[0m\n");
    printf("\x1b[7mCores Invertidas (Fundo vira Texto, Texto vira Fundo)\x1b[0m\n");
    printf("\x1b[8mTexto Escondido\x1b[0m\n");
    printf("\x1b[9mTexto Tachado\x1b[0m\n");
    printf("\x1b[31;47mTexto Vermelho com Fundo Branco\x1b[0m\n");
    printf("\x1b[1;32;40mNegrito, Verde, Fundo Preto\x1b[0m\n");

    printf("\n--- Cores RGB (24-bit True Color) ---\n");
    // Verifica se o terminal suporta true color (nem todos suportam)
    printf("Este terminal pode suportar cores RGB (24-bit True Color):\n");
    printf("\x1b[38;2;255;0;0mVermelho Puro\x1b[0m\n");
    printf("\x1b[38;2;0;255;0mVerde Puro\x1b[0m\n");
    printf("\x1b[38;2;0;0;255mAzul Puro\x1b[0m\n");
    printf("\x1b[38;2;255;165;0mLaranja\x1b[0m\n");
    printf("\x1b[48;2;128;0;128mFundo Roxo\x1b[0m\n");
    printf("\x1b[38;2;255;215;0m\x1b[48;2;10;10;10mDourado em Fundo Escuro\x1b[0m\n");


    printf("\n--- Resetando Cores e Estilos ---\n");
    printf("Este texto deve estar com as cores e estilos padrão do terminal.\n");

    getchar();

    return 0;
}