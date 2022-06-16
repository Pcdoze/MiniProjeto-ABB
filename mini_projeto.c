#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

// Definindo a estrtura do nó (arvore)
typedef struct Elemento{
	int     rgm;
    char *  nome;

} t_elemento;
typedef struct no{
	struct no * esq;
	struct Elemento * info;
	struct no * dir;
} t_no;

typedef t_no * t_arvore;

// Cria um no vazio
t_no * criar ()
{
    t_no * no = (t_no *) malloc(sizeof(t_no));

    if (no)
        no->esq = no->dir = NULL;

        t_elemento temp;
        temp.rgm = -1;

        no->info = &temp;

    return no;
}

// Verifica se árvore está vazia
int isVazia (t_arvore tree){
	return (tree == NULL);
}

void exibirPreOrdem(t_arvore tree) {
    if (tree!=NULL) {
        printf("%d ", tree->info->rgm);
        exibirPreOrdem(tree->esq);
        exibirPreOrdem(tree->dir);
    }
}

void exibirInOrdem(t_arvore tree) {
    if (tree!=NULL) {
        exibirInOrdem(tree->esq);
        printf("%d ", tree->info->rgm);
        exibirInOrdem(tree->dir);
    }
}

void exibirPosOrdem(t_arvore tree) {
    if (tree!=NULL) {
        exibirPosOrdem(tree->esq);
        exibirPosOrdem(tree->dir);
        printf("%d ", tree->info->rgm);
    }
}

