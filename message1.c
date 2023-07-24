#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

typedef struct {
	int codigo;
    char descricao[200];
    int qtd_estoque;
    
    int preco;
    int preco_decimal; //Resolver bug do valor com .
} Produto;

typedef struct {
	int codigo;
	int quantidade;
} Carrinho;

static void menuGerenciamento();
static void clear();
static char* opcoesPrincipais();
static void menuPrincipal();
static void menuGerenciamentoPedido();
static void esvaziarCarrinho();

/**
* Declarando as propriedades do nosso vetor de produtos.
*/
static int tamanhoMaximo = 10;
static int tamanhoAtual = 0;
static Produto **produto;

/**
* Declarando as propriedades do nosso vetor de produtos.
*/
static int tamanhoAtualCarrinho = 0;
static Carrinho **carrinho;

void clear() {
	system("cls");
} //Modularização

static bool produtoExiste(int codigo) {
	int i;
	for (i = 0; i <= tamanhoAtual; i++) {
		Produto *prod = produto[i];
    	if (prod->codigo == codigo) return true;
	}
	return false;
}

static Produto* getProduto(int codigo) {
	int i;
	for (i = 0; i <= tamanhoAtual; i++) {
		Produto *prod = produto[i];
    	if (prod->codigo == codigo) return prod;
	}
	return NULL;
}

static void sendMessage(char* message[200]) {
	printf(message);
	sleep(2);
	clear();
}

static char* opcoesGerenciamento() { 
	clear();
	
	char *opcaoGem;
	printf("\n######################");
	printf("\nGerenciar Produto");
	printf("\n######################\n");
	printf("\n(1) Incluir");
	printf("\n(2) Alterar");
	printf("\n(3) Listar"); 
	printf("\n(4) Consultar");
	printf("\n(5) Excluir");
	printf("\n(6) Voltar"); 
	printf("\n\nEscolha uma opção: ");
	scanf("%c", &opcaoGem);
	getchar(); //Limpa o scanf (bug)
	
	return &opcaoGem;
}

static void incluirProduto() {
	if (tamanhoAtual >= tamanhoMaximo) {
	    tamanhoMaximo += 10;
	    produto = realloc(produto, tamanhoMaximo * sizeof(Produto));
	}
	
	int codigoIncluir;
    produto[tamanhoAtual] = malloc(sizeof(Produto));

	printf("\nInforme o Código do produto: ");
	scanf("%d", &codigoIncluir);
	getchar();
	
	printf("Informe a Descrição do produto: ");
	scanf("%[^\n]s", &produto[tamanhoAtual]->descricao);
	getchar();
	
	printf("Informe a Quantidade em estoque desse produto: ");
	scanf("%d", &produto[tamanhoAtual]->qtd_estoque);
	getchar();
	
	printf("Informe o Preço desse produto: ");
	scanf("%d.%d", &produto[tamanhoAtual]->preco, &produto[tamanhoAtual]->preco_decimal);
	getchar();
	
	if (produtoExiste(codigoIncluir)) {
		sendMessage("Esse produto já está registrado!");
		menuGerenciamento();
		free(produto[tamanhoAtual]);
		return;
	}
	
	produto[tamanhoAtual]->codigo = codigoIncluir;
	tamanhoAtual+=1;
	
	sendMessage("\nProduto adicionado com sucesso!\n");
	menuGerenciamento();
}

static void consultarProduto() {
	int j, codigo;
	printf("Digite o Código do produto que deseja consultar: ");
    scanf("%d", &codigo);
    getchar();
    
    clear();
    for (j = 0; j <= tamanhoAtual; j++) {
    	Produto *prod = produto[j];
    	
    	if (prod->codigo == codigo) {
		    printf("\n===== Informacoes do produto =====\n");
		    printf("Codigo: %d \n", prod->codigo);
		    printf("Descricao: %s \n", prod->descricao);
		    printf("Quantidade: %d \n", prod->qtd_estoque);
		    printf("Preço: %d.%d", prod->preco, prod->preco_decimal);
		    
		    sleep(3); //ATENÇÃO: BOTAR PRA CLICAR ALGUMA PARADA PRA VOLTAR
		    menuGerenciamento();
		    return;
		}
	}
    
    sendMessage("Produto não encontrado! Tente novamente...");
	menuGerenciamento();
}

