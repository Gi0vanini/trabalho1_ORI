/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Organização de Recuperação da Informação
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 01
 *
 * RA: 743541
 * Aluno: Giovanni Marçon Rossi
 * ========================================================================== */

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Tamanho dos campos dos registros */
#define TAM_PRIMARY_KEY 11
#define TAM_NOME 		31
#define TAM_GENERO 		2
#define TAM_NASCIMENTO 	11
#define TAM_CELULAR 	16
#define TAM_VEICULO 	31
#define TAM_PLACA 		9
#define TAM_DATA 		9
#define TAM_HORA 		6
#define TAM_TRAJETO 	121
#define TAM_VALOR 		7
#define TAM_VAGAS 		2
#define TAM_REGISTRO 	256
#define MAX_REGISTROS 	1000
#define MAX_TRAJETOS 	30
#define TAM_ARQUIVO 	(MAX_REGISTROS * TAM_REGISTRO + 1)

/* Saídas do usuário */
#define OPCAO_INVALIDA 				"Opcao invalida!\n"
#define MEMORIA_INSUFICIENTE 		"Memoria insuficiente!"
#define REGISTRO_N_ENCONTRADO 		"Registro(s) nao encontrado!\n"
#define CAMPO_INVALIDO 				"Campo invalido! Informe novamente.\n"
#define ERRO_PK_REPETIDA 			"ERRO: Ja existe um registro com a chave primaria: %s.\n"
#define ARQUIVO_VAZIO 				"Arquivo vazio!\n"
#define INICIO_BUSCA 				"**********************BUSCAR**********************\n"
#define INICIO_LISTAGEM 			"**********************LISTAR**********************\n"
#define INICIO_ALTERACAO 			"**********************ALTERAR*********************\n"
#define INICIO_EXCLUSAO 			"**********************EXCLUIR*********************\n"
#define INICIO_ARQUIVO 				"**********************ARQUIVO*********************\n"
#define INICIO_ARQUIVO_SECUNDARIO 	"*****************ARQUIVO SECUNDARIO****************\n"
#define SUCESSO 					"OPERACAO REALIZADA COM SUCESSO!\n"
#define FALHA 						"FALHA AO REALIZAR OPERACAO!\n"

/* Registro da Carona */
typedef struct
{
	char pk[TAM_PRIMARY_KEY];
	char nome[TAM_NOME];
	char genero[TAM_GENERO];
	char nascimento[TAM_NASCIMENTO]; 	/* DD/MM/AAAA */
	char celular[TAM_CELULAR]; 			/*(99) 99999-9999 */
	char veiculo[TAM_VEICULO];
	char placa[TAM_PLACA];				/* AAA-9999 */
	char trajeto[TAM_TRAJETO];
	char data[TAM_DATA];   				/* DD/MM/AA, ex: 24/09/19 */
	char hora[TAM_HORA];   				/* HH:MM, ex: 07:30 */
	char valor[TAM_VALOR]; 				/* 999.99, ex: 004.95 */
	char vagas[TAM_VAGAS];
} Carona;

/*----- Registros dos Índices -----*/

/* Struct para índice Primário */
typedef struct primary_index
{
	char pk[TAM_PRIMARY_KEY];
	int rrn;
} Ip;

/* Struct para índice secundário de nome */
typedef struct secundary_index
{
	char pk[TAM_PRIMARY_KEY];
	char nome[TAM_NOME];
} Is;

/* Struct para índice secundário de data */
typedef struct secundary_index_of_date
{
	char pk[TAM_PRIMARY_KEY];
	char data[TAM_DATA];
} Isd;

/* Struct para índice secundário de hora */
typedef struct secundary_index_of_time
{
	char pk[TAM_PRIMARY_KEY];
	char hora[TAM_HORA];
} Ist;

/* Lista ligada usada no índice com lista invertida */
typedef struct linked_list
{
	char pk[TAM_PRIMARY_KEY];
	struct linked_list *prox;
} ll;

/* Struct para lista invertida de trajetos*/
typedef struct reverse_index
{
	char trajeto[TAM_TRAJETO];
	ll *lista;
} Ir;

/* GLOBAL: ARQUIVO DE DADOS */
char ARQUIVO[TAM_ARQUIVO];

/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

/* Recebe do usuário uma string simulando o arquivo completo e retorna o número
 * de registros. */
int carregar_arquivo();

/* Exibe a Carona */
int exibir_registro(int rrn);

/* Recupera do arquivo o registro com o rrn informado
 *  e retorna os dados na struct Carona */
Carona recuperar_registro(int rrn);

/* (Re)faz o índice respectivo */
void criar_iprimary(Ip *indice_primario, int *nregistros);

void criar_idriver(Is *indice_nome, int *nregistros);

void criar_idate(Isd *indice_data, int *nregistros);

void criar_itime(Ist *indice_hora, int *nregistros);

void criar_iroute(Ir *indice_rota, int *nregistros, int *ntraj);

/* Constroi o buffer a ser inserido no arquivo de dados a partir da struct carona */
void construir_buffer(char *buffer, Carona *novo);

/* Gera a chave para determinada struct de carona */ 
void gerarChave(Carona *novo);

/* Rotina para impressao de indice secundario */
void imprimirSecundario(Is *idriver, Ir *iroute, Isd *idate, Ist *itime, int nregistros, int ntraj);


/* <<< COLOQUE AQUI OS DEMAIS PROTÓTIPOS DE FUNÇÕES >>> */

/* Lê os dados da entrada e os insere na struct Carona 'novo' */
void criar_carona(Carona *novo);

/* Insere o registro novo no arquivo de dados e em todos os índices - FUNÇÃO INCOMPLETA */
void cadastrar(Ip *iprimary, Is *idriver, Isd *idate, Ist *itime, Ir *iroute, int *nregistros, int *ntraj);


/* INSERÇÕES */

/* Insere o novo registro no índice primario */
int inserir_iprimary(Ip *iprimary, Carona *novo, int *nregistros);

/* Insere o novo registro no índice secundário de nomes */
void inserir_idriver(Is *idriver, Carona *novo, int *nregistros);

/* Insere o novo registro no índice secundário de datas */
void inserir_idate(Isd *idate, Carona *novo, int *nregistros);

/* Insere o novo registro no índice secundário de hora de partida */
void inserir_itime(Ist *itime, Carona *novo, int *nregistros);

/* Insere o novo registro na lista invertida */
void inserir_iroute(Ir *iroute, Carona *novo, int *ntraj);

/* Função que insere uma localidade nova na lista invertida iroute */
int inserir_localidade_iroute(Ir *iroute, char *trajeto, char *pk, int *ntraj);


/* ALTERAÇÃO */

/* Altera o campo numero de vagas do registro de dados */
int alterar(Ip *iprimary, int *nregistros);


/* REMOÇÃO */

/* Remove o registro do arquivo a partir de uma chave primária */
int remover(Ip *iprimary, int *nregistros);

/* Marca o rrn do registro com a chave primaria pk_removida com -1 */
void remover_iprimary(Ip *iprimary, char *pk_removida, int *nregistros);


/* BUSCAS */

/* Busca por um registro a partir de um índice e o exibe na tela */
void buscar(Ip *iprimary, Isd *idate, Ir *iroute, int *nregistros, int *ntraj);

/* Busca pela chave pk_procurada no iprimary */
int buscar_iprimary(Ip *iprimary, char *pk_procurada, int *nregistros);

/* Busca pela data_procurada e exibe na tela os registros encontrados */
void buscar_idate(Ip *iprimary, Isd *idate, char *data_procurada, int *nregistros);

/* Exibe todos os registros da mesma data a partir de atual, de acordo com o índice secunário */
void print_busca_idate(Ip *iprimary, Isd *idate, Isd *atual, int *nregistros);

/* Busca pela rota procurada e exibe na tela os registros encontrados */
void buscar_iroute(Ip *iprimary, Ir *iroute, char *route, int *nregistros, int *ntraj);

/* Busca por uma struct Isd que possua a pk_procurada */
Isd *buscar_pk_idate(Isd *idate, char *pk_procurada, int *nregistros);

/* Busca por uma struct Ist que possua a pk_procurada */
Ist *buscar_pk_itime(Ist *itime, char *pk_procurada, int *nregistros);

/* Busca por um registro que contenha a data e a rota procurada */
void buscar_iroute_idate(Ip *iprimary, Ir *iroute, Isd *idate, char* rota_procurada, char *data_procurada, int *nregistros, int *ntraj);

/* Busca pela rota_procurada em iroute e coloca o resultado em vet_ir */
int buscar_iroute_vet(Ir *iroute, ll *vet_ir, char *rota_procurada, int *ntraj, int *tam_vet_ir);

/* Busca pela data_procurada em idate e coloca o resultado em vet_isd */
int buscar_idate_vet(Isd *idate, Isd *vet_isd, char *data_procurada, int *nregistros, int *tam_vet_isd);

/* Passa os registros de idate da busca 'atual' para o vetor vet_isd */
void passar_busca_vet_isd(Isd *idate, Isd *vet_isd, Isd* atual, int *nregistros, int *tam_vet_isd);


/* LISTAGEM */

/* Lista os registros que estão nos respectivos índices */
void listar(Ip *iprimary, Ir *iroute, Is *idriver, Isd *idate, Ist *itime, int *nregistros, int *ntraj);

/* Lista os registros contidos no índice primário */
void listar_iprimary(Ip *iprimary, int *nregistros);

/* Lista os registros contidos no indice de motoristas */
void listar_idriver(Ip *iprimary, Is *idriver, int *nregistros);

/* Lista os registros contidos no pindice de rotas */
void listar_iroute(Ip *iprimary, Ir *iroute, int *nregistros, int *ntraj);

