/*
Crie uma struct chamada livros que que receba Registro, título, autor, quantidade no estoque

Use uma lista encadeada para armazenar os livros

Crie a função adicionar livro e remover livro da lista, se o livro for removido, peça ao usuário para escrever o motivo

Mostre a lista completa

Crie a função alterar livro usando seu código através do seu código de registro

Crie uma lista a parte, para poder mostrar quais livros foram deletados e por que

Escreva a lista atual de livros em estoque em um arquivo .txt ->Segue logica de programação criada para montangem do codigo usando chat gpt */

// Definição da struct livro
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIVROS 100

typedef struct Livro {
    int registro;
    char titulo[100];
    char autor[100];
    int quantidade;
    struct Livro *proximo;
} Livro;

//verifica a existencia de registros de algum livro preexistente
int verificarRegistroExistente(Livro *inicio, int registro) {
    Livro *livro = inicio;

    while (livro != NULL) {
        if (livro->registro == registro) {
            return 1; // Registro já existe
        }
        livro = livro->proximo;
    }

    return 0; // Registro não existe
}

//adiciona mais um elemento livro
void adicionarLivro(Livro **inicio) {
    if (*inicio != NULL && (*inicio)->registro >= MAX_LIVROS) {
        printf("O limite máximo de livros foi atingido.\n");
        return;
    }

    Livro *novoLivro = (Livro *)malloc(sizeof(Livro));

    printf("Digite o registro do livro: ");
    scanf("%d", &novoLivro->registro);

    if (verificarRegistroExistente(*inicio, novoLivro->registro)) {
        printf("Já existe um livro com esse registro. Digite outro registro.\n");
        free(novoLivro);
        return;
    }

    printf("Digite o título do livro: ");
    scanf(" %[^\n]", novoLivro->titulo);

    printf("Digite o autor do livro: ");
    scanf(" %[^\n]", novoLivro->autor);

    printf("Digite a quantidade em estoque: ");
    scanf("%d", &novoLivro->quantidade);

    novoLivro->proximo = NULL;

    if (*inicio == NULL) {
        *inicio = novoLivro;
    } else {
        Livro *temp = *inicio;
        while (temp->proximo != NULL) {
            temp = temp->proximo;
        }
        temp->proximo = novoLivro;
    }

    printf("Livro adicionado com sucesso!\n");
}

//retira um elemento livro e pergunta motivo da remoção
void removerLivro(Livro **inicio, Livro **removidos) {
    if (*inicio == NULL) {
        printf("A lista de livros está vazia.\n");
        return;
    }

    int registro;
    printf("Digite o registro do livro a ser removido: ");
    scanf("%d", &registro);

    Livro *anterior = NULL;
    Livro *atual = *inicio;

    while (atual != NULL) {
        if (atual->registro == registro) {
            if (anterior == NULL) {
                *inicio = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }

            printf("Digite o motivo da remoção: ");
            char motivo[100];
            scanf(" %[^\n]", motivo);

            Livro *livroRemovido = (Livro *)malloc(sizeof(Livro));
            livroRemovido->registro = atual->registro;
            strcpy(livroRemovido->titulo, atual->titulo);
            strcpy(livroRemovido->autor, atual->autor);
            livroRemovido->quantidade = atual->quantidade;
            livroRemovido->proximo = NULL;

            if (*removidos == NULL) {
                *removidos = livroRemovido;
            } else {
                Livro *temp = *removidos;
                while (temp->proximo != NULL) {
                    temp = temp->proximo;
                }
                temp->proximo = livroRemovido;
            }

            free(atual);
            printf("Livro removido com sucesso!\n");
            return;
        }

        anterior = atual;
        atual = atual->proximo;
    }

    printf("Nenhum livro encontrado com o registro fornecido.\n");
}

//exibe a lista 
void exibirLista(Livro *inicio) {
    if (inicio == NULL) {
        printf("A lista de livros está vazia.\n");
        return;
    }

    Livro *livro = inicio;
    printf("===== Lista de Livros =====\n");
    while (livro != NULL) {
        printf("Registro: %d\n", livro->registro);
        printf("Título: %s\n", livro->titulo);
        printf("Autor: %s\n", livro->autor);
        printf("Quantidade em estoque: %d\n", livro->quantidade);
        printf("---------------------------\n");
        livro = livro->proximo;
    }
}

//salva a lista num arquivo txt que o proprio codigo gera
void salvarListaEmArquivo(Livro *inicio, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    Livro *livro = inicio;

    while (livro != NULL) {
        fprintf(arquivo, "Registro: %d\n", livro->registro);
        fprintf(arquivo, "Título: %s\n", livro->titulo);
        fprintf(arquivo, "Autor: %s\n", livro->autor);
        fprintf(arquivo, "Quantidade em estoque: %d\n", livro->quantidade);
        fprintf(arquivo, "---------------------------\n");
        livro = livro->proximo;
    }

    fclose(arquivo);
    printf("Lista de livros salva no arquivo com sucesso!\n");
}

//le lista de arquivo existente
void lerListaDeArquivo(Livro **inicio, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Arquivo não encontrado. Continuando com uma lista vazia.\n");
        return;
    }

    Livro *livroAnterior = NULL;
    Livro *livroAtual = NULL;

    while (!feof(arquivo)) {
        livroAtual = (Livro *)malloc(sizeof(Livro));

        fscanf(arquivo, "Registro: %d\n", &livroAtual->registro);
        fscanf(arquivo, "Título: %[^\n]\n", livroAtual->titulo);
        fscanf(arquivo, "Autor: %[^\n]\n", livroAtual->autor);
        fscanf(arquivo, "Quantidade em estoque: %d\n", &livroAtual->quantidade);

        livroAtual->proximo = NULL;

        if (*inicio == NULL) {
            *inicio = livroAtual;
        } else {
            livroAnterior->proximo = livroAtual;
        }

        livroAnterior = livroAtual;
    }

    fclose(arquivo);
    printf("Lista de livros lida do arquivo com sucesso!\n");
}

//libera memoria alocada temporariamente
void liberarLista(Livro **inicio) {
    Livro *livro = *inicio;
    Livro *temp;

    while (livro != NULL) {
        temp = livro;
        livro = livro->proximo;
        free(temp);
    }

    *inicio = NULL;
    printf("Lista de livros liberada da memória com sucesso!\n");
}

int main() {
    Livro *listaLivros = NULL;
    Livro *listaRemovidos = NULL;

    char nomeArquivo[100];

    printf("Digite o nome do arquivo: ");
    scanf(" %[^\n]", nomeArquivo);

    lerListaDeArquivo(&listaLivros, nomeArquivo);

    int opcao;

    do {
        printf("\n===== MENU =====\n");
        printf("1. Adicionar livro\n");
        printf("2. Remover livro\n");
        printf("3. Exibir lista de livros\n");
        printf("4. Salvar lista de livros em arquivo\n");
        printf("5. Sair\n");
        printf("Digite a opção desejada: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarLivro(&listaLivros);
                break;
            case 2:
                removerLivro(&listaLivros, &listaRemovidos);
                break;
            case 3:
                exibirLista(listaLivros);
                break;
            case 4:
                salvarListaEmArquivo(listaLivros, nomeArquivo);
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Digite novamente.\n");
                break;
        }
    } while (opcao != 5);

    liberarLista(&listaLivros);
    liberarLista(&listaRemovidos);

    return 0;
}