static void alterarProduto() {
	int j, codAlterar;
	printf("Digite o Código do produto que deseja alterar: ");
	scanf("%d", &codAlterar);
	getchar();
    
	clear();
	for (j = 0; j <= tamanhoAtual; j++) {
		Produto *prod = produto[j];
		
    	if (prod->codigo == codAlterar) {
    		int codigoIncluir;
    		
			printf("\nInforme o novo Código do produto: ");
			scanf("%d", &codigoIncluir);
			getchar();
			
			if (produtoExiste(codigoIncluir)) {
				sendMessage("Já existe um produto com esse código!");
				menuGerenciamento();
				return;
			}

			printf("Informe a nova Descrição do produto: ");
			scanf("%[^\n]s", &prod->descricao);
			getchar();

			printf("Informe a nova Quantidade em estoque desse produto: ");
			scanf("%d", &prod->qtd_estoque);
			getchar();
			
			printf("Informe o novo Preço desse produto: ");
			scanf("%d.%d", &prod->preco, &prod->preco_decimal);
			getchar();
			
			prod->codigo = codigoIncluir;
			
			sendMessage("Produto alterado!");
			menuGerenciamento();
			return;
		}
	}
			
	sendMessage("Produto não encontrado! Tente novamente...");
	menuGerenciamento();
}

static void listarProdutos() {
	int i = 0;
	
	if (tamanhoAtual == 0) {
		sendMessage("Nenhum produto encontrado! Tente adicionar um novo produto.");
		return;
	}
	
	for (i = 0; i <= tamanhoAtual-1; i++) {
		Produto *prod = produto[i];
		
		printf("\n===== Informacoes do produto de código %d =====\n", prod->codigo);
	    printf("Codigo: %d \n", prod->codigo);
	    printf("Descricao: %s \n", prod->descricao);
	    printf("Quantidade: %d \n", prod->qtd_estoque);
	    printf("Preço: %d.%d", prod->preco, prod->preco_decimal);
		
	}
	sleep(3);
	menuGerenciamento();
}

static void excluirProduto() {
	int i;
	int index = -1;
	int codExcluir;
	
	printf("Digite o Código do produto que deseja excluir: ");
	scanf("%d", &codExcluir);
	getchar();
	
	
	/**
	* Encontrando o index para remover do vetor.
	*/
	for(i = 0; i < tamanhoAtual; i++) {
		Produto *prod = produto[i];
        if(prod->codigo == codExcluir) {
            index = i;
            break;
        }
	}
	
	/**
	* Caso encontre algum produto, vamos remover ele da memoria e
	* fazer os produtos seguintes voltar uma posição no vetor.
	*/
	if (index != -1) {
		free(produto[index]);
		for(i = index; i < tamanhoAtual; i++) {
			if (i == tamanhoAtual) {
				free(produto[i]);
				continue;
			}
			produto[i] = produto[i+1];
		}
		tamanhoAtual-=1;
		
		sendMessage("Produto removido com sucesso!");
		menuGerenciamento();
		return;
	}
	
	sendMessage("Produto não encontrado! Tente novamente...");
	menuGerenciamento();
}

void menuGerenciamento() {
	char* opcao = *opcoesGerenciamento();
	
	if (opcao == '1') {
		incluirProduto();
		return;
	}
	
	if (opcao == '2') {
		alterarProduto();
		return;
	}
	
	if (opcao == '3') {
		listarProdutos();
		return;
	}
	
	if (opcao == '4') {
		consultarProduto();
		return;
	}
	
	if (opcao == '5') {
		excluirProduto();
		return;
	}
	
	if (opcao == '6') {
		menuPrincipal();
		return;
	}
	
	sendMessage("Opção invalida!");
	fflush(stdin);
	menuGerenciamento();
}

