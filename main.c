#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int id;
    char cpf[12];
    char nome[100];
    char sobrenome[100];
} Pessoa;

typedef struct no {
    Pessoa *pessoa;
    struct no *esquerdo, *direito;
    int altura;
}No;

typedef struct{
int **adjacencia;
 Pessoa x;
 } GrafoAmizade;

No* novoNo(Pessoa *x){
    No *novo = malloc(sizeof(No));

    if(novo){
        novo->pessoa = x;
        novo->esquerdo = NULL;
        novo->direito = NULL;
        novo->altura = 0;
    }
    else{
        printf("\nERRO ao alocar nó em novoNo !\n");
    }

    return novo;
}

int maior(int a, int b){
    return (a > b)? a: b;
}

int alturaDoNo(No *no){
    if(no == NULL)
        return -1;
    else
        return no->altura;
}

int fatorDeBalanceamento(No *no){
    if(no)
        return (alturaDoNo(no->esquerdo) - alturaDoNo(no->direito));
    else
        return 0;
}

No* rotacaoEsquerda(No *r){
    No *y, *f;

    y = r->direito;
    f = y->esquerdo;

    y->esquerdo = r;
    r->direito = f;

    r->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;
    y->altura = maior(alturaDoNo(y->esquerdo), alturaDoNo(y->direito)) + 1;

    return y;
}

No* rotacaoDireita(No *r){
    No *y, *f;

    y = r->esquerdo;
    f = y->direito;

    y->direito = r;
    r->esquerdo = f;

    r->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;
    y->altura = maior(alturaDoNo(y->esquerdo), alturaDoNo(y->direito)) + 1;

    return y;
}

No* rotacaoDireitaEsquerda(No *r){
    r->direito = rotacaoDireita(r->direito);
    return rotacaoEsquerda(r);
}

No* rotacaoEsquerdaDireita(No *r){
    r->esquerdo = rotacaoEsquerda(r->esquerdo);
    return rotacaoDireita(r);
}

No* balancear(No *raiz){
    short fb = fatorDeBalanceamento(raiz);

    //Rotação à esquerda
    if(fb < -1 && fatorDeBalanceamento(raiz->direito) <= 0)
        raiz = rotacaoEsquerda(raiz);

    //Rotação à direita
    else if(fb > 1 && fatorDeBalanceamento(raiz->esquerdo) >= 0)
        raiz = rotacaoDireita(raiz);

    //Rotação dupla à esquerda
    else if(fb > 1 && fatorDeBalanceamento(raiz->esquerdo) < 0)
        raiz = rotacaoEsquerdaDireita(raiz);

    //Rotação dupla à direita
    else if(fb < -1 && fatorDeBalanceamento(raiz->direito) > 0)
        raiz = rotacaoDireitaEsquerda(raiz);

    return raiz;
}

No* inserir(No *raiz, Pessoa *x){
    if(raiz == NULL) //árvore vazia
        return novoNo(x);
    else{
        if(x->id < raiz->pessoa->id)
            raiz->esquerdo = inserir(raiz->esquerdo, x);
        else if(x->id > raiz->pessoa->id)
            raiz->direito = inserir(raiz->direito, x);
        else
            printf("\nInsercao nao realizada!\nO ID %d ja existe!\n", x->id);
    }

    //Recalcular a altura de todos os nós
    raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito)) + 1;

    //Verifica a necessidade de rebalancear a árvore
    raiz = balancear(raiz);

    return raiz;
}

No* remover(No *raiz, int chave){
    if(raiz == NULL){
        printf("Valor nao encontrado!\n");
        return NULL;
    } else { //procura o nó a remover
        if(raiz->pessoa->id == chave){
            //remove nós folhas(nós sem filhos)
            if(raiz->esquerdo == NULL && raiz->direito == NULL){
                free(raiz);
                printf("Elemento folha removido: %d !\n", chave);
                return NULL;
            }
            else{
                //remove nós que possuem 2 filhos
                if(raiz->esquerdo != NULL && raiz->direito != NULL){
                    No *aux = raiz->esquerdo;
                    while(aux->direito != NULL)
                        aux = aux->direito;
                    Pessoa *pessoaAux;
                    pessoaAux = raiz->pessoa;
                    raiz->pessoa = aux->pessoa;
                    aux->pessoa = pessoaAux;
                    printf("Elemento trocado: %d !\n", chave);
                    raiz->esquerdo = remover(raiz->esquerdo, chave);
                    return raiz;
                }
                else{
                    //remove nós que possuem apenas 1 filho
                    No *aux;
                    if(raiz->esquerdo != NULL)
                        aux = raiz->esquerdo;
                    else
                        aux = raiz->direito;
                    free(raiz);
                    printf("Elemento com 1 filho removido: %d !\n", chave);
                    return aux;
                }
            }
        } else {
            if(chave < raiz->pessoa->id)
                raiz->esquerdo = remover(raiz->esquerdo, chave);
            else
                raiz->direito = remover(raiz->direito, chave);
        }

       //Recalcular a altura de todos os nós
    raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito)) + 1;

    //Verifica a necessidade de rebalancear a árvore
    raiz = balancear(raiz);

    return raiz;
    }
}

void imprimirPessoa(Pessoa *pessoa){
    printf("Nome: %s %s CPF: %s ID: %d\n", pessoa->nome, pessoa->sobrenome, pessoa->cpf, pessoa->id);
}

void imprimirAvl(No *raiz, int nivel){
    int i;
    if(raiz){
        imprimir(raiz->direito, nivel + 1);
        printf("\n\n");

        for(i = 0; i < nivel; i++)
            printf("\t");

        imprimirPessoa(raiz->pessoa);
        imprimir(raiz->esquerdo, nivel + 1);
    }
}

/*void imprimir(No *raiz){
    if(raiz){
        imprimirPessoa(raiz->pessoa);
        imprimir(raiz->esquerdo);
        imprimir(raiz->direito);
    }
} */


int main()
{
    int ultimoIdPessoa = 0;
    int opcao, valor;
    No *raiz = NULL;
    Pessoa *p;

    do{
        printf("\n\n\t0 - Sair\n\t1 - Inserir\n\t2 - Remover\n\t3 - Imprimir\n\n");
        scanf("%d", &opcao);

        switch(opcao){
        case 0:
            printf("Saindo!!!");
            break;
        case 1:
            p = malloc(sizeof(Pessoa));
            printf("\tDigite o nome: ");
            scanf("%s", p->nome);
            printf("\tDigite o sobrenome: ");
            scanf("%s", &p->sobrenome);
            printf("\tDigite o CPF: ");
            scanf("%s", &p->cpf);
            p->id = ultimoIdPessoa;
            ultimoIdPessoa++;
            raiz = inserir(raiz, p);
            break;
        case 2:
            printf("\tDigite o ID a ser removido: ");
            scanf("%d", &valor);
            raiz = remover(raiz, valor);
            break;
        case 3:
            imprimirAvl(raiz);
            break;
        default:
            printf("\nOpcao invalida!!!\n");
        }

    } while(opcao != 0);

    return 0;
}
