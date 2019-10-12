/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Organização de Recuperação da Informação
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 01
 *
 * RA:
 * Aluno:
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

/* Realiza os scanfs na struct Carona*/
void ler_entrada(char *registro, Carona *novo);

/* Gera a chave para determinada struct de carona */ 
void gerarChave(Carona *novo);

/* Rotina para impressao de indice secundario */
void imprimirSecundario(Is *idriver, Ir *iroute, Isd *idate, Ist *itime, int nregistros, int ntraj);


/* <<< COLOQUE AQUI OS DEMAIS PROTÓTIPOS DE FUNÇÕES >>> */



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

			/* <<< COLOQUE AQUI A CHAMADA PARA A FUNCAO CADASTRAR >>> */
			
			break;

		case 2:
			/* alterar */ 
			printf(INICIO_ALTERACAO);

		/*	if(alterar([args]))
				printf(SUCESSO);
			else
				printf(FALHA); */

			break;

		case 3:
			/* excluir */
			printf(INICIO_EXCLUSAO);

		/*	if(remover([args]))
				printf(SUCESSO);
			else
				printf(FALHA); */

			break;

		case 4:
			/* buscar */
			printf(INICIO_BUSCA);

		/*	buscar([args]); */

			break;

		case 5:
			/* listar */
			printf(INICIO_LISTAGEM);

		/*	listar([args]); */

			break;

		case 6:
			/*	libera espaço */

			/* <<< COLOQUE AQUI A CHAMADA PARA A FUNCAO LIBERAR ESPACO >>> */

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

			/* <<< LIBERE A MEMÓRIA DAS ESTRUTURAS CRIADAS >>> */
			
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

/* ==========================================================================
 * ============================= CRIAR ÍNDICES ==============================
 * ========================================================================== */

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