#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// Struct da sala da mansao
struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
};

// Struct da arvore BST de pistas
struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
};

// Struct da tabela hash
struct HashItem {
    char pista[100];
    char suspeito[50];
    int ocupado;
};

// Tabela hash global
struct HashItem tabelaHash[TAM_HASH];

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

// Cria um no da BST de pistas
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

// Insere uma pista na BST
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

// Mostra as pistas em ordem alfabetica
void exibirPistas(struct PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Funcao simples de hash
int funcaoHash(char chave[]) {
    int soma = 0;
    int i;

    for (i = 0; chave[i] != '\0'; i++) {
        soma = soma + chave[i];
    }

    return soma % TAM_HASH;
}

// Inicializa a tabela hash
void inicializarHash() {
    int i;

    for (i = 0; i < TAM_HASH; i++) {
        tabelaHash[i].ocupado = 0;
    }
}

// Insere pista e suspeito na tabela hash
void inserirNaHash(char pista[], char suspeito[]) {
    int indice = funcaoHash(pista);
    int inicio = indice;

    while (tabelaHash[indice].ocupado == 1) {
        indice = (indice + 1) % TAM_HASH;

        if (indice == inicio) {
            printf("Tabela hash cheia.\n");
            return;
        }
    }

    strcpy(tabelaHash[indice].pista, pista);
    strcpy(tabelaHash[indice].suspeito, suspeito);
    tabelaHash[indice].ocupado = 1;
}

// Procura o suspeito de uma pista
char* encontrarSuspeito(char pista[]) {
    int indice = funcaoHash(pista);
    int inicio = indice;

    while (tabelaHash[indice].ocupado == 1) {
        if (strcmp(tabelaHash[indice].pista, pista) == 0) {
            return tabelaHash[indice].suspeito;
        }

        indice = (indice + 1) % TAM_HASH;

        if (indice == inicio) {
            break;
        }
    }

    return NULL;
}

// Conta quantas pistas coletadas apontam para um suspeito
int contarPistasSuspeito(struct PistaNode *raiz, char suspeito[]) {
    int total = 0;
    char *suspeitoEncontrado;

    if (raiz == NULL) {
        return 0;
    }

    total = total + contarPistasSuspeito(raiz->esquerda, suspeito);

    suspeitoEncontrado = encontrarSuspeito(raiz->pista);
    if (suspeitoEncontrado != NULL && strcmp(suspeitoEncontrado, suspeito) == 0) {
        total++;
    }

    total = total + contarPistasSuspeito(raiz->direita, suspeito);

    return total;
}

// Fase final de julgamento
void verificarSuspeitoFinal(struct PistaNode *arvorePistas) {
    char acusado[50];
    int quantidade;

    printf("\nDigite o nome do suspeito acusado: ");
    scanf(" %[^\n]", acusado);

    quantidade = contarPistasSuspeito(arvorePistas, acusado);

    printf("\nQuantidade de pistas contra %s: %d\n", acusado, quantidade);

    if (quantidade >= 2) {
        printf("Acusacao consistente! Ha pistas suficientes contra %s.\n", acusado);
    } else {
        printf("Acusacao fraca! Nao ha pistas suficientes contra %s.\n", acusado);
    }
}

// Explora as salas e coleta pistas
void explorarSalas(struct Sala *atual, struct PistaNode **arvorePistas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);

            if (encontrarSuspeito(atual->pista) != NULL) {
                printf("Essa pista pode estar ligada a: %s\n", encontrarSuspeito(atual->pista));
            }
        } else {
            printf("Nenhuma pista nesta sala.\n");
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

// Libera memoria da arvore de salas
void liberarSalas(struct Sala *raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

// Libera memoria da arvore de pistas
void liberarPistas(struct PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

int main() {
    struct PistaNode *arvorePistas = NULL;

    // Inicializa tabela hash
    inicializarHash();

    // Associa pistas a suspeitos
    inserirNaHash("Pegadas no tapete", "Mordomo");
    inserirNaHash("Livro fora da estante", "Professor");
    inserirNaHash("Taca quebrada", "Herdeira");
    inserirNaHash("Bilhete rasgado", "Mordomo");
    inserirNaHash("Faca suja", "Cozinheira");
    inserirNaHash("Lanterna apagada", "Mordomo");

    // Cria as salas da mansao
    struct Sala *hall = criarSala("Hall de Entrada", "Pegadas no tapete");
    struct Sala *biblioteca = criarSala("Biblioteca", "Livro fora da estante");
    struct Sala *salaJantar = criarSala("Sala de Jantar", "Taca quebrada");
    struct Sala *escritorio = criarSala("Escritorio", "Bilhete rasgado");
    struct Sala *jardim = criarSala("Jardim", "");
    struct Sala *cozinha = criarSala("Cozinha", "Faca suja");
    struct Sala *porao = criarSala("Porao", "Lanterna apagada");

    // Monta a arvore binaria da mansao
    hall->esquerda = biblioteca;
    hall->direita = salaJantar;

    biblioteca->esquerda = escritorio;
    biblioteca->direita = jardim;

    salaJantar->esquerda = cozinha;
    salaJantar->direita = porao;

    printf("=== DETECTIVE QUEST ===\n");
    printf("Explore a mansao, colete pistas e faca sua acusacao final.\n");

    explorarSalas(hall, &arvorePistas);

    printf("\n=== PISTAS COLETADAS EM ORDEM ALFABETICA ===\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(arvorePistas);
    }

    if (arvorePistas != NULL) {
        verificarSuspeitoFinal(arvorePistas);
    }

    liberarSalas(hall);
    liberarPistas(arvorePistas);

    return 0;
}
