#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct da sala da mansao
struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
};

// Funcao para criar uma sala dinamicamente
struct Sala* criarSala(char nome[]) {
    struct Sala *novaSala;

    novaSala = (struct Sala*) malloc(sizeof(struct Sala));

    if (novaSala == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

// Funcao para explorar as salas
void explorarSalas(struct Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        // Se for uma sala sem caminhos, chegou ao fim
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Voce chegou a um comodo final da mansao.\n");
            break;
        }

        printf("Caminhos disponiveis:\n");

        if (atual->esquerda != NULL) {
            printf("e - Ir para a esquerda\n");
        }

        if (atual->direita != NULL) {
            printf("d - Ir para a direita\n");
        }

        printf("s - Sair da exploracao\n");
        printf("Escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 's') {
            printf("Exploracao encerrada pelo jogador.\n");
            break;
        } else if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Nao existe caminho para a esquerda.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Nao existe caminho para a direita.\n");
            }
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

// Funcao para liberar a memoria da arvore
void liberarSalas(struct Sala *raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

int main() {
    // Montando o mapa da mansao manualmente
    struct Sala *hall = criarSala("Hall de entrada");
    struct Sala *biblioteca = criarSala("Biblioteca");
    struct Sala *salaJantar = criarSala("Sala de jantar");
    struct Sala *escritorio = criarSala("Escritorio");
    struct Sala *jardim = criarSala("Jardim");
    struct Sala *cozinha = criarSala("Cozinha");
    struct Sala *porao = criarSala("Porao");

    // Ligando as salas na arvore binaria
    hall->esquerda = biblioteca;
    hall->direita = salaJantar;

    biblioteca->esquerda = escritorio;
    biblioteca->direita = jardim;

    salaJantar->esquerda = cozinha;
    salaJantar->direita = porao;

    // Iniciando exploracao
    printf("=== DETECTIVE QUEST ===\n");
    printf("Explore a mansao a partir do Hall de entrada.\n");

    explorarSalas(hall);

    // Liberando a memoria
    liberarSalas(hall);

    return 0;
}
