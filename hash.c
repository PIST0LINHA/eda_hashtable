//Lucas Manoel Martins de Souza - 2320715
//Luiz Henrique Machado da Silva - 2411819
//Turma: 3WA

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define TAMANHO_TABELA 1201

typedef struct {
	char chave[12]; //estritamente para CPF (11 digitos + '/n')
	int ocupado; //1 = ocupado, 0 = livre
} item;

typedef struct {
	item tabela[TAMANHO_TABELA];
} hash_table;


//usa do fnv-1a de 32 bits como algoritmo de hashing
//https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
uint32_t fnv1a_hash(const char* chave)
{
	uint32_t hash = 2166136261; //offset para 32 bits  
	while(*chave)
	{
		hash ^= (uint8_t)*chave;
		hash *= 16777619; //decimal para 32 bits
		chave++;
	}
	return hash;
}

uint32_t fnv1a_hash_aux(const char *chave) {
    uint32_t hash = 2166136261;
    while (*chave) {
        hash ^= (uint8_t)*chave;
        hash *= 16777619;
        chave++;
    }
	//perto de 1201
    return (hash % 1193) + 1;
}

//one at a time hash
//https://en.wikipedia.org/wiki/Jenkins_hash_function
uint32_t lookup3(const uint8_t* key, size_t length) {
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

uint32_t lookup3_aux(const uint8_t* key, size_t length)
{
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 13;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

void insere_hashtable(hash_table *hash, const char *chave, int *colisao)
{
	//indice normalizado para o tamanho da tabela
	//uint32_t idx = fnv1a_hash(chave) % TAMANHO_TABELA;
	
	int len = strlen(chave);
	uint32_t idx = fnv1a_hash_aux(chave) % TAMANHO_TABELA;
	uint32_t idx2 = (1 + lookup3((unsigned char*)chave, len)) % (TAMANHO_TABELA - 1);

	for(int i = 0; i < TAMANHO_TABELA; i++)
	{
		uint32_t p = (idx + (idx2 * i)) % TAMANHO_TABELA;

		if(hash->tabela[p].ocupado == 0)
		{
			strcpy(hash->tabela[p].chave, chave);
			hash->tabela[p].ocupado = 1;
			return;
		} else if(hash->tabela[p].ocupado == 1){
		  *colisao += 1;
		}
	}
}


int main()
{
	FILE* arq = fopen("cpf.txt", "r");
	char cpf[12];
	hash_table table;

	//inicializa a regiao de memoria da hash table com zero
	memset(&table, 0, sizeof(table));

	int colisao = 0;

	if(arq == NULL)
	{
		printf("Falha ao abrir arquivo\n");
		return -1;
	}
	
	int n_chaves = 100;

	//checa o num de colisões em intervalos de 100 em 100
	for(int i = n_chaves; i < 1001; i += 100)
	{
	  //reseta o arquivo, contador e tabela entre cada iteracao
	  rewind(arq);
	  memset(&table, 0, sizeof(table));
	  colisao = 0;

	  while(fgets(cpf, 12, arq) && n_chaves != 0)
	  {
		//tira o \n no final de cada linha
		cpf[strcspn(cpf, "\n")] = 0;

		if(strlen(cpf) != 11) continue;
		insere_hashtable(&table, cpf, &colisao);
		n_chaves--;
	  }
	  n_chaves = i;
	  printf("colisoes: %d | n_chaves: %d\n", colisao, n_chaves);
	}
  return 0;
}
