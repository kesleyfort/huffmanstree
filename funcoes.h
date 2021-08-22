//
// Created by kalves on 17/08/2021.
//

#ifndef PROJETOED2_FUNCOES_H
#define PROJETOED2_FUNCOES_H
/*
 * Huffman Coding
 *
 * Trabalho feito para a disciplina de estrutura de dados 2.
 * O programa lê um arquivo de texto para determinar quantas vezes
 * os caracteres aparecem e depois executa a compressão
 * Como funciona o algoritmo?
 * O algoritmo de Huffman recebe um fluxo de bits e devolve um fluxo
 * de bits comprimido que representa o fluxo original. Em geral, o
 * fluxo comprimido é mais curto que o original.

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

/* Existem 256 caracteres possíveis */

#define NUM_CARACTERES	256

/* tree node, heap node */

typedef struct arvore arvore;
struct arvore {
    int		freq;	/* Frequência; Define a prioridade para a heap */
    unsigned char	ch;	/* Carácter. Pode ser qualquer tipo presente na table ASCII */
    arvore	*esq,	/* Filho da esquerda da árvore de huffman */
    *dir;	/* Filho da direita da árvore de huffman */
};
/* Fila de prioridade implementada como uma heap binária*/
typedef struct fprioridades {
    int		tam_heap;
    arvore	*A[NUM_CARACTERES];
} FP;
/* Cria uma fila vazia */
void criar_fila_prioridades (FP *p);
/* Retorna o pai do node da heap */
int pai (int i);
/* Retorna o filho da esquerda do nó na heap */
int esquerda (int i);
/* Retorna o filho da direita do nó na heap */
int direita (int i);
/* Transforma a subheap com raiz i numa heap. Assume que esquerda(i) e dir(i) são heaps
 */
void heapify (FP *p, int i);
/* Insere um elemento na fila de prioridades.  r->freq define a prioridade */
void inserir_fp (FP *p, arvore *r);
/* Remove o elemento na raiz da heap (Conforme o algoritmo, deve ser o elemento com menor repetição) */
arvore *extrair_min_fp (FP *p);
/* Lê o arquivo para calcular a frequência dos carácteres e os insere no vetor v
 */
unsigned int buscarFrequencias (FILE *f, unsigned int v[]);
/* Cria a árvore de huffman baseando nas frequencias encontradas no vetor freq (usando o algoritmo de Huffman) */
arvore *constroi_arv_Huffman (const unsigned int frequencias[]);
/* transversa a árvore, construindo a codificação do arquivo no vetor codigos*/
void transversar (arvore *r, 	/* Raiz da sub-árvore*/
               int nivel, 	/* Nível atual na árvore de huffman */
               char codigosFeitos[], /* string codificada até a chamada da função */
               char *codigos[]);
/* Imprime um bit único para o arquivo */
void imprime_bit (FILE *f, char b);
/* Usando a codificação no vetor códigos, cria o arquivo codificado/comprimido.
 */
int comprimir_arquivo (FILE *arquivoEntrada, FILE *arquivoSaida, char *codigos[]);
#endif //PROJETOED2_FUNCOES_H
