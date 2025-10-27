#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define TAMANHO_TABELA = 1200

typedef struct {
	char chave[12]; //estritamente para CPF (11 digitos + '/0')
	int ocupado; //1 = ocupado, 0 = livre
} item;

typedef struct {
	item tabela[TAMANHO_TABELA];
} hash_table;


//usa do fnv-1a de 32 bits como algoritmo de hashing
//https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

uint_32t fnv1a_hash(const char* chave)
{
	uint_32 hash = 2166136261u; //offset para 32 bits  
	while(*chave++)
	{
		hash ^= (uint_8t)*chave;
		hash *= 16777619u; //decimal para 32 bits
	}
	return hash;
}

void insere_hashtable(hash_table *hash, const char *chave)
{
	//indice normalizado para o tamanho da tabela
	uint_32t = idx = fnv1a_hash(chave) % TAMANHO_TABELA;

	for(int i = 0; i < TAMANHO_TABELA; i++)
	{
		uint_32t p = (idx + i) % TAMANHO_TABELA;

		if(!hash->tabela[p].ocupado)
		{
			strcpy(hash->tabela[p].chave, chave);
			hash->tabela[p].ocupado = 1;
			return;
		}
	}
}


int main()
{
	FILE* aqr;
	char cpf[12];
	hash_table tabela;

	if(fopen("cpf.txt", "r") == NULL)
	{
		printf("Falha ao abrir arquivo\n");
		return -1;
	}
	
	while(fgets(cpf, 12, arq))
	{
		insere_hashtable(&tabela, cpf);
	}

	return 0;
}
