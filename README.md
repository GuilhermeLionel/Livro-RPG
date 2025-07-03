# 📚 Livro-Jogo Interativo em C

Este projeto foi desenvolvido como parte da disciplina **Programação Estruturada**, com o objetivo de criar um **livro-jogo** interativo utilizando a linguagem C, executado totalmente no **modo console**.

## 🎮 Sobre o Projeto

O jogo simula um capítulo de um livro-jogo, no qual o jogador pode tomar decisões que influenciam o rumo da história. O jogador cria um personagem, participa de batalhas, cumpre missões e avança por diferentes caminhos narrativos.

### Funcionalidades principais:

- Tela inicial com as opções:
  - Iniciar novo jogo
  - Continuar jogo salvo
  - Ranking de pontuações
  - Listar integrantes do grupo

- Criação de personagem com:
  - Geração aleatória de atributos
  - Distribuição manual de pontos

- Navegação pela história com:
  - Texto carregado de um arquivo
  - Opções de decisão para o jogador

- Sistema de combate com:
  - Informações do jogador e inimigo
  - Ações como lutar, fugir ou interagir

- Finais possíveis com:
  - Encerramento do jogo
  - Salvamento de pontuação no ranking
  - Continuação a partir do último salvamento

## 🧠 Estrutura de Dados

O jogo utiliza uma estrutura `Personagem` que armazena:

- Nome  
- Local atual no jogo  
- Pontuação  
- Alinhamento (bondoso, neutro ou maldoso)  
- Atributos: Força, Inteligência, Sabedoria, Carisma, etc.  
- Recursos: Vida, Magia, Energia  
- Itens e Quests (vetores de inteiros)

## 🔧 Funções principais

O projeto é modular e conta com funções como:

- `void grava(Personagem)` – salva o estado do jogo  
- `void le(Personagem)` – carrega o estado salvo  
- `void recorde(Personagem)` – salva a pontuação no ranking  
- `void inicio()` – exibe a tela inicial  
- `void criar(Personagem*)` – cria o personagem do jogador  
- `void menu()` – exibe as opções do jogo  
- `void combate(Personagem*, Personagem*)` – executa o sistema de combate  
- `void final()` – finaliza o jogo

## 👥 Integrantes do Grupo

- Nome 1 – Guilherme Lionel de Souza  
- Nome 2 – Alexandre Gabriel Angelo de Souza Blandino
- Nome 3 – Samuel Pereira da Silva

## 🎥 Apresentação

Além deste código, o projeto será acompanhado de um vídeo com a explicação do funcionamento do código e testes de jogabilidade.

## 🚀 Como compilar e executar

Siga os passos abaixo para rodar o jogo no terminal (Bash):

### 1. Abra o terminal

Acesse a pasta onde está o código-fonte do projeto.

```bash
gcc main.c -o livrojogo