/* Lista os registros de idate e iroute */
void listar_idate_itime(Ip *iprimary, Isd *idate, Ist* itime, int *nregistros);

/* Lista os registros de iroute, idate e itime */
void listar_iroute_idate_itime(Ip *iprimary, Ir *iroute, Isd *idate, Ist* itime, int *nregistros, int *ntraj);


/* MEMÓRIA */

/* Retira os registros marcados como removidos e re-cria os índices */
void liberarEspaco(Ip *iprimary, Is *idriver, Isd *idate, Ist *itime, Ir *iroute, int *nregistros, int *ntraj);

/* Retira os registros que estão com o caractere de removido e atualiza a variável nregistros */
void limpar_arquivo(int *nregistros);

/* Libera a memória utilizada pelos indices primários e secundários */
void liberarMemoria(Ip *iprimary, Is *idriver, Isd *idate, Ist *itime, Ir *iroute, int *ntraj);


/* FUNÇÕES DE APOIO */

/* Insere o 'campo' na string 'arq' e posisiona o caractere @ ao final */
void inserir_campo_arq(char *arq, char *campo, int *reg_restante);

/* Ordena uma lista ligada da lista invertida */
void ordenar_pk_iroute(Ir *lista_invertida);

/* Ordena vet_idate utilizando o vet_itime como critério de desempate */
void ordenar_vet_idate_com_itime(Isd *vet_idate, Ist *vet_itime, int *total);

/* Compara duas chaves primárias da struct Ip - Função usada no bsearch */
int func_comp_iprimary_bsearch(const void *pk1, const void *pk2);

/* Compara duas chaves primárias da struct Ip - Função usada no qsort */
int func_comp_iprimary_qsort(const void *pk1, const void *pk2);

/* Compara dois nomes da struct Is */
int func_comp_idriver(const void *nome1, const void *nome2);

/* Compara duas datas da struct Isd - Função usada no bsearch */
int func_comp_idate_bsearch(const void *data1, const void *data2);

/* Compara duas datas da struct Isd - Função usada no qsort */
int func_comp_idate_qsort(const void *data1, const void *data2);

/* Compara dois duas chaves primárias da struct Isd - Função usada no bsearch */
int func_comp_idate_pk_bsearch(const void *pk1, const void *pk2);

/* Compara dois duas chaves primárias da struct Isd - Função usada no qsort */
int func_comp_idate_pk_qsort(const void *pk1, const void *pk2);

/* Compara duas horas de partida da struct Ist - Função usada no qsort */
int func_comp_itime_qsort(const void *data1, const void *data2);

/* Compara dois duas chaves primárias da struct Ist - Função usada no bsearch */
int func_comp_itime_pk_bsearch(const void *pk1, const void *pk2);

/* Compara dois duas chaves primárias da struct Ist - Função usada no qsort */
int func_comp_itime_pk_qsort(const void *pk1, const void *pk2);

/* Compara dois trajetos da struct Ir - Função usada no bsearch */
int func_comp_iroute_bsearch(const void *traj1, const void *traj2);

/* Compara dois trajetos da struct Ir - Função usada no qsort */
int func_comp_iroute_qsort(const void *traj1, const void *traj2);