static char* opcoesGerenciamentoPedido() {
	clear();
	char *opcao;
	
	printf("\n######################");
	printf("\nGerenciar Pedido");
	printf("\n######################\n");	
	printf("\n(1) Adicionar produto no carrinho");
	printf("\n(2) Consultar carrinho de compras");
	printf("\n(3) Excluir produto do carrinho");
	printf("\n(4) Alterar quantidade do produto do carrinho");	
	printf("\n(5) Finalizar Pedido");	
	printf("\n(6) Esvaziar carrinho de compras");
	printf("\n(7) Voltar");
	printf("\n\nDigite uma opcao: ");
	scanf("%c",&opcao);	
	
	return &opcao;
}

static void adicionarProdutoCarrinho() {
	int codigoIncluir, quantidade;

	printf("\nInforme o Código do produto: ");
	scanf("%d", &codigoIncluir);
	getchar();
	
	Produto *produto = getProduto(codigoIncluir);
	
	if (!produto) {
		sendMessage("Este produto não existe!");
		menuGerenciamentoPedido();
		return;
	}
	
	printf("Informe a quantidade do produto: ");
	scanf("%d", &quantidade);
	getchar();
	
	if (quantidade <= 0) {
		sendMessage("Quantidade deve ser maior que zero.");
		menuGerenciamentoPedido();
		return;
	}
	
	carrinho[tamanhoAtualCarrinho] = malloc(sizeof(Carrinho));
	
	carrinho[tamanhoAtualCarrinho]->codigo = codigoIncluir;
	carrinho[tamanhoAtualCarrinho]->quantidade = quantidade;
	
	tamanhoAtualCarrinho+=1;
	
	sendMessage("Produto adicionado com sucesso!");
	menuGerenciamentoPedido();
}

static void consultarProdutosCarrinho() {
    int i = 0;

    if (tamanhoAtual == 0) {
        sendMessage("Nenhum produto encontrado! Tente adicionar um novo produto.");
        getchar();
        menuGerenciamentoPedido();
        return;
    }
	printf("=-=- Produtos do Carrinho -=-=\n");
    for (i = 0; i <= tamanhoAtualCarrinho-1; i++) {
        Carrinho *car = carrinho[i];
        printf("\n===== Produto de código %d =====\n", car->codigo);
        printf("Codigo: %d \n", car->codigo);
        printf("Quantidade: %d \n", car->quantidade);

    }
    sleep(3);
    getchar();
    menuGerenciamentoPedido();
}

static void excluirProdutoCarrinho() {
	int i;
	int index = -1;
	int codExcluir;
	
	printf("Digite o Código do produto que deseja excluir: ");
	scanf("%d", &codExcluir);
	getchar();
	
	
	/**
	* Encontrando o index para remover do vetor.
	*/
	for(i = 0; i < tamanhoAtualCarrinho; i++) {
		Carrinho *car = carrinho[i];
        if(car->codigo == codExcluir) {
            index = i;
            break;
        }
	}
	
	/**
	* Caso encontre algum produto, vamos remover ele da memoria e
	* fazer os produtos seguintes voltar uma posição no vetor.
	*/
	if (index != -1) {
		free(carrinho[index]);
		for(i = index; i < tamanhoAtualCarrinho; i++) {
			if (i == tamanhoAtualCarrinho) {
				free(carrinho[i]);
				continue;
			}
			carrinho[i] = carrinho[i+1];
		}
		tamanhoAtualCarrinho-=1;
		
		sendMessage("Produto removido do carrinho com sucesso!");
		menuGerenciamentoPedido();
		return;
	}
	
	sendMessage("Produto não encontrado no carrinho! Tente novamente...");
	menuGerenciamentoPedido();
}