void gotoxy(int coluna, int linha)
{
   COORD point;
   point.X = coluna;
   point.Y = linha;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

// Exibir a arvore - Procedimento recursivo, usando um percurso pre-ordem.
// sugestao de uso: exibirGraficamente(arvore, 10, 10, 3);

void exibirGraficamente(t_arvore tree, int col, int lin, int desloc)
{
    // col e lin sao as coordenadas da tela onde a arvore ira iniciar,
    // ou seja, a posicao da raiz, e desloc representa o deslocamento na tela
    // (em colunas) de um no em relacao ao no anterior.
    if (tree == NULL)
        return; // condicao de parada do procedimento recursivo

    gotoxy(col,lin);

    printf("%d ", tree->info->rgm);
    if (tree->esq != NULL)
        exibirGraficamente(tree->esq,col-desloc,lin+2,desloc/2+1);

    if (tree->dir != NULL)
        exibirGraficamente(tree->dir,col+desloc,lin+2,desloc/2+1);
}

int inserir (t_arvore * tree, int rgm, char *nome)
{
    int ok;
    // se a raiz for nula, entao insere a raiz
    if (*tree == NULL) {

        *tree = criar();
        if (*tree == NULL)
            return 0;

        t_elemento * el = (t_elemento*) malloc(sizeof(t_elemento));

        if (el){
            el->rgm = rgm;

            el->nome = nome;
        }
        else
            return 0;

        (*tree)->info = el;

        return 1;
    }

    if (rgm > (*tree)->info->rgm)
        ok = inserir (&((*tree)->dir), rgm, nome);
    else
        if (rgm < (*tree)->info->rgm)
            ok = inserir (&((*tree)->esq), rgm, nome);
        else
            ok = 0;

	return ok;
}

t_no * busca(t_arvore tree, int rgm)
{
    if (tree == NULL){
    	printf ("Nao encontrado");
        return NULL;
	}
    if (tree->info->rgm == rgm){
    	printf("%d - %s foi encontrado\n", rgm, tree->info->nome);
        return tree;
	}
    if (rgm < tree->info->rgm)
        return busca(tree->esq, rgm);
    else
        return busca(tree->dir, rgm);
}

t_no * buscaSetPai(t_arvore tree, int rgm, t_no ** pai)
{
    if (tree == NULL) {
        *pai = NULL;
        return NULL;
    }
    if (rgm == tree->info->rgm)
        return tree;

    if (rgm < tree->info->rgm) {
        *pai = tree;
        return buscaSetPai(tree->esq, rgm, pai);
    }
    else {
        *pai = tree;
        return buscaSetPai(tree->dir, rgm, pai);
    }
}

int remover (t_arvore * tree, int rgm) {
    t_no 	* no, // no aponta para o no a ser removido
    		* pai, // pai aponta para o pai do no
    		* sub, // sub aponta que ira substituir o no no
    		* paiSuce, // pai do no sucessor
    		* suce; // sucessor do no no

    no = *tree; pai=NULL;

    // procura o no a ser removido, e seta o seu pai.
    no = buscaSetPai(*tree, rgm, &pai);
    if (no == NULL){
    	printf("Nao encontrado");
        return 0; // a chave nao existe na arvore, nao conseguiu remover
	}

    // agora iremos ver os dois primeiros casos, o no tem um filho no maximo
    if (no->esq == NULL)
        sub = no->dir;
    else {
        if (no->dir == NULL)
           sub = no->esq;
        else { // caso em que o no tem dois filhos
	        paiSuce = no;
	        sub = no->dir;
	        suce = sub->esq;   // suce eh sempre o filho esq de sub
	        while (suce != NULL) {
	              paiSuce = sub;
	              sub = suce;
	              suce = sub->esq;
	        }
	        // neste ponto, sub eh o sucessor em ordem de no
	        if (paiSuce != no) {
	           //  no nao e o pai de sub, e sub == paiSuce->esq
	           paiSuce->esq = sub->dir;
	           // remove o no sub de sua atual posicao e o
	           // substitui pelo filho direito de sub
	           // sub ocupa o lugar de no
	           sub->dir = no->dir;
	        }
	        // define o filho esquerdo de sub de modo que sub
	        // ocupe o lugar de no
	        sub->esq = no->esq;
		}
	}
    // insere sub na posicao ocupada anteriormente por no
    if (pai == NULL) // no eh a raiz, nao tem pai
       *tree = sub;
    else
        // verifica se o no eh o filho da esquerda ou da direita
        if (no == pai->esq)
            pai->esq = sub;
        else
            pai->dir = sub;

    free(no); // libera o no

    return 1; // verdadeiro, conseguiu remover
}
int getRGM(){
    int rgm;
    printf("\nDigite um rgm: ");
    scanf("%d", &rgm);
    fflush(stdin);

    return rgm;
}
char *getNome(){
    char *nome = malloc (sizeof (char) * 50);
    printf("\nDigite um nome: ");
    scanf("%s", nome);
    fflush(stdin);

    return nome;
}
int loop_principal(t_arvore arvore){
    int escolha;
    printf("\n\nEscolha uma opção: ");
    printf("\n1 - INSERIR"
              "\n2 - REMOVER UM NÓ"
              "\n3 - PESQUISAR"
              "\n4 - ESVAZIAR A ÁRVORE"
              "\n5 - EXIBIR A ÁRVORE"
              "\n0 - SAIR\n");

    int isint = scanf("%d", &escolha);
    fflush(stdin);

    if (isint != 1) {
        printf("\nEntrada Inválida!");
        loop_principal(arvore);
    }

    switch(escolha){
    case 1:
        inserir(&arvore, getRGM(), getNome());

        loop_principal(arvore);
        break;
    case 2:
        if(remover(&arvore, getRGM())){
            printf("nó removido!");
        }

        loop_principal(arvore);
        break;
    case 3:
        busca(arvore, getRGM());

        loop_principal(arvore);
        break;
    case 4:
        loop_principal(arvore);
        break;
    case 5:
        system("cls");
        exibirGraficamente(arvore, 15 , 5 , 4);

        loop_principal(arvore);
        break;
    case 0:
        break;

    default:
        printf("Opção %d não disponível", escolha);
        loop_principal(arvore);
        break;

    }
}
int main ()
{
    setlocale(LC_ALL, "Portuguese");

    t_arvore arvore = NULL;
    loop_principal(arvore);

    /*
	t_arvore arvore = NULL;


	inserir(&arvore, 35);
	inserir(&arvore, 15);
	inserir(&arvore, 30);
	inserir(&arvore, 8);
	inserir(&arvore, 45);
	inserir(&arvore, 10);
	inserir(&arvore, 43);
	inserir(&arvore, 55);
	inserir(&arvore, 2);

	exibirPreOrdem(arvore); printf("\n");
	exibirInOrdem(arvore); printf("\n");
	exibirPosOrdem(arvore); printf("\n");

	exibirGraficamente(arvore, 15 , 5 , 4); printf("\n\n\n\n");

	busca(arvore, 15);

	remover(&arvore, 15);

	exibirGraficamente(arvore, 15 , 15 , 4); printf("\n");

	getchar();
	*/
}
