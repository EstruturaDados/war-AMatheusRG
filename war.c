#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[15];
    int tropas;
} Territorio;

/* --- Funções básicas --- */
void preencherTerritorio(Territorio *t) {
    printf("Digite o nome do território: ");
    scanf("%29s", t->nome);   // limita entrada para evitar overflow

    printf("Digite a cor do território: ");
    scanf("%14s", t->cor);    // limita entrada

    printf("Digite o número de tropas: ");
    scanf("%d", &t->tropas);
}

void exibirTerritorio(Territorio *t, int idx) {
    printf("[%d] %s (Cor: %s) - Tropas: %d\n", idx, t->nome, t->cor, t->tropas);
}

/* Função de ataque: um dado pra cada lado; atacante precisa ter >1 tropa */
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) return;

    if (atacante->tropas <= 1) {
        printf("%s não tem tropas suficientes para atacar (tropas = %d).\n",
               atacante->nome, atacante->tropas);
        return;
    }

    int dadoA = rand() % 6 + 1; // 1..6
    int dadoD = rand() % 6 + 1; // 1..6

    printf("\n%s ataca %s!\n", atacante->nome, defensor->nome);
    printf("Rolagem: %s -> %d | %s -> %d\n", atacante->nome, dadoA, defensor->nome, dadoD);

    if (dadoA > dadoD) {
        defensor->tropas -= 1;
        if (defensor->tropas < 0) defensor->tropas = 0;
        printf("%s perde 1 tropa (agora %d).\n", defensor->nome, defensor->tropas);
    } else {
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("%s perde 1 tropa (agora %d).\n", atacante->nome, atacante->tropas);
    }
}

/* Mostra lista de territórios */
void listarTerritorios(Territorio *vet, int n) {
    printf("\n=== Lista de Territórios ===\n");
    for (int i = 0; i < n; i++) {
        exibirTerritorio(&vet[i], i);
    }
    printf("\n");
}

int main() {
    srand((unsigned) time(NULL)); // inicializa rand

    int n;
    printf("Quantos territórios deseja cadastrar? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Número inválido. Saindo.\n");
        return 1;
    }

    // aloca dinamicamente com calloc (inicializa com zeros)
    Territorio *territorios = (Territorio *) calloc(n, sizeof(Territorio));
    if (territorios == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Preenche os territórios
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do território %d ---\n", i);
        preencherTerritorio(&territorios[i]);
    }

    int opcao = 0;
    while (1) {
        listarTerritorios(territorios, n);

        printf("Menu:\n");
        printf("1 - Atacar (uma rodada)\n");
        printf("2 - Auto-atacar até fim (atacante repete até não poder ou defensor zerar)\n");
        printf("3 - Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            // limparem entrada em caso de erro
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        if (opcao == 3) break;

        int idxA, idxD;
        printf("Escolha o índice do território atacante: ");
        if (scanf("%d", &idxA) != 1 || idxA < 0 || idxA >= n) {
            printf("Índice atacante inválido.\n");
            continue;
        }
        printf("Escolha o índice do território defensor: ");
        if (scanf("%d", &idxD) != 1 || idxD < 0 || idxD >= n) {
            printf("Índice defensor inválido.\n");
            continue;
        }
        if (idxA == idxD) {
            printf("Atacante e defensor devem ser territórios diferentes.\n");
            continue;
        }

        Territorio *att = &territorios[idxA];
        Territorio *def = &territorios[idxD];

        if (opcao == 1) {
            // uma rodada
            atacar(att, def);
        } else if (opcao == 2) {
            // auto-ataque até condição de parada
            while (att->tropas > 1 && def->tropas > 0) {
                atacar(att, def);
                // opcional: pequena pausa visual
                // getchar();
            }
            if (def->tropas == 0) {
                printf("\n%s conquistou %s!\n", att->nome, def->nome);
            } else {
                printf("\n%s não pode continuar atacando (tropas = %d).\n", att->nome, att->tropas);
            }
        } else {
            printf("Opção inválida.\n");
        }
    }

    // libera memória
    free(territorios);
    printf("Programa finalizado. Memória liberada.\n");
    return 0;
}
