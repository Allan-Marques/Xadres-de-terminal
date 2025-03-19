#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define TAM 8

char tabuleiro[TAM][TAM];
char jogador1[50], jogador2[50];
int jogadorAtual;  // 0: jogador 1 (peças brancas - maiúsculas), 1: jogador 2 (peças pretas - minúsculas)

// Inicializa o tabuleiro com posições simplificadas
void inicializarTabuleiro() {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tabuleiro[i][j] = ' ';
        }
    }
    // Peças brancas (jogador 1)
    tabuleiro[0][0] = tabuleiro[0][7] = 'T';  // Torres
    tabuleiro[0][2] = tabuleiro[0][5] = 'B';  // Bispos
    tabuleiro[0][3] = 'Q';                    // Rainha
    // Peças pretas (jogador 2)
    tabuleiro[7][0] = tabuleiro[7][7] = 't';
    tabuleiro[7][2] = tabuleiro[7][5] = 'b';
    tabuleiro[7][3] = 'q';
}

// Imprime o tabuleiro com alinhamento uniforme
void imprimirTabuleiro() {
    printf("\n     a     b     c     d     e     f     g     h\n");
    for (int i = 0; i < TAM; i++) {
        printf("%d  ", 8 - i);
        for (int j = 0; j < TAM; j++) {
            printf("[ %c ] ", tabuleiro[i][j]);
        }
        printf(" %d\n", 8 - i);
    }
    printf("     a     b     c     d     e     f     g     h\n");
}

// Troca o jogador
void trocarJogador() {
    jogadorAtual = 1 - jogadorAtual;
}

// Calcula os movimentos possíveis para uma peça na posição (x, y).
// Armazena os destinos válidos em 'destinos' e retorna o número de movimentos.
int calcularMovimentosPossiveis(char peca, int x, int y, int destinos[][2]) {
    int count = 0;
    char p = tolower(peca);
    if (p == 't') { // Torre: horizontais e verticais
        for (int i = 0; i < TAM; i++) {
            if (i != x) {
                destinos[count][0] = i;
                destinos[count][1] = y;
                count++;
            }
        }
        for (int j = 0; j < TAM; j++) {
            if (j != y) {
                destinos[count][0] = x;
                destinos[count][1] = j;
                count++;
            }
        }
    } else if (p == 'b') { // Bispo: diagonais
        for (int i = 1; i < TAM; i++) {
            if (x + i < TAM && y + i < TAM) {
                destinos[count][0] = x + i;
                destinos[count][1] = y + i;
                count++;
            }
            if (x - i >= 0 && y - i >= 0) {
                destinos[count][0] = x - i;
                destinos[count][1] = y - i;
                count++;
            }
            if (x + i < TAM && y - i >= 0) {
                destinos[count][0] = x + i;
                destinos[count][1] = y - i;
                count++;
            }
            if (x - i >= 0 && y + i < TAM) {
                destinos[count][0] = x - i;
                destinos[count][1] = y + i;
                count++;
            }
        }
    } else if (p == 'q') { // Rainha: torre + bispo
        // Torre
        for (int i = 0; i < TAM; i++) {
            if (i != x) {
                destinos[count][0] = i;
                destinos[count][1] = y;
                count++;
            }
        }
        for (int j = 0; j < TAM; j++) {
            if (j != y) {
                destinos[count][0] = x;
                destinos[count][1] = j;
                count++;
            }
        }
        // Bispo
        for (int i = 1; i < TAM; i++) {
            if (x + i < TAM && y + i < TAM) {
                destinos[count][0] = x + i;
                destinos[count][1] = y + i;
                count++;
            }
            if (x - i >= 0 && y - i >= 0) {
                destinos[count][0] = x - i;
                destinos[count][1] = y - i;
                count++;
            }
            if (x + i < TAM && y - i >= 0) {
                destinos[count][0] = x + i;
                destinos[count][1] = y - i;
                count++;
            }
            if (x - i >= 0 && y + i < TAM) {
                destinos[count][0] = x - i;
                destinos[count][1] = y + i;
                count++;
            }
        }
    }
    return count;
}

// Exibe os movimentos possíveis e um tutorial com os códigos reais
void mostrarMovimentosPossiveis(char peca, int x, int y) {
    int destinos[64][2];
    int numDestinos = calcularMovimentosPossiveis(peca, x, y, destinos);
    printf("\nOpções disponíveis para %c na posição (%c%d):\n", peca, 'a' + y, 8 - x);
    printf("Movimentos possíveis: ");
    for (int i = 0; i < numDestinos; i++) {
         printf("(%c%d) ", 'a' + destinos[i][1], 8 - destinos[i][0]);
    }
    printf("\n");
    
    // Tutorial: exibe os códigos dos movimentos possíveis
    printf("Tutorial: para mover a peça %c, você pode digitar um dos seguintes códigos:\n", peca);
    for (int i = 0; i < numDestinos; i++) {
         printf("Código %d: (%c%d)\n", i+1, 'a' + destinos[i][1], 8 - destinos[i][0]);
    }
}

// Lista as posições onde a peça selecionada se encontra
int listarPosicoes(char peca) {
    int found = 0;
    printf("Posições da peça %c: ", peca);
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == peca) {
                printf("(%c%d) ", 'a' + j, 8 - i);
                found = 1;
            }
        }
    }
    printf("\n");
    return found;
}