static void alterarQuantidadeCarrinho() {
	int j, codAlterar;
	printf("Digite o Código do produto que deseja alterar: ");
	scanf("%d", &codAlterar);
	getchar();
    
	for (j = 0; j <= tamanhoAtualCarrinho; j++) {
		Carrinho *car = carrinho[j];
		
    	if (car->codigo == codAlterar) {

			printf("Informe a nova Quantidade que deseja comprar: ");
			scanf("%d", &car->quantidade);
			sendMessage("Quantidade Alterada!");
			getchar();
			menuGerenciamentoPedido();
			return;
		}
	}
			
	sendMessage("Produto não encontrado! Tente novamente...");
	getchar();
	menuGerenciamentoPedido();
}

static void finalizarPedido(){
	if (tamanhoAtualCarrinho <= 0) {
		sendMessage("Você não possui nenhum produto em seu carrinho.");
		menuGerenciamentoPedido();
		return;
	}
	
	int i;
	
	for (i = 0; i <= tamanhoAtualCarrinho-1; i++) {
		Carrinho *car = carrinho[i];
		
		if (car->codigo) {
			Produto *prod = getProduto(car->codigo);
			
			if (prod) {
				if (prod->qtd_estoque >= car->quantidade) {
					prod->qtd_estoque-=car->quantidade;
				
					int idProduto = rand();
					srand(time(0));
					
					printf("\nPedido finalizado! Código de identificação: %d", idProduto);	
				} else {
					printf("\nProduto %s não possui quantidade em estoque suficiente.", prod->descricao);
				}
			}
		}
	}
	
	sleep(3);
	fflush(stdin);
	menuGerenciamentoPedido();
}

void esvaziarCarrinho() {
	int i = tamanhoAtualCarrinho;
	
    while (i != 0) {
    free(carrinho[i]);
    i--;
    tamanhoAtualCarrinho--;
    }
	sendMessage("Todos os produtos foram removidos!");
	getchar();
	menuGerenciamentoPedido();
	return;
}

void menuGerenciamentoPedido() {
	char* opcao = *opcoesGerenciamentoPedido();
	
	if (opcao == '1') {
		adicionarProdutoCarrinho();//feito
		return;
	}
	if (opcao == '2') {
		consultarProdutosCarrinho();//feito
		return;
	}
	if (opcao == '3') {
		excluirProdutoCarrinho();//feito
		return;
	}
	if (opcao == '4') {
		alterarQuantidadeCarrinho();//feito
		return;
	}
	if (opcao == '5') {
		finalizarPedido();
		return;
	}
	if (opcao == '6') {
		esvaziarCarrinho();
		return;
	}
	if (opcao == '7') {
		menuPrincipal();
		return;
	}
	
	sendMessage("Opção invalida!");
	fflush(stdin);
	menuGerenciamentoPedido();	
}

char* opcoesPrincipais() { 
	clear();
	char *opcao;
	printf("\n(1) Gerenciar Produto");
	printf("\n(2) Gerenciar Pedido");
	printf("\n(3) Sair");
	printf("\n\nEscolha uma opção: ");
	scanf("%c", &opcao);
	getchar();
	
	return &opcao;
}

void menuPrincipal() {
	char* opcao = *opcoesPrincipais();
	
	if (opcao == '1') {
		menuGerenciamento();
		return;
	}
	
	if (opcao == '2') {
		menuGerenciamentoPedido();
		return;
	}
	
	if (opcao == '3') {
		exit(EXIT_SUCCESS);
		return;
	}
	
	sendMessage("Opção invalida!");
	fflush(stdin);
	menuPrincipal();
}

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "");
	
	produto = (Produto*) malloc(tamanhoMaximo * sizeof(Produto));
	carrinho = (Carrinho*) malloc(tamanhoMaximo * sizeof(Carrinho));

	
	menuPrincipal();
    return 0;
}