/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
int main()
{
	/* Verifica se há arquivo de dados */
	int carregarArquivo = 0, nregistros = 0, ntraj = 0;
	scanf("%d%*c", &carregarArquivo); /* 1 (sim) | 0 (nao) */
	if (carregarArquivo)
		nregistros = carregar_arquivo();


	/* === ALOCAÇÃO E CRIAÇÃO DOS ÍNDICES === */
	
	/* Índice primário */
	Ip *iprimary = (Ip *)malloc(MAX_REGISTROS * sizeof(Ip));
	if (!iprimary)
	{
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_iprimary(iprimary, &nregistros);

	Is *idriver = (Is *)malloc(MAX_REGISTROS * sizeof(Is));
	if (!idriver){
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_idriver(idriver, &nregistros);

	Isd *idate = (Isd *)malloc(MAX_REGISTROS * sizeof(Isd));
	if (!idate){
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_idate(idate, &nregistros);

	Ist *itime = (Ist *)malloc(MAX_REGISTROS * sizeof(Ist));
	if (!itime){
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_itime(itime, &nregistros);

	Ir *iroute = (Ir *)malloc(MAX_REGISTROS * sizeof(Ir));
	if (!iroute){
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_iroute(iroute, &nregistros, &ntraj);
	
	/* <<< COMPLETE AQUI A ALOCAÇÃO E CRIAÇÃO DOS INDICES SECUNDÁRIOS >>> */


	/* === ROTINA DE EXECUÇÃO DO PROGRAMA === */
	int opcao = 0;
	while (1)
	{
		scanf("%d%*c", &opcao);
		switch (opcao)
		{
		case 1:
			/* cadastrar */
            cadastrar(iprimary, idriver, idate, itime, iroute, &nregistros, &ntraj);
			
			break;

		case 2:
			/* alterar */ 
			printf(INICIO_ALTERACAO);
			if(alterar(iprimary, &nregistros))
				printf(SUCESSO);
			else
				printf(FALHA);
			
			break;

		case 3:
			/* excluir */
			printf(INICIO_EXCLUSAO);
			if(remover(iprimary, &nregistros))
				printf(SUCESSO);
			else
				printf(FALHA);

			break;

		case 4:
			/* buscar */
			printf(INICIO_BUSCA);
			buscar(iprimary, idate, iroute, &nregistros, &ntraj);

			break;

		case 5:
			/* listar */
			printf(INICIO_LISTAGEM);
			listar(iprimary, iroute, idriver, idate, itime, &nregistros, &ntraj);

			break;

		case 6:
			/*	libera espaço */
			liberarEspaco(iprimary, idriver, idate, itime, iroute, &nregistros, &ntraj);

			break;

		case 7:
			/* imprime arquivo de dados */
			printf(INICIO_ARQUIVO);
			if (nregistros == 0)
				printf(ARQUIVO_VAZIO);
			else
				printf("%s\n", ARQUIVO);

			break;

		case 8:
			/*	imprime os índices secundários */
			imprimirSecundario(idriver, iroute, idate, itime, nregistros, ntraj);

			break;

		case 9:
			/*	liberar memória e finalizar o programa */
			liberarMemoria(iprimary, idriver, idate, itime, iroute, &ntraj);
			
			return (0);
			break;

		default:
			printf(OPCAO_INVALIDA);
			break;
		}
	}
	return 0;
}

/* ==========================================================================
 * =========================== CÓDIGO DAS FUNÇÕES ===========================
 * ========================================================================== */

/* Exibe a Carona */
int exibir_registro(int rrn)
{
	if (rrn < 0)
		return 0;

	Carona j = recuperar_registro(rrn);
	char *traj, trajeto[TAM_TRAJETO];

	printf("%s\n", j.pk);
	printf("%s\n", j.nome);
	printf("%s\n", j.genero);
	printf("%s\n", j.nascimento);
	printf("%s\n", j.celular);
	printf("%s\n", j.veiculo);
	printf("%s\n", j.placa);
	printf("%s\n", j.data);
	printf("%s\n", j.hora);
	printf("%s\n", j.valor);
	printf("%s\n", j.vagas);

	strcpy(trajeto, j.trajeto);

	traj = strtok(trajeto, "|");

	while (traj != NULL)
	{
		printf("%s", traj);
		traj = strtok(NULL, "|");
		if (traj != NULL)
		{
			printf(", ");
		}
	}

	printf("\n");

	return 1;
}

/* carrega dados do arquivo e retorna
 * a quantidade de registros lidos */
int carregar_arquivo()
{
	scanf("%[^\n]%*c", ARQUIVO);
	return strlen(ARQUIVO) / TAM_REGISTRO;
}


/* Recupera do arquivo o registro com o rrn
 * informado e retorna os dados na struct Carona */
Carona recuperar_registro(int rrn)
{
	char temp[257], *p;
	strncpy(temp, ARQUIVO + ((rrn)*TAM_REGISTRO), TAM_REGISTRO);
	temp[256] = '\0';
	Carona j;

	p = strtok(temp, "@");
	strcpy(j.nome, p);
	p = strtok(NULL, "@");
	strcpy(j.genero, p);
	p = strtok(NULL, "@");
	strcpy(j.nascimento, p);
	p = strtok(NULL, "@");
	strcpy(j.celular, p);
	p = strtok(NULL, "@");
	strcpy(j.veiculo, p);
	p = strtok(NULL, "@");
	strcpy(j.placa, p);
	p = strtok(NULL, "@");
	strcpy(j.trajeto, p);
	p = strtok(NULL, "@");
	strcpy(j.data, p);
	p = strtok(NULL, "@");
	strcpy(j.hora, p);
	p = strtok(NULL, "@");
	strcpy(j.valor, p);
	p = strtok(NULL, "@");
	strcpy(j.vagas, p);

	gerarChave(&j);

	return j;
}

/* Imprimir indices secundarios */
void imprimirSecundario(Is *idriver, Ir *iroute, Isd *idate, Ist *itime, int nregistros, int ntraj)
{
	int opPrint = 0;
	ll *aux;
	printf(INICIO_ARQUIVO_SECUNDARIO);
	scanf("%d", &opPrint);
	if (!nregistros)
		printf(ARQUIVO_VAZIO);
	switch (opPrint)
	{
	case 1:
		for (int i = 0; i < nregistros; i++)
		{
			printf("%s %s\n", idriver[i].pk, idriver[i].nome);
		}
		break;
	case 2:
		for (int i = 0; i < nregistros; i++)
		{
			printf("%s %s\n", idate[i].pk, idate[i].data);
		}
		break;
	case 3:
		for (int i = 0; i < nregistros; i++)
		{
			printf("%s %s\n", itime[i].pk, itime[i].hora);
		}
		break;
	case 4:
		for (int i = 0; i < ntraj; i++)
		{
			printf("%s", iroute[i].trajeto);
			aux = iroute[i].lista;
			while (aux != NULL)
			{
				printf(" %s", aux->pk);
				aux = aux->prox;
			}
			printf("\n");
		}
		break;
	default:
		printf(OPCAO_INVALIDA);
		break;
	}
}

/* <<< IMPLEMENTE AQUI AS DEMAIS FUNÇÕES >>> */

/* ==========================================================================
 * ============================= CRIAR ÍNDICES ==============================
 * ========================================================================== */

/* (Re)Cria o índice primário */
void criar_iprimary(Ip *indice_primario, int *nregistros){
	
	if(*nregistros == 0)
		return;	
	
	Carona aux;

	for(int i = 0; i < *nregistros; i++){
		aux = recuperar_registro(i);

		strcpy(indice_primario[i].pk, aux.pk);
		indice_primario[i].rrn = i;
	}

	qsort(indice_primario, *nregistros, sizeof(Ip), func_comp_iprimary_qsort);

	return;
}

/* (Re)Cria o índice secundário de nomes */
void criar_idriver(Is *indice_nome, int *nregistros){
	
	if(*nregistros == 0)
		return;	
	
	Carona aux;

	for(int i = 0; i < *nregistros; i++){
		aux = recuperar_registro(i);
		
		strcpy(indice_nome[i].pk, aux.pk);
		strcpy(indice_nome[i].nome, aux.nome);
	}

	qsort(indice_nome, *nregistros, sizeof(Is), func_comp_idriver);

	return;
}

/* (Re)Cria o índice secundário de datas */
void criar_idate(Isd *indice_data, int *nregistros){
	
	if(*nregistros == 0)
		return;	
	
	Carona aux;

	for(int i = 0; i < *nregistros; i++){
		aux = recuperar_registro(i);
		
		strcpy(indice_data[i].pk, aux.pk);
		strcpy(indice_data[i].data, aux.data);
	}

	qsort(indice_data, *nregistros, sizeof(Isd), func_comp_idate_qsort);

	return;
}

/* (Re)Cria o índice secundário de tempo */
void criar_itime(Ist *indice_hora, int *nregistros){
	
	if(*nregistros == 0)
		return;
	
	Carona aux;

	for(int i = 0; i < *nregistros; i++){
		aux = recuperar_registro(i);
		
		strcpy(indice_hora[i].pk, aux.pk);
		strcpy(indice_hora[i].hora, aux.hora);
	}

	qsort(indice_hora, *nregistros, sizeof(Ist), func_comp_itime_qsort);

	return;
}

/* (Re)Cria a lista invertida */
void criar_iroute(Ir *indice_rota, int *nregistros, int *ntraj){
	
	if(*nregistros == 0)
		return;

	Carona aux;

	ll *atual;
	ll *proximo;

	// Liberando a memória alocada anteriormente de todos os trajetos
	for(int i = 0; i < *ntraj; i++){
		atual = indice_rota[i].lista;
		proximo = atual->prox;

		while(1){
			if(proximo == NULL){
				free(atual);
				break;
			}else{
				free(atual);
				atual = proximo;
				proximo = proximo->prox;
			}
		}
	}

	// Começando a re-fazer o índice
	*ntraj = 0;

	for(int i = 0; i < *nregistros; i++){
		aux = recuperar_registro(i);

		inserir_iroute(indice_rota, &aux, ntraj);
	}

	return;
}

/* ==========================================================================
 * ============================= FUNÇÕES BASE ===============================
 * ========================================================================== */

/* Cadastra uma carona e a insere em todos os arquivos de índice e no arquivo de dados */
/* Se o registro já foi inserido a função exibe um erro e não o insere no arquivo de dados e nos índices */
void cadastrar(Ip *iprimary, Is *idriver, Isd *idate, Ist *itime, Ir *iroute, int *nregistros, int *ntraj){
	if(*nregistros == MAX_REGISTROS){
		printf("O arquivo de dados está cheio\n");
		return;
	}
	
	Carona novo;

    
    criar_carona(&novo);

    //inserção nos índices
    if(inserir_iprimary(iprimary, &novo, nregistros)){

		inserir_idriver(idriver, &novo, nregistros);
		inserir_idate(idate, &novo, nregistros);
		inserir_itime(itime, &novo, nregistros);
		inserir_iroute(iroute, &novo, ntraj);

		char buffer[TAM_REGISTRO + 1];
		char *pos_arq = ARQUIVO + ((*nregistros) * TAM_REGISTRO);	//Coloca o ponteiro para onde o registro será inserido no arquivo de dados
		construir_buffer(buffer, &novo);
		strncpy(pos_arq, buffer, (TAM_REGISTRO + 1));

		*nregistros += 1;
		return;
	}else{
		printf(ERRO_PK_REPETIDA, novo.pk);
		return;
	}
}

/* Lê da entrada padão e insere na struct Carona 'novo' */
void criar_carona(Carona *novo){
    scanf("%[^\n]%*c", novo->nome);
    scanf("%[^\n]%*c", novo->genero);
    scanf("%[^\n]%*c", novo->nascimento);
    scanf("%[^\n]%*c", novo->celular);
    scanf("%[^\n]%*c", novo->veiculo);
    scanf("%[^\n]%*c", novo->placa);
    scanf("%[^\n]%*c", novo->trajeto);
    scanf("%[^\n]%*c", novo->data);
    scanf("%[^\n]%*c", novo->hora);
    scanf("%[^\n]%*c", novo->valor);
    scanf("%[^\n]%*c", novo->vagas);

    gerarChave(novo);

    return;
}

/* Gera uma chave primária para a struct Carona novo */
void gerarChave(Carona *novo){
    // Nome
    novo->pk[0] = novo->nome[0];
    // Placa
    novo->pk[1] = novo->placa[0];
    novo->pk[2] = novo->placa[1];
    novo->pk[3] = novo->placa[2];
    // Data
    novo->pk[4] = novo->data[0];
    novo->pk[5] = novo->data[1];
    novo->pk[6] = novo->data[3];
    novo->pk[7] = novo->data[4];
    // Hora
    novo->pk[8] = novo->hora[0];
    novo->pk[9] = novo->hora[1];
    // \0
    novo->pk[10] = '\0';

    return;
}

/* Passa os arquivos da struct Carona para a string buffer com todos os marcadores */
/* A função espera que o buffer fornecido tenhe o tamanho de de TAM_REGISTROS + 1 */
/* A variável reg_preenchido serve como referência para a função inserir_campo_arq */
/* para saber quantos campos existem no registro atualmente, assim permitindo escrever */
/* na string arq, onde a função anterior havia parado. */
void construir_buffer(char *buffer, Carona *novo){
	int reg_preenchido = 0;

	inserir_campo_arq(buffer, novo->nome, &reg_preenchido);
	inserir_campo_arq(buffer, novo->genero, &reg_preenchido);
	inserir_campo_arq(buffer, novo->nascimento, &reg_preenchido);
	inserir_campo_arq(buffer, novo->celular, &reg_preenchido);
	inserir_campo_arq(buffer, novo->veiculo, &reg_preenchido);
	inserir_campo_arq(buffer, novo->placa, &reg_preenchido);
	inserir_campo_arq(buffer, novo->trajeto, &reg_preenchido);
	inserir_campo_arq(buffer, novo->data, &reg_preenchido);
	inserir_campo_arq(buffer, novo->hora, &reg_preenchido);
	inserir_campo_arq(buffer, novo->valor, &reg_preenchido);
	inserir_campo_arq(buffer, novo->vagas, &reg_preenchido);

	for(int i = reg_preenchido; i < TAM_REGISTRO; i++){
		buffer[i] = '#';
	}

	buffer[TAM_REGISTRO] = '\0';

	return;
}

/* ==========================================================================
 * =============================== INSERÇÕES ================================
 * ========================================================================== */

/* Insere 'novo' no índice primário */
/* Retorna 1 se 'novo' foi inserido */
/* Retorna 0 se 'novo' já foi inserido */
int inserir_iprimary(Ip *iprimary, Carona *novo, int *nregistros){

	if(buscar_iprimary(iprimary, novo->pk, nregistros) == -1){
		int total = *nregistros + 1;			//Como nregistros só é incrementado depois de inserir em todos os índices, a variável total é criada
												//A variável total é usada somente na função qsort
		strcpy(iprimary[*nregistros].pk, novo->pk);
		iprimary[*nregistros].rrn = *nregistros;

		qsort(iprimary, total, sizeof(Ip), func_comp_iprimary_qsort);
		
		return 1;
	}else
		return 0;
}

/* Insere 'novo' no índice secundário 'idriver' */
/* Como sabemos que não existe chave primária repetida, não ha necessidade de exibir falhas */
void inserir_idriver(Is *idriver, Carona *novo, int *nregistros){
	int total = *nregistros + 1;

	strcpy(idriver[*nregistros].nome, novo->nome);
	strcpy(idriver[*nregistros].pk, novo->pk);

	qsort(idriver, total, sizeof(Is), func_comp_idriver);

	return;
}

/* Insere o novo registro no índice secundário de datas */
void inserir_idate(Isd *idate, Carona *novo, int *nregistros){
	int total = *nregistros + 1;

	strcpy(idate[*nregistros].data, novo->data);
	strcpy(idate[*nregistros].pk, novo->pk);

	qsort(idate, total, sizeof(Isd), func_comp_idate_qsort);

	return;
}

/* Insere o novo registro no índice secundário de hora de partida */
void inserir_itime(Ist *itime, Carona *novo, int *nregistros){
	int total = *nregistros + 1;

	strcpy(itime[*nregistros].hora, novo->hora);
	strcpy(itime[*nregistros].pk, novo->pk);

	qsort(itime, total, sizeof(Ist), func_comp_itime_qsort);

	return;
}

/* Insere o novo registro na lista invertida */
void inserir_iroute(Ir *iroute, Carona *novo, int *ntraj){
	char temp[TAM_TRAJETO];
	char *atual;

	strcpy(temp, novo->trajeto);

	atual = strtok(temp, "|");

	while(1){

		if(atual == NULL)
			return;
		else if(inserir_localidade_iroute(iroute, atual, novo->pk, ntraj)){
			*ntraj += 1;
		}

		atual = strtok(NULL, "|");
	}
}

/* Função que insere uma localidade nova na lista invertida iroute */
/* Retorna 1 caso um trajeto que ainda não havia sido inserido seja inserido */
/* Retorna 0 caso um trajeto que já havia sido inserido seja inserido novamente */
int inserir_localidade_iroute(Ir *iroute, char *trajeto, char *pk, int *ntraj){
	Ir *busca;

	// Preparando o nó que será inserido (ele sempre será inserido no final da lista e depois a lista será ordenada)
	ll *novo = (ll *)malloc(sizeof(ll));
	if (!novo){
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	strcpy(novo->pk, pk);

	busca = bsearch(trajeto, iroute, *ntraj, sizeof(Ir), func_comp_iroute_bsearch);

	if(busca != NULL){	// Foi encontrado o trajeto que já foi inserido
		ll *aux = busca->lista;

		busca->lista = novo;
		novo->prox = aux;

		ordenar_pk_iroute(busca);

		return 0;
	}else{				// Não foi encontrado no índice nenhum trajeto com o nome 'trajeto'
		int total = *ntraj + 1;

		strcpy(iroute[*ntraj].trajeto, trajeto);
		iroute[*ntraj].lista = novo;
		novo->prox = NULL;

		qsort(iroute, total, sizeof(Ir), func_comp_iroute_qsort);
		
		return 1;
	}
}

/* ==========================================================================
 * =============================== ALTERAÇÃO ================================
 * ========================================================================== */

/* Altera o campo numero de vagas do registro de dados */
/* Retorna 1 caso o registro seja salvo com sucesso */
/* Retorna 0 caso o registro que seria alterado não existe */
int alterar(Ip *iprimary, int *nregistros){
	char pk[TAM_PRIMARY_KEY];
	char nvagas[2];
	
	int n_vag = 0;
	int rrn = 0;

	scanf("%[^\n]%*c", pk);

	rrn = buscar_iprimary(iprimary, pk, nregistros);

	if(rrn == -1){
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}else{

	scanf("%d%*c", &n_vag);
	
		while(1){							// Checando se o campo isnerido respeita as regras
			if(n_vag > 9 || n_vag < 0){
				printf(CAMPO_INVALIDO);
				scanf("%d%*c", &n_vag);
			}else{
				sprintf(nvagas, "%d", n_vag);
				break;
			}
		}

		char buffer[TAM_REGISTRO + 1];		// Criando um buffer para inserir no arquivo
		char *p_arq;

		p_arq = ARQUIVO + (rrn * TAM_REGISTRO);

		strncpy(buffer, p_arq, TAM_REGISTRO);	// Recuperando o registro

		buffer[TAM_REGISTRO] = '\0';

		for(int i = TAM_REGISTRO; i > 0; i--){	// Encontrando o lugar no buffer em que o registro deve ser inserido
			if(buffer[i] == '@'){
				buffer[i - 1] = nvagas[0];
				break;
			}
		}

		strncpy(p_arq, buffer, TAM_REGISTRO);	// Colocando o buffer no arquivo de dados

		return 1;
	}
}

/* ==========================================================================
 * ================================ REMOÇÃO =================================
 * ========================================================================== */

/* Remove o registro do arquivo a partir de uma chave primária */
/* Retorna 1 caso o registro seja removido com sucesso */
/* Retorna 0 caso o registro não seja removido */
int remover(Ip *iprimary, int *nregistros){
	int rrn = 0;
	
	char pk[TAM_PRIMARY_KEY];
	char *p_arq;

	scanf("%[^\n]%*c", pk);
	
	rrn = buscar_iprimary(iprimary, pk, nregistros);

	if(rrn != -1){
		p_arq = ARQUIVO + (rrn * TAM_REGISTRO);

		strncpy(p_arq, "*|", 2);
		
		remover_iprimary(iprimary, pk, nregistros);

		return 1;
	}else{
		printf(REGISTRO_N_ENCONTRADO);
		return 0;
	}
}

/* Marca o rrn do registro com a chave primaria pk_removida com -1 */
void remover_iprimary(Ip *iprimary, char *pk_removida, int *nregistros){
	Ip *busca;

	busca = (Ip *) bsearch(pk_removida, iprimary, *nregistros, sizeof(Ip), func_comp_iprimary_bsearch);

	busca->rrn = -1;

	return;
}

/* ==========================================================================
 * ================================ BUSCAS ==================================
 * ========================================================================== */

/* Busca e exibe um registro a partir de um código de registro, data, rota ou rota e data */
void buscar(Ip *iprimary, Isd *idate, Ir *iroute, int *nregistros, int *ntraj){
	int opcao = 0;
	int rrn = 0;

	char pk[TAM_PRIMARY_KEY];
	char data[TAM_DATA];
	char route[TAM_TRAJETO];

	Carona encontrado;

	scanf("%d%*c", &opcao);

	switch (opcao){
	case 1:
		scanf("%[^\n]%*c", pk);
		rrn = buscar_iprimary(iprimary, pk, nregistros);
		if(rrn != -1)
			exibir_registro(rrn);
		else
			printf(REGISTRO_N_ENCONTRADO);
		break;

	case 2:
		scanf("%[^\n]%*c", data);
		buscar_idate(iprimary, idate, data, nregistros);
		break;

	case 3:
		scanf("%[^\n]%*c", route);
		buscar_iroute(iprimary, iroute, route, nregistros, ntraj);
		break;
		
	case 4:
		scanf("%[^\n]%*c", route);
		scanf("%[^\n]%*c", data);
		buscar_iroute_idate(iprimary, iroute, idate, route, data, nregistros, ntraj);
		break;
	}

	return;
}

/* Busca pela chave pk_procurada no iprimary */
/* Retorna o RRN do registro encontrado */
/* Retorna -1 caso o registro não existe */
/* Devido a forma de como a inserção e a remoção foram realizadas, foram criados alguns passos a mais na busca de uma chave primaria */
int buscar_iprimary(Ip *iprimary, char *pk_procurada, int *nregistros){
	Ip *busca;
	int total = *nregistros - 1;

	busca = (Ip *) bsearch(pk_procurada, iprimary, *nregistros, sizeof(Ip), func_comp_iprimary_bsearch);

	if(busca != NULL){
		if(busca == &(iprimary[total])){					//Caso a posição retornada seja a ultima, o rrn da ultima posição é retornado
			return busca->rrn;
		}else{
			Ip *prox = busca + 1;								//Um ponteiro auxiliar é criado para comparar a proxima chave com a chave da busca

			while(1){
				if(strcmp(busca->pk, prox->pk) != 0){			//Caso a busca e o proximo possuam chaves diferentes, o rrn da busca é retornado
					return busca->rrn;
				}else{
					if(prox->rrn != -1){						//Caso o rrn do proximo seja diferente de -1 (ou seja ele existe), ele é retornado
						return prox->rrn;
					}else if(prox == &(iprimary[total])){		//Caso o proximo seja a ultima posição do vetor, o rrn para onde 
						return prox->rrn;
					}
				}
				prox++;
				busca++;
			}
		}
	}else
		return -1;
}

/* Busca pela data_procurada e exibe na tela os registros encontrados */
void buscar_idate(Ip *iprimary, Isd *idate, char *data_procurada, int *nregistros){
	Isd *busca;

	int rrn = 0;

	busca = (Isd *) bsearch(data_procurada, idate, *nregistros, sizeof(Isd), func_comp_idate_bsearch);

	if(busca != NULL){
		if(busca == idate){					//Caso a posição retornada seja a primeira, está no começo da lista
			print_busca_idate(iprimary, idate, busca, nregistros);
			return;
		}else{
			Isd *anterior = busca - 1;

			while(1){
				if(strcmp(busca->data, anterior->data) != 0){		
					print_busca_idate(iprimary, idate, busca, nregistros);
					return;
				
				}else if(anterior == idate){
					print_busca_idate(iprimary, idate, anterior, nregistros);
					return;
				
				}else{
					
					anterior--;
					busca--;	
				}
			}
		}
	}else{
		printf(REGISTRO_N_ENCONTRADO);
		return;
	}
}

/* Exibe todos os registros da mesma data a partir de atual, de acordo com o índice secunário */
void print_busca_idate(Ip *iprimary, Isd *idate, Isd *atual, int *nregistros){
	int rrn = 0;
	int total = *nregistros - 1;
	int primeiro_print = 1;

	Isd *proximo = atual + 1;

	while(1){

		if(proximo == &(idate[total])){		// Caso o proximo seja o ultimo elemento da lista
			
			if(strcmp(atual->data, proximo->data) == 0){ 	// Caso as Datas sejam iguais

				if(strcmp(atual->pk, proximo->pk) != 0){ 	// Caso as chaves sejam diferentes, exibir atual, proximo e sair do loop
					rrn = buscar_iprimary(iprimary, atual->pk, nregistros);
						
					if(!primeiro_print && rrn != -1){
						printf("\n");
						exibir_registro(rrn);
					}else if(primeiro_print && rrn != -1){
						exibir_registro(rrn);
						primeiro_print--;
					}

					rrn = buscar_iprimary(iprimary, proximo->pk, nregistros);
						
					if(!primeiro_print && rrn != -1){
						printf("\n");
						exibir_registro(rrn);
					}else if(primeiro_print && rrn != -1){
						exibir_registro(rrn);
						primeiro_print--;
					}

					break;
				}else{									// Caso as chaves sejam iguais, exibir somente proximo e sair do loop
					rrn = buscar_iprimary(iprimary, proximo->pk, nregistros);
						
					if(!primeiro_print && rrn != -1){
						printf("\n");
						exibir_registro(rrn);
					}else if(primeiro_print && rrn != -1){
						exibir_registro(rrn);
						primeiro_print--;
					}

					break;
				}
			}else{										// Caso as datas sejam diferentes, exibir atual e sair do loop
				rrn = buscar_iprimary(iprimary, atual->pk, nregistros);
						
				if(!primeiro_print && rrn != -1){
					printf("\n");
					exibir_registro(rrn);
				}else if(primeiro_print && rrn != -1){
					exibir_registro(rrn);
					primeiro_print--;
				}

				break;
			}
		}


		if(strcmp(atual->data, proximo->data) == 0){	// Caso as datas sejam iguais
			
			if(strcmp(atual->pk, proximo->pk) != 0){	// Caso as as chaves sejam diferentes, exibir chave atual
				rrn = buscar_iprimary(iprimary, atual->pk, nregistros);
						
				if(!primeiro_print && rrn != -1){
					printf("\n");
					exibir_registro(rrn);
				}else if(primeiro_print && rrn != -1){
					exibir_registro(rrn);
					primeiro_print--;
				}	
			}
		}else{											// Caso as datas sejam diferentes, exibir a atual e sair
			rrn = buscar_iprimary(iprimary, atual->pk, nregistros);
						
			if(!primeiro_print && rrn != -1){
				printf("\n");
				exibir_registro(rrn);
			}else if(primeiro_print && rrn != -1){
				exibir_registro(rrn);
				primeiro_print--;
			}	
			break;
		}

		atual++;
		proximo++;
	}

	if(primeiro_print){
			printf(REGISTRO_N_ENCONTRADO);
			return;
		}else
			return;
}

/* Busca pela rota procurada e exibe na tela os registros encontrados */ // REVER OS PRINTS DESSA FUNÇÃO
void buscar_iroute(Ip *iprimary, Ir *iroute, char *route, int *nregistros, int *ntraj){
	Ir *busca;

	int rrn = 0;
	int primeiro_print = 1;

	busca = bsearch(route, iroute, *ntraj, sizeof(Ir), func_comp_iroute_bsearch);

	if(busca != NULL){
		ll *atual = busca->lista;
		ll *proximo = atual->prox;

		while(1){					// O loop sempre exibe o registro apontado pelo ponteiro atual que tenha uma pk diferente do proximo registro

			if(proximo == NULL){	// Caso proximo seja null (a lista ligada possui dois elementos somente)
				rrn = buscar_iprimary(iprimary, atual->pk, nregistros);
				
				if(!primeiro_print && rrn != -1){
					printf("\n");
					exibir_registro(rrn);
				}else if(primeiro_print && rrn != -1){
					exibir_registro(rrn);
					primeiro_print--;
				}

				break;
			}

			if(strcmp(atual->pk, proximo->pk) != 0){	// Caso as pks dos registros sejam diferentes
				rrn = buscar_iprimary(iprimary, atual->pk, nregistros);
				
				if(!primeiro_print && rrn != -1){
					printf("\n");
					exibir_registro(rrn);
				}else if(primeiro_print && rrn != -1){
					exibir_registro(rrn);
					primeiro_print--;
				}
			}

			atual = atual->prox;
			proximo = proximo->prox;
		}

		if(primeiro_print){
			printf(REGISTRO_N_ENCONTRADO);
			return;
		}else
			return;

	}else{
		printf(REGISTRO_N_ENCONTRADO);
		return;
	}
}

/* Busca por uma struct Isd que possua a pk_procurada */
/* Retorna o registro encontrado */
Isd *buscar_pk_idate(Isd *idate, char *pk_procurada, int *nregistros){
	Isd *busca;

	busca = (Isd *) bsearch(pk_procurada, idate, *nregistros, sizeof(Isd), func_comp_idate_pk_bsearch);

	return busca;
}

/* Busca por uma struct Ist que possua a pk_procurada */
/* Retorna o registro encontrado */
Ist *buscar_pk_itime(Ist *itime, char *pk_procurada, int *nregistros){
	Ist *busca;

	busca = (Ist *) bsearch(pk_procurada, itime, *nregistros, sizeof(Ist), func_comp_itime_pk_bsearch);

	return busca;
}

/* Busca por um registro que contenha a data e a rota procurada */
/* Para a execução desta função, foram adicionadas outras duas funçoes buscar_iroute_vet e buscar_idate_vet. */
/* Essas funções preenchem os vetores vet_ir e vet_isd, respectivamente, com o retorno da busca, sem a repetição de chaves. */
/* A implementação dessas funções é similar a implementação das funçoes buscar_iroute e buscar_idate, respectivamente. */
/* A única diferença sendo que: ao invéz de exibir na tela o resultado, as chaves primárias são colocadas dentro do vetor para ser comparado depois. */
void buscar_iroute_idate(Ip *iprimary, Ir *iroute, Isd *idate, char* rota_procurada, char *data_procurada, int *nregistros, int *ntraj){
	ll vet_ir[*nregistros];		//Vetores que armazenam as buscas realizadas (sem repetição de chaves)
	Isd vet_isd[*nregistros];

	int rrn = 0;
	int tam_vet_ir = 0;
	int tam_vet_isd = 0;
	int primeiro_print = 1;

	if(buscar_iroute_vet(iroute, vet_ir, rota_procurada, ntraj, &tam_vet_ir) && buscar_idate_vet(idate, vet_isd, data_procurada, nregistros, &tam_vet_isd)){
		for(int i = 0; i < tam_vet_ir; i++){
			for(int j = 0; j < tam_vet_isd; j++){
				if(strcmp(vet_ir[i].pk, vet_isd[j].pk) == 0){
					rrn = buscar_iprimary(iprimary, vet_ir[i].pk, nregistros);

					if(!primeiro_print && rrn != -1){
						printf("\n");
						exibir_registro(rrn);
					}else if(primeiro_print && rrn != -1){
						exibir_registro(rrn);
						primeiro_print--;
					}
				}
			}
		}

		if(primeiro_print){
			printf(REGISTRO_N_ENCONTRADO);
			return;	
		}
	}else{
		printf(REGISTRO_N_ENCONTRADO);
		return;
	}
}

/* Busca pela rota_procurada em iroute e coloca o resultado em vet_ir */
/* Retorna 1 caso a busca encontrou algum resultado */
/* Retorna 0 caso a busca tenha falhado */
int buscar_iroute_vet(Ir *iroute, ll *vet_ir, char *rota_procurada, int *ntraj, int *tam_vet_ir){
	Ir *busca;

	busca = bsearch(rota_procurada, iroute, *ntraj, sizeof(Ir), func_comp_iroute_bsearch);

	if(busca != NULL){
		ll *atual = busca->lista;
		ll *proximo = atual->prox;

		while(1){					// O loop sempre exibe o registro apontado pelo ponteiro atual que tenha uma pk diferente do proximo registro

			if(proximo == NULL){	// Caso proximo seja null (a lista ligada possui dois elementos somente)
				strcpy(vet_ir[*tam_vet_ir].pk, atual->pk);
				vet_ir[*tam_vet_ir].prox = NULL;
				*tam_vet_ir += 1;

				return 1;
			}

			if(strcmp(atual->pk, proximo->pk) != 0){	// Caso as pks dos registros sejam diferentes
				strcpy(vet_ir[*tam_vet_ir].pk, atual->pk);
				vet_ir[*tam_vet_ir].prox = NULL;
				*tam_vet_ir += 1;
			}

			atual = atual->prox;
			proximo = proximo->prox;
		}
	}else
		return 0;
}

/* Busca pela data_procurada em idate e coloca o resultado em vet_isd */
/* Retorna 1 caso a busca encontrou algum resultado */
/* Retorna 0 caso a busca tenha falhado */
int buscar_idate_vet(Isd *idate, Isd *vet_isd, char *data_procurada, int *nregistros, int *tam_vet_isd){
	Isd *busca;

	busca = (Isd *) bsearch(data_procurada, idate, *nregistros, sizeof(Isd), func_comp_idate_bsearch);

	if(busca != NULL){
		if(busca == idate){					//Caso a posição retornada seja a primeira, está no começo da lista
			passar_busca_vet_isd(idate, vet_isd, busca, nregistros, tam_vet_isd);
			return 1;
		}else{
			Isd *anterior = busca - 1;

			while(1){
				if(strcmp(busca->data, anterior->data) != 0){		
					passar_busca_vet_isd(idate, vet_isd, busca, nregistros, tam_vet_isd);
					return 1;
				
				}else if(anterior == idate){
					passar_busca_vet_isd(idate, vet_isd, anterior, nregistros, tam_vet_isd);
					return 1;
				
				}else{
					anterior--;
					busca--;	
				}
			}
		}
	}else
		return 0;
}

/* Passa os registros de idate da busca 'atual' para o vetor vet_isd */
void passar_busca_vet_isd(Isd *idate, Isd *vet_isd, Isd* atual, int *nregistros, int *tam_vet_isd){
	int total = *nregistros - 1;

	int primeiro_print = 1;

	Isd *proximo = atual + 1;

	while(1){

		if(proximo == &(idate[total])){		// Caso o proximo seja o último elemento da lista
			
			if(strcmp(atual->data, proximo->data) == 0){ 	// Caso as Datas sejam iguais

				if(strcmp(atual->pk, proximo->pk) != 0){ 	// Caso as chaves sejam diferentes, passar atual, proximo e sair do loop
					strcpy(vet_isd[*tam_vet_isd].pk, atual->pk);
					vet_isd[*tam_vet_isd].data[0] = '\0';
					*tam_vet_isd += 1;

					strcpy(vet_isd[*tam_vet_isd].pk, proximo->pk);
					vet_isd[*tam_vet_isd].data[0] = '\0';
					*tam_vet_isd += 1;	
					break;
				}else{									// Caso as chaves sejam iguais, passar somente proximo e sair do loop
					strcpy(vet_isd[*tam_vet_isd].pk, proximo->pk);
					vet_isd[*tam_vet_isd].data[0] = '\0';
					*tam_vet_isd += 1;
					break;
				}
			}else{										// Caso as datas sejam diferentes, passar atual e sair do loop
				strcpy(vet_isd[*tam_vet_isd].pk, atual->pk);
				vet_isd[*tam_vet_isd].data[0] = '\0';
				*tam_vet_isd += 1;
				break;
			}
		}


		if(strcmp(atual->data, proximo->data) == 0){	// Caso as datas sejam iguais

			if(strcmp(atual->pk, proximo->pk) != 0){	// Caso as chaves sejam diferentes, passar atual
				strcpy(vet_isd[*tam_vet_isd].pk, atual->pk);
				vet_isd[*tam_vet_isd].data[0] = '\0';
				*tam_vet_isd += 1;
			}
		}else{										// Caso as datas sejam diferentes, passar atual e sai do loop
			strcpy(vet_isd[*tam_vet_isd].pk, atual->pk);
			vet_isd[*tam_vet_isd].data[0] = '\0';
			*tam_vet_isd += 1;
			break;
		}

		atual++;
		proximo++;
	}
}

/* ==========================================================================
 * =============================== LISTAGEM =================================
 * ========================================================================== */

/* Lista os registros que estão nos respectivos índices */
void listar(Ip *iprimary, Ir *iroute, Is *idriver, Isd *idate, Ist *itime, int *nregistros, int *ntraj){
	int opcao = 0;
	int rrn = 0;

	scanf("%d%*c", &opcao);

	switch (opcao){
	case 1:
		listar_iprimary(iprimary, nregistros);
		break;

	case 2:
		listar_iroute(iprimary, iroute, nregistros, ntraj);
		break;

	case 3:
		listar_idriver(iprimary, idriver, nregistros);
		break;
		
	case 4:
		listar_idate_itime(iprimary, idate, itime, nregistros);
		break;

	case 5:
		listar_iroute_idate_itime(iprimary, iroute, idate, itime, nregistros, ntraj);
		break;
	}

	return;
}

/* Lista os registros contidos no índice primário */
void listar_iprimary(Ip *iprimary, int *nregistros){
	int rrn = 0;
	int primeiro_print = 1;				// Variável utilizada como uma falg para exibir os registros da maneira correta

	if(*nregistros == 0){
		printf(REGISTRO_N_ENCONTRADO);
		return;
	}else{
		for (int i = 0; i < *nregistros; i++){
			rrn = buscar_iprimary(iprimary, iprimary[i].pk, nregistros);
			
			if(!primeiro_print && rrn != -1){
				printf("\n");
				exibir_registro(rrn);
			}else if(primeiro_print && rrn != -1){
				exibir_registro(rrn);
				primeiro_print--;
			}
		}

		if(primeiro_print){
			printf(REGISTRO_N_ENCONTRADO);
			return;
		}else
			return;
	}
}

/* Lista os registros contidos no indice de motoristas */
void listar_idriver(Ip *iprimary, Is *idriver, int *nregistros){
	int rrn = 0;
	int primeiro_print = 1;

	if(*nregistros == 0){
		printf(REGISTRO_N_ENCONTRADO);
		return;
	}else if(*nregistros == 1){				// Caso o número de registros seja 1, basta exibir o registro atual
		rrn = buscar_iprimary(iprimary, idriver[0].pk, nregistros);

		if(rrn != -1){
			exibir_registro(rrn);
			return;
		}else{
			printf(REGISTRO_N_ENCONTRADO);
			return;
		}
	}else{										// Caso o número de registros seja maior que 1, o programa entrará em loop exibindo o primeiro registro
		for(int i = 1; i < *nregistros; i++){
			if(strcmp(idriver[i - 1].pk, idriver[i].pk) != 0){
				rrn = buscar_iprimary(iprimary, idriver[i - 1].pk, nregistros);

				if(!primeiro_print && rrn != -1){
					printf("\n");
					exibir_registro(rrn);
				}else if(primeiro_print && rrn != -1){
					exibir_registro(rrn);
					primeiro_print--;
				}
			}
		}

		rrn = buscar_iprimary(iprimary, idriver[*nregistros - 1].pk, nregistros);	// Após sair do loop o programa exibe o ultimo registro que não foi exibido

		if(!primeiro_print && rrn != -1){
			printf("\n");
			exibir_registro(rrn);
		}else if(primeiro_print && rrn != -1){
			exibir_registro(rrn);
			primeiro_print--;
		}

		if(primeiro_print){
			printf(REGISTRO_N_ENCONTRADO);
			return;
		}else
			return;
	}
}

/* Lista os registros contidos no índice de rotas */
void listar_iroute(Ip *iprimary, Ir *iroute, int *nregistros, int *ntraj){
	ll *atual;
	ll *proximo;

	int primeiro_print = 1;
	int rrn = 0;
	
	if(*nregistros == 0){
		printf(REGISTRO_N_ENCONTRADO);
		return;
	}else{

		for(int i = 0; i < *ntraj; i++){
			atual = iroute[i].lista; 
			proximo = atual->prox;

			while(1){					// O loop sempre exibe o registro apontado pelo ponteiro atual que tenha uma pk diferente do proximo registro

				if(proximo == NULL){	// Caso proximo seja null (a lista ligada possui dois elementos somente)
					rrn = buscar_iprimary(iprimary, atual->pk, nregistros);
					
					if(!primeiro_print && rrn != -1){
						printf("\n");
						exibir_registro(rrn);
					}else if(primeiro_print && rrn != -1){
						exibir_registro(rrn);
						primeiro_print--;
					}

					break;
				}

				if(strcmp(atual->pk, proximo->pk) != 0){	// Caso as pks dos registros sejam diferentes
					rrn = buscar_iprimary(iprimary, atual->pk, nregistros);
					
					if(!primeiro_print && rrn != -1){
						printf("\n");
						exibir_registro(rrn);
					}else if(primeiro_print && rrn != -1){
						exibir_registro(rrn);
						primeiro_print--;
					}
				}

				atual = atual->prox;
				proximo = proximo->prox;
			}
		}

		if(primeiro_print){
			printf(REGISTRO_N_ENCONTRADO);
			return;
		}else
			return;
	}
}

/* Lista os registros de idate e iroute */
/* Caso a data seja igual, quem possuir o menor tempo de partida é exibido primeiro  */
/* Caso o tempo de partida também seja igual, quem possuir a menor chave primária é exibido */
void listar_idate_itime(Ip *iprimary, Isd *idate, Ist* itime, int *nregistros){
	int primeiro_print = 1;
	int rrn = 0;
	int qtd_vet_idate = 0;
	
	Isd vet_idate[*nregistros];

	if(*nregistros == 0){
		printf(REGISTRO_N_ENCONTRADO);
		return;
	}else if(*nregistros == 1){
		rrn = buscar_iprimary(iprimary, idate[0].pk, nregistros);
		
		if(rrn != -1){
			exibir_registro(rrn);
			return;
		}else{
			printf(REGISTRO_N_ENCONTRADO);
			return;
		}
	}else{
		for (int i = 0; i < *nregistros; i++){		// Inserindo os registros de idate em vet_idate sem repetição de chaves

			if(((i + 1) == (*nregistros - 1))){		// Consição de parada (o vetor chegou ao final)
				if(strcmp(idate[i].pk, idate[i + 1].pk) != 0){
					strcpy(vet_idate[qtd_vet_idate].data, idate[i].data);
					strcpy(vet_idate[qtd_vet_idate].pk, idate[i].pk);
					qtd_vet_idate = qtd_vet_idate + 1;

					strcpy(vet_idate[qtd_vet_idate].data, idate[i+1].data);
					strcpy(vet_idate[qtd_vet_idate].pk, idate[i+1].pk);
					qtd_vet_idate++;
				}else{
					strcpy(vet_idate[qtd_vet_idate].data, idate[i].data);
					strcpy(vet_idate[qtd_vet_idate].pk, idate[i].pk);
					qtd_vet_idate++;
				}
				break;
			}

			if(strcmp(idate[i].pk, idate[i + 1].pk) != 0){		// Comparação para ver se existem datas repetidas
				strcpy(vet_idate[qtd_vet_idate].data, idate[i].data);
				strcpy(vet_idate[qtd_vet_idate].pk, idate[i].pk);
				qtd_vet_idate++;
			}
		}

		// Re-ordenando o vetor itime em relação a chave primária
		qsort(itime, *nregistros, sizeof(Ist), func_comp_itime_pk_qsort);

		// Agora tenho vet_idate com todas as datas e sem repetição de chaves
		Ist vet_itime[qtd_vet_idate];
		Ist *busca;

		for(int i = 0; i < qtd_vet_idate; i++){		// Buscando todos os registros em itime que possuem a mesma chave de vet_idate e os ordena
			busca = buscar_pk_itime(itime, vet_idate[i].pk, nregistros);
			strcpy(vet_itime[i].pk, busca->pk);
			strcpy(vet_itime[i].hora, busca->hora);
		}

		// Agora tenho um vetor com a mesma ordem que o vet_idate só que com a informação das horas de partida
		// Agora vou ordenar o vet_idate, quando encontrar datas iguais, ordenar pela hora de partida
			
		ordenar_vet_idate_com_itime(vet_idate, vet_itime, &qtd_vet_idate);
			
		// Agora basta exibir o vet_idate
		for(int i = 0; i < qtd_vet_idate; i++){
			rrn = buscar_iprimary(iprimary, vet_idate[i].pk, nregistros);

			if(!primeiro_print && rrn != -1){
				printf("\n");
				exibir_registro(rrn);
			}else if(primeiro_print && rrn != -1){
				exibir_registro(rrn);
				primeiro_print--;
			}
			
		}

		if(primeiro_print)
			printf(REGISTRO_N_ENCONTRADO);
		
		
		// Re-ordenando o indice itime para a ordenação anterior
		qsort(itime, *nregistros, sizeof(Ist), func_comp_itime_qsort);

		return;
	}
}

/* Lista os registros de iroute, idate e itime */
/* Em caso de empate, a menor data será impresa primeiro */
/* Em caso de empate novamente, a menor hora sera impressa primeiro */
/* Em caso de empate novamente, a menor chave primária será impressa primeiro */
void listar_iroute_idate_itime(Ip *iprimary, Ir *iroute, Isd *idate, Ist* itime, int *nregistros, int *ntraj){
	ll *atual;
	ll *proximo;
	Isd *busca_idate;
	Ist *busca_itime;

	Isd vet_idate[*nregistros];
	
	int qtd_vet_idate = 0;
	int rrn = 0;
	int primeiro_print = 1;

	if(*nregistros == 0){
		printf(REGISTRO_N_ENCONTRADO);
		return;
	}else{
		//Ordenando idate e itime em relação a chave primária para realizar as buscas
		qsort(idate, *nregistros, sizeof(Isd), func_comp_idate_pk_qsort);
		qsort(itime, *nregistros, sizeof(Ist), func_comp_itime_pk_qsort);

		for(int i = 0; i < *ntraj; i++){
			atual = iroute[i].lista;
			proximo = atual->prox;
			qtd_vet_idate = 0;

			while(1){	// Coloca todos os elementos de iroute em vet_idate

				if(proximo == NULL){
					busca_idate = buscar_pk_idate(idate, atual->pk, nregistros);

					strcpy(vet_idate[qtd_vet_idate].data, busca_idate->data);
					strcpy(vet_idate[qtd_vet_idate].pk, busca_idate->pk);
					qtd_vet_idate++;

					break;
				}

				if(strcmp(atual->pk, proximo->pk) != 0){
					busca_idate = buscar_pk_idate(idate, atual->pk, nregistros);

					strcpy(vet_idate[qtd_vet_idate].data, busca_idate->data);
					strcpy(vet_idate[qtd_vet_idate].pk, busca_idate->pk);
					qtd_vet_idate++;
				}


				atual = atual->prox;
				proximo = proximo->prox;
			}

			// Agora tenho o vetor vet_idate não ordenado com todas as datas de um trajeto e sem repetição de chaves
			// Ordenando vet_idate em relação as datas

			qsort(vet_idate, qtd_vet_idate, sizeof(Isd), func_comp_idate_qsort);

			Ist vet_itime[qtd_vet_idate];

			for(int j = 0; j < qtd_vet_idate; j++){		// Buscando todos os registros em itime que possuem a mesma chave de vet_idate e os ordena
				busca_itime = buscar_pk_itime(itime, vet_idate[j].pk, nregistros);
				strcpy(vet_itime[j].pk, busca_itime->pk);
				strcpy(vet_itime[j].hora, busca_itime->hora);
			}
			
			// Agora tenho um vetor com a mesma ordem que o vet_idate só que com a informação das horas de partida
			// Agora vou ordenar o vet_idate, quando encontrar datas iguais, ordenar pela hora de partida
				
			ordenar_vet_idate_com_itime(vet_idate, vet_itime, &qtd_vet_idate);

			// Agora basta exibir o vet_idate
			for(int j = 0; j < qtd_vet_idate; j++){
				rrn = buscar_iprimary(iprimary, vet_idate[j].pk, nregistros);

				if(!primeiro_print && rrn != -1){
					printf("\n");
					exibir_registro(rrn);
				}else if(primeiro_print && rrn != -1){
					exibir_registro(rrn);
					primeiro_print--;
				}
				
			}
		}

		if(primeiro_print)
			printf(REGISTRO_N_ENCONTRADO);
		
		// Re-ordenando os indices itime e idate para a ordenação anterior
		qsort(itime, *nregistros, sizeof(Ist), func_comp_itime_qsort);
		qsort(idate, *nregistros, sizeof(Isd), func_comp_idate_qsort);

		return;
	}
}

/* ==========================================================================
 * =============================== MEMÓRIA ==================================
 * ========================================================================== */

/* Retira os registros marcados como removidos e re-cria os índices */
void liberarEspaco(Ip *iprimary, Is *idriver, Isd *idate, Ist *itime, Ir *iroute, int *nregistros, int *ntraj){
	// Re-organiza o arquivo de dados e atualiza o valor de nregistros
	limpar_arquivo(nregistros);

	// Re-cria todos os índices
	criar_iprimary(iprimary, nregistros);
	criar_idriver(idriver, nregistros);
	criar_idate(idate, nregistros);
	criar_itime(itime, nregistros);
	criar_iroute(iroute, nregistros, ntraj);

	return;
}

/* Retira os registros que estão com o caractere de removido e atualiza a variável nregistros */
void limpar_arquivo(int *nregistros){
	int errado = 0;
	
	char buffer[TAM_REGISTRO];
	char *p_arq;
	char *p_aux;

	// Vou recuperar todo o registro do arquivo	
	for(int i = 0; i < *nregistros; i++){
		p_arq = (i * TAM_REGISTRO) + ARQUIVO;

		strncpy(buffer, p_arq, TAM_REGISTRO);

		if(buffer[0] == '*' && buffer[1] == '|'){	// Checo se o registro recuperado está com o marcador, caso esteja, eu incremento errado
			errado++;
		}else if(errado != 0){
			p_aux = ((i - errado) * TAM_REGISTRO) + ARQUIVO;	// Quando identidico um arquivo certo, eu retiro a quantidade de registros errados do ponteiro e coloco o registro em seu lugar

			strncpy(p_aux, buffer, TAM_REGISTRO);
		}

	}

	// Retirando o número errado de registros do número total de registros
	*nregistros = *nregistros - errado;

	// Colocando o caractere indicador de final de string no arquivo
	p_arq = (*nregistros * TAM_REGISTRO) + ARQUIVO;
	*p_arq = '\0';

	return;
}

/* Libera a memória utilizada pelos indices primários e secundários */
void liberarMemoria(Ip *iprimary, Is *idriver, Isd *idate, Ist *itime, Ir *iroute, int *ntraj){
	ll *atual;
	ll *proximo;

	free(iprimary);
	free(idriver);
	free(idate);
	free(itime);

	for(int i = 0; i < *ntraj; i++){
		atual = iroute[i].lista;
		proximo = atual->prox;

		while(1){
			if(proximo == NULL){
				free(atual);
				break;
			}else{
				free(atual);
				atual = proximo;
				proximo = proximo->prox;
			}
		}
	}

	free(iroute);
	return;
}

/* ==========================================================================
 * =========================== FUNÇÕES DE APOIO =============================
 * ========================================================================== */

/* Insere no campo no registro do arq */
void inserir_campo_arq(char *arq, char *campo, int *reg_preenchido){
	int temp;

	temp = sprintf((arq + *reg_preenchido), "%s", campo);	// Colocando o campo no registro
	*reg_preenchido += temp;								// Adicionando o número total de caracteres adicionados no registro preenchido
	arq[*reg_preenchido] = '@';								// Adicionando o caractere separador
	*reg_preenchido += 1;									// Adicionando o número total de caracteres devido a adição do caractere separador

	return;
}

/* Ordena uma lista ligada da lista invertida */
/* Sempre asume que o elemento inserido esteja na primeira posição */
void ordenar_pk_iroute(Ir *lista_invertida){
	ll *atual = lista_invertida->lista;
	
	char temp[TAM_PRIMARY_KEY];

	int result = 0;

	while(1){
		result = strcmp(atual->pk, atual->prox->pk);
		
		if(result > 0){					//Invertendo os nós (Como os ponteiros nontinuariam os mesmos, eu somente estou invertendo as chaves)
			strcpy(temp, atual->prox->pk);
			strcpy(atual->prox->pk, atual->pk);
			strcpy(atual->pk, temp);
		}else if(result < 0)			// Caso a comparação das chaves seja menor que 0, a ordenação para pois a chave já encontrou o seu local
			return;

		atual = atual->prox;

		if(atual->prox == NULL)			// Caso atual->prox seja null (ou seja, está no final da lista) a ordenção para
			return;
	}
}

/* Ordena vet_idate utilizando o vet_itime como critério de desempate */
/* ATENÇÃO - SE ESTIVER DANDO ERRO DE TIMEOUT NO TRABALHO CHECAR ESSA FUNÇÃO */
void ordenar_vet_idate_com_itime(Isd *vet_idate, Ist *vet_itime, int *total){
	char aux[TAM_PRIMARY_KEY];
	
	for(int i = 0; i < *total; i++){
		for(int j = (i+1); j < *total; j++){
			if(strcmp(vet_idate[i].data, vet_idate[j].data) == 0){		// As datas de partidas sã iguais
				if(strcmp(vet_itime[i].hora, vet_itime[j].hora) > 0){	// A hora de partida atual é maior
					
					// Invertendo as pk's do vet_idate
					strcpy(aux, vet_idate[j].pk);
					strcpy(vet_idate[j].pk, vet_idate[i].pk);
					strcpy(vet_idate[i].pk, aux);
					// Invertendo as pk's do vet_itime
					strcpy(aux, vet_itime[j].pk);
					strcpy(vet_itime[j].pk, vet_itime[i].pk);
					strcpy(vet_itime[i].pk, aux);
					// Invertendo as oras do vet_itime
					strcpy(aux, vet_itime[j].hora);
					strcpy(vet_itime[j].hora, vet_itime[i].hora);
					strcpy(vet_itime[i].hora, aux);

				}else if(strcmp(vet_itime[i].hora, vet_itime[j].hora) == 0){	// As horas de partida são iguais
					if(strcpy(vet_idate[i].pk, vet_idate[j].pk) > 0){			// A chave atual é maior. Caso contrário nada acontece
						
						// Invertendo as pk's do vet_idate
						strcpy(aux, vet_idate[j].pk);
						strcpy(vet_idate[j].pk, vet_idate[i].pk);
						strcpy(vet_idate[i].pk, aux);
						// Invertendo as pk's do vet_itime
						strcpy(aux, vet_itime[j].pk);
						strcpy(vet_itime[j].pk, vet_itime[i].pk);
						strcpy(vet_itime[i].pk, aux);
						// Invertendo as oras do vet_itime
						strcpy(aux, vet_itime[j].hora);
						strcpy(vet_itime[j].hora, vet_itime[i].hora);
						strcpy(vet_itime[i].hora, aux);
					}
				}
							
			}else
				break;
		}
	}

	return;
}

/* Compara duas chaves primárias da struct Ip */
/* Retorna 0 se pk1 é igual a pk2 */
/* Retorna um número positivo se pk1 é maior que pk2 */
/* Retorna um número negativo se pk1 é menor que pk2 */
int func_comp_iprimary_bsearch(const void *pk1, const void *pk2){
	return strcmp((char*)pk1, ((Ip*)pk2)->pk);
}

/* Compara duas chaves primárias da struct Ip */
/* Retorna 0 se pk1 é igual a pk2 */
/* Retorna um número positivo se pk1 é maior que pk2 */
/* Retorna um número negativo se pk1 é menor que pk2 */
int func_comp_iprimary_qsort(const void *pk1, const void *pk2){
	int result = 0;
	
	result = strcmp(((Ip*)pk1)->pk, ((Ip*)pk2)->pk);
	
	if(result == 0)		//pk1 e pk2 são iguais
		return ((Ip*)pk1)->rrn - ((Ip*)pk2)->rrn;		//comparando os rrn dos registros, no caso os rrns negativos ficarão antes dos rrns positivos de chaves iguais
	else if(result > 0)	//pk1 é maior que pk2
		return 1;			
	else				//pk1 é menor que pk2
		return -1;
}

/* Compara dois nomes da struct Is */
/* Retorna 0 se nome1 é igual a nome2 */
/* Retorna 1 se nome1 é maior que nome2 */
/* Retorna -1 se nome1 é menor que nome2 */
/* Caso o nome seja igual, quem é maior é decidido pela chave primária */
int func_comp_idriver(const void *nome1, const void *nome2){
	int result = 0;

	result = strcmp(((Is*)nome1)->nome, ((Is*)nome2)->nome);

	if(result == 0){		//nome1 e nome2 são iguais
		return strcmp(((Is*)nome1)->pk, ((Is*)nome2)->pk);	//como nome1 e nome2 são iguais, eles serão ordenados pela chave
	}else if(result > 0)	//nome1 é maior que nome2
		return 1;
	else					//nome1 é menor que nome2
		return -1;
	
}

/* Compara duas datas da struct Isd */
/* Retorna 0 se d1 é igual a d2 */
/* Retorna um número positivo se d1 é maior que d2 */
/* Retorna um número negativo se d1 é menor que d2 */
int func_comp_idate_bsearch(const void *d1, const void *d2){
	int result = 0;
	char *aux = (char *)d1;
	char data1[7];
	char data2[7]; 

	// Colocando os campos da hora invertidos para comparação
	// Ano
	data1[0] = aux[6];
	data1[1] = aux[7];
	// Mês
	data1[2] = aux[3];
	data1[3] = aux[4];
	// Dia
	data1[4] = aux[0];
	data1[5] = aux[1];
	// Caractere indicando final da string
	data1[6] = '\0';

	// Ano
	data2[0] = ((Isd*)d2)->data[6];
	data2[1] = ((Isd*)d2)->data[7];
	// Mês
	data2[2] = ((Isd*)d2)->data[3];
	data2[3] = ((Isd*)d2)->data[4];
	// Dia
	data2[4] = ((Isd*)d2)->data[0];
	data2[5] = ((Isd*)d2)->data[1];
	// Caractere indicando final da string
	data2[6] = '\0';

	return strcmp(data1, data2);
}

/* Compara duas datas da struct Isd */
/* Retorna 0 se data1 é igual a data2 */
/* Retorna um número positivo se data1 é maior que data2 */
/* Retorna um número negativo se data1 é menor que data2 */
/* Caso a data seja igual, quem é maior é decidido pela chave primária */
int func_comp_idate_qsort(const void *d1, const void *d2){
	int result = 0;
	char data1[7];
	char data2[7];

	// Colocando os campos da hora invertidos para comparação
	// Ano
	data1[0] = ((Isd*)d1)->data[6];
	data1[1] = ((Isd*)d1)->data[7];
	// Mês
	data1[2] = ((Isd*)d1)->data[3];
	data1[3] = ((Isd*)d1)->data[4];
	// Dia
	data1[4] = ((Isd*)d1)->data[0];
	data1[5] = ((Isd*)d1)->data[1];
	// Caractere indicando final da string
	data1[6] = '\0';

	// Ano
	data2[0] = ((Isd*)d2)->data[6];
	data2[1] = ((Isd*)d2)->data[7];
	// Mês
	data2[2] = ((Isd*)d2)->data[3];
	data2[3] = ((Isd*)d2)->data[4];
	// Dia
	data2[4] = ((Isd*)d2)->data[0];
	data2[5] = ((Isd*)d2)->data[1];
	// Caractere indicando final da string
	data2[6] = '\0';

	result = strcmp(data1, data2);
	
	if(result == 0){		//d1 e d2 são iguais
		return strcmp(((Isd*)d1)->pk, ((Isd*)d2)->pk); //comparando as chaves dos registros para decidir qual é maior
	}else if(result > 0)	//d1 é maior que d2
		return 1;			
	else				//d1 é menor que d2
		return -1;
}

/* Compara dois duas chaves primárias da struct Isd */
/* Retorna 0 se pk1 é igual a pk2 */
/* Retorna um número positivo se pk1 é maior que pk2 */
/* Retorna um número negativo se pk1 é menor que pk2 */
int func_comp_idate_pk_bsearch(const void *pk1, const void *pk2){
	return strcmp((char*)pk1, ((Isd*)pk2)->pk);
}

/* Compara dois duas chaves primárias da struct Isd */
/* Retorna 0 se pk1 é igual a pk2 */
/* Retorna um número positivo se pk1 é maior que pk2 */
/* Retorna um número negativo se pk1 é menor que pk2 */
int func_comp_idate_pk_qsort(const void *pk1, const void *pk2){
	return strcmp(((Isd*)pk1)->pk, ((Isd*)pk2)->pk);
}

/* Compara duas horas de partida da struct Ist */
/* Retorna 0 se hora1 é igual a hora2 */
/* Retorna um número positivo se hora1 é maior que hora2 */
/* Retorna um número negativo se hora1 é menor que hora2 */
/* Caso o horario seja igual, quem é maior é decidido pela chave primária */
int func_comp_itime_qsort(const void *hora1, const void *hora2){
	int result = 0;
	
	result = strcmp(((Ist*)hora1)->hora, ((Ist*)hora2)->hora);
	
	if(result == 0){		//hora1 e hora2 são iguais
		return strcmp(((Ist*)hora1)->pk, ((Ist*)hora2)->pk); //comparando as chaves dos registros para decidir qual é maior
	}else if(result > 0)	//hora1 é maior que hora2
		return 1;			
	else				//hora1 é menor que hora2
		return -1;
}

/* Compara dois duas chaves primárias da struct Ist */
/* Retorna 0 se pk1 é igual a pk2 */
/* Retorna um número positivo se pk1 é maior que pk2 */
/* Retorna um número negativo se pk1 é menor que pk2 */
int func_comp_itime_pk_bsearch(const void *pk1, const void *pk2){
	return strcmp((char*)pk1, ((Ist*)pk2)->pk);
}

/* Compara dois duas chaves primárias da struct Ist */
/* Retorna 0 se pk1 é igual a pk2 */
/* Retorna um número positivo se pk1 é maior que pk2 */
/* Retorna um número negativo se pk1 é menor que pk2 */
int func_comp_itime_pk_qsort(const void *pk1, const void *pk2){
	return strcmp(((Ist*)pk1)->pk, ((Ist*)pk2)->pk);
}

/* Compara dois trajetos da struct Ir */
/* Retorna 0 se traj1 é igual a traj2 */
/* Retorna um número positivo se traj1 é maior que traj2 */
/* Retorna um número negativo se traj1 é menor que traj2 */
int func_comp_iroute_bsearch(const void *traj1, const void *traj2){
	return strcmp((char*)traj1, ((Ir*)traj2)->trajeto);
}

/* Compara dois trajetos da struct Ir */
/* Retorna 0 se traj1 é igual a traj2 */
/* Retorna um número positivo se traj1 é maior que traj2 */
/* Retorna um número negativo se traj1 é menor que traj2 */
int func_comp_iroute_qsort(const void *traj1, const void *traj2){
	return strcmp(((Ir*)traj1)->trajeto, ((Ir*)traj2)->trajeto);
}