// Lê uma coordenada (ex: a2) e converte para índices (x, y) usando fgets.
// Retorna 1 se o formato estiver correto, 0 caso contrário.
int lerCoordenada(char prompt[], int *x, int *y) {
    char coord[10];
    printf("%s", prompt);
    if(fgets(coord, sizeof(coord), stdin) == NULL)
        return 0;
    coord[strcspn(coord, "\n")] = '\0'; // remove o \n
    if (strlen(coord) != 2)
        return 0;
    if (!isalpha(coord[0]) || !isdigit(coord[1]))
        return 0;
    *y = tolower(coord[0]) - 'a';
    *x = 8 - (coord[1] - '0');
    if (*x < 0 || *x >= TAM || *y < 0 || *y >= TAM)
        return 0;
    return 1;
}

// Inicia o jogo lendo os nomes e sorteando o jogador
void iniciarJogo() {
    printf("Bem-vindo ao Xadrez Terminal!\n");
    printf("Digite o nome do Jogador 1 (peças brancas): ");
    fgets(jogador1, sizeof(jogador1), stdin);
    jogador1[strcspn(jogador1, "\n")] = 0;
    
    printf("Digite o nome do Jogador 2 (peças pretas): ");
    fgets(jogador2, sizeof(jogador2), stdin);
    jogador2[strcspn(jogador2, "\n")] = 0;
    
    srand(time(NULL));
    jogadorAtual = rand() % 2;
    printf("\n%s começa o jogo!\n", jogadorAtual == 0 ? jogador1 : jogador2);
}

int main() {
    iniciarJogo();
    inicializarTabuleiro();
    
    while (1) {
        char jogadorNome[50];
        strcpy(jogadorNome, jogadorAtual == 0 ? jogador1 : jogador2);
        printf("\n%s, é a sua vez!\n", jogadorNome);
        imprimirTabuleiro();
        
        // PASSO 1: Seleção da peça
        char pecaEscolhida;
        while (1) {
            if (jogadorAtual == 0)
                printf("Digite a letra da peça que deseja mover (T, B, Q): ");
            else
                printf("Digite a letra da peça que deseja mover (t, b, q): ");
            if(fgets(&pecaEscolhida, 3, stdin) == NULL)
                continue;
            // Limpar o restante da linha, se houver
            while(getchar() != '\n');
            if (jogadorAtual == 0 && (pecaEscolhida=='T' || pecaEscolhida=='B' || pecaEscolhida=='Q'))
                break;
            if (jogadorAtual == 1 && (pecaEscolhida=='t' || pecaEscolhida=='b' || pecaEscolhida=='q'))
                break;
            printf("Peça inválida. Tente novamente.\n");
        }
        
        // PASSO 2: Listar posições da peça selecionada
        if (!listarPosicoes(pecaEscolhida)) {
            printf("Nenhuma peça %c encontrada. Volte a selecionar outra peça.\n", pecaEscolhida);
            continue;
        }
        
        // PASSO 3: Selecionar a posição de origem dentre as listadas
        int ox, oy;
        {
            char prompt[100];
            sprintf(prompt, "Digite a posição onde está a peça %c que deseja mover: ", pecaEscolhida);
            while (1) {
                if (!lerCoordenada(prompt, &ox, &oy)) {
                    printf("Formato inválido. Tente novamente.\n");
                    continue;
                }
                printf("DEBUG: Coordenada lida -> (%d, %d)\n", ox, oy);
                if (tabuleiro[ox][oy] == pecaEscolhida)
                    break;
                printf("A posição digitada não contém a peça %c. Tente novamente.\n", pecaEscolhida);
            }
        }
        
        // PASSO 4: Mostrar movimentos possíveis para a peça na posição escolhida
        mostrarMovimentosPossiveis(pecaEscolhida, ox, oy);
        
        // PASSO 5: Selecionar a posição de destino ou voltar à seleção
        int dx = -1, dy = -1;
        int destinos[64][2];
        int numDestinos = calcularMovimentosPossiveis(pecaEscolhida, ox, oy, destinos);
        char input[10];
        while (1) {
            printf("Digite a posição de destino (ex: a2), o código do movimento ou 'V' para voltar: ");
            if(fgets(input, sizeof(input), stdin) == NULL)
                continue;
            input[strcspn(input, "\n")] = '\0';
            if (strlen(input) == 0)
                continue;
            if (tolower(input[0]) == 'v') {
                printf("Voltando à seleção de peça...\n");
                break;
            }
            int isNumber = 1;
            for (int i = 0; input[i] != '\0'; i++) {
                if (!isdigit(input[i])) { isNumber = 0; break; }
            }
            if (isNumber) {
                int code = atoi(input);
                if (code < 1 || code > numDestinos) {
                    printf("Código inválido. As opções são de 1 a %d.\n", numDestinos);
                    continue;
                }
                dx = destinos[code-1][0];
                dy = destinos[code-1][1];
            } else if (strlen(input) == 2) {
                if (!lerCoordenada("", &dx, &dy)) {
                    printf("Formato inválido.\n");
                    continue;
                }
            } else {
                printf("Entrada inválida. Tente novamente.\n");
                continue;
            }
            
            printf("DEBUG: Destino lido -> (%d, %d)\n", dx, dy);
            int moveValido = 0;
            for (int i = 0; i < numDestinos; i++) {
                if (destinos[i][0] == dx && destinos[i][1] == dy) {
                    moveValido = 1;
                    break;
                }
            }
            if (moveValido) {
                tabuleiro[dx][dy] = pecaEscolhida;
                tabuleiro[ox][oy] = ' ';
                break;
            } else {
                printf("Movimento não permitido para essa peça.\n");
                mostrarMovimentosPossiveis(pecaEscolhida, ox, oy);
            }
        }
        
        // Se o movimento foi realizado (dx e dy válidos), troca o turno
        if (dx != -1 && dy != -1)
            trocarJogador();
    }
    return 0;
}
