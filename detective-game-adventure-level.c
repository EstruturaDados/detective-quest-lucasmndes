#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct da sala da mansao
struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
};

// Struct do no da BST de pistas
struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
};

// Cria uma sala dinamicamente
struct Sala* criarSala(char nome[], char pista[]) {
    struct Sala *novaSala = (struct Sala*) malloc(sizeof(struct Sala));

    if (novaSala == NULL) {
        printf("Erro ao alocar memoria para a sala.\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

// Cria um no de pista
struct PistaNode* criarPistaNode(char pista[]) {
    struct PistaNode *novo = (struct PistaNode*) malloc(sizeof(struct PistaNode));

    if (novo == NULL) {
        printf("Erro ao alocar memoria para a pista.\n");
        exit(1);
    }

    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

// Insere pista na BST
struct PistaNode* inserirPista(struct PistaNode *raiz, char pista[]) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }

    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }

    return raiz;
}

// Exibe pistas em ordem alfabetica
void exibirPistas(struct PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Explora a mansao e coleta pistas
void explorarSalasComPistas(struct Sala *atual, struct PistaNode **arvorePistas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        // Se a sala tiver pista, coleta
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista encontrada nesta sala.\n");
        }

        printf("\nOpcoes:\n");

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
            printf("\nExploracao encerrada.\n");
            break;
        } else if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Nao existe sala a esquerda.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Nao existe sala a direita.\n");
            }
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

// Libera memoria das salas
void liberarSalas(struct Sala *raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

// Libera memoria das pistas
void liberarPistas(struct PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

int main() {
    struct PistaNode *arvorePistas = NULL;

    // Criando salas da mansao
    struct Sala *hall = criarSala("Hall de Entrada", "Pegadas no tapete");
    struct Sala *biblioteca = criarSala("Biblioteca", "Livro fora da estante");
    struct Sala *salaJantar = criarSala("Sala de Jantar", "Taça quebrada");
    struct Sala *escritorio = criarSala("Escritorio", "Bilhete rasgado");
    struct Sala *jardim = criarSala("Jardim", "");
    struct Sala *cozinha = criarSala("Cozinha", "Faca suja");
    struct Sala *porao = criarSala("Porao", "Lanterna apagada");

    // Montando a arvore da mansao
    hall->esquerda = biblioteca;
    hall->direita = salaJantar;

    biblioteca->esquerda = escritorio;
    biblioteca->direita = jardim;

    salaJantar->esquerda = cozinha;
    salaJantar->direita = porao;

    printf("=== DETECTIVE QUEST ===\n");
    printf("Explore a mansao e colete pistas.\n");

    explorarSalasComPistas(hall, &arvorePistas);

    printf("\n=== PISTAS COLETADAS EM ORDEM ALFABETICA ===\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(arvorePistas);
    }

    liberarSalas(hall);
    liberarPistas(arvorePistas);

    return 0;
}
