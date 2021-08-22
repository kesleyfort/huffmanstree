//
// Created by kalves on 17/08/2021.
//

/*
 * Algoritmo de Huffman
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

int nbits, byte_atual, nbytes;

/* Cria uma fila vazia */
void criar_fila_prioridades(FP *p) {
    p->tam_heap = 0;
}

/* Retorna o pai do node da heap */
int pai(int i) {
    return (i - 1) / 2;
}

/* Retorna o filho da esquerda do nó na heap */
int esquerda(int i) {
    return i * 2 + 1;
}

/* Retorna o filho da direita do nó na heap */
int direita(int i) {
    return i * 2 + 2;
}

/* Transforma a subheap com raiz i numa heap. Assume que esquerda(i) e dir(i) são heaps
 */
void heapify(FP *p, int i) {
    int l, r, menor;
    arvore *t;

    l = esquerda(i);
    r = direita(i);

    /* Encontra o menor valor entre os nós */

    if (l < p->tam_heap && p->A[l]->freq < p->A[i]->freq)
        menor = l;
    else
        menor = i;
    if (r < p->tam_heap && p->A[r]->freq < p->A[menor]->freq)
        menor = r;

    /* Se preciso, faz a troca do pai com o menor valor */

    if (menor != i) {
        t = p->A[i];
        p->A[i] = p->A[menor];
        p->A[menor] = t;
        heapify(p, menor);
    }
}

/* Insere um elemento na fila de prioridades. r->freq define a prioridade */
void inserir_fp(FP *p, struct arvore *r) {
    int i;

    p->tam_heap++;
    i = p->tam_heap - 1;

    /* Conforme o algoritmo, o ideal seria colocarmos a árvore R no final do vetor,
     * mas isso vai contra uma das propriedades de heap, então vamos começar do final da fila e ir subindo
	 */
    while ((i > 0) && (p->A[pai(i)]->freq > r->freq)) {
        p->A[i] = p->A[pai(i)];
        i = pai(i);
    }
    p->A[i] = r;
}

/* Remove o elemento na raiz da heap (Conforme o algoritmo, deve ser o elemento com menor repetição) */
struct arvore *extrair_min_fp(FP *p) {
    struct arvore *r;

    if (p->tam_heap == 0) {
        printf("A heap está vazia\n");
        exit(1);
    }

    /* Pega o valor da raiz da árvore */

    r = p->A[0];

    /*Utiliza o heapsort para pegar o último valor e o inserir na raiz*/

    p->A[0] = p->A[p->tam_heap - 1];

    /* Diminui a fila */

    p->tam_heap--;

    /* esq e dir são heaps, então transforme a raiz numa heap */

    heapify(p, 0);
    return r;
}

/* Lê o arquivo para calcular a frequência dos carácteres e os insere no vetor v
 */
unsigned int buscarFrequencias(FILE *f, unsigned int v[]) {
    int r, n;

    /* n será o contador para armazenar os caracteres */

    /*for indeterminado. Poderia colocar while, mas for é mais bonitinho*/
    for (n = 0;; n++) {

        /* Decidi usar a função fgetc() pois ela pega um char e converte num int
         * e ficaria mais fácil dessa forma */

        r = fgetc(f);

        /* Verifica se não existe mais caracteres, se sim, sai do loop */
        if (feof(f)) break;

        /* Adiciona mais um caractere */

        v[r]++;
    }
    return n;
}

/* Cria a árvore de huffman baseando nas frequências
 * encontradas no vetor freq (usando o algoritmo de Huffman) */
arvore *constroi_arv_Huffman(const unsigned int frequencias[]) {
    int i, n;
    /*Talvez eu perca ponto aqui por não ter usado nomes identificáveis, mas preguiça.*/
    arvore *x, *y, *z;
    FP p;

    /* Cria uma fila vazia */

    criar_fila_prioridades(&p);

    /* Faz uma árvore/heap para cada um dos caracteres com seus respectivos valores e frequências.
     * (Aqui que a mágica acontece)
	 */
    for (i = 0; i < NUM_CARACTERES; i++) {
        x = malloc(sizeof(arvore));

        /* Cria uma folha na árvore */
        x->esq = NULL;
        x->dir = NULL;
        x->freq = frequencias[i];
        x->ch = (char) i;

        /* Insere o nó na heap */
        inserir_fp(&p, x);
    }

    /* Aqui temos várias árvores pingadas */

    n = p.tam_heap - 1; /* tam_heap não é invariável */

    /* Após iterarmos sobre o tam_heap fazendo sempre tam_heap-- teremos apenas uma arvóre de nós esq*/
    for (i = 0; i < n; i++) {

        /*
         * Cria um nó z usando os dois nós x e y menos frequentes
		 */
        z = malloc(sizeof(arvore));
        x = extrair_min_fp(&p);
        y = extrair_min_fp(&p);
        z->esq = x;
        z->dir = y;

        /* a frequência de z é a soma das frequências de x e y */

        z->freq = x->freq + y->freq;

        /* Após realizar os devidos cálculos de prioridade, insere de novo na fila */

        inserir_fp(&p, z);
    }

    /* retorna o único valor esq na árvore (Neste caso, a árvore de huffman inteira) */

    return extrair_min_fp(&p);
}

/* transversa a árvore, construindo a codificação do arquivo no vetor codigos*/
void transversar(arvore *r,    /* Raiz da sub-árvore*/
                 int nivel,    /* Nível atual na árvore de huffman */
                 char codigosFeitos[], /* 'string' codificada até a chamada da função */
                 char *codigos[]) {/* Vetor de códigos */

    /* Se for um nó esq */

    if ((r->esq == NULL) && (r->dir == NULL)) {

        /* defini um ponto final nulo (0) */

        codigosFeitos[nivel] = 0;

        /* Cria uma cópia do código e o insere no vetor */

        codigos[r->ch] = strdup(codigosFeitos);
    } else {

        /* Não é um nó folha. Defina a esquerda com um 'bit' 0 */

        codigosFeitos[nivel] = '0';
        transversar(r->esq, nivel + 1, codigosFeitos, codigos);

        /* Define a direita como um 'bit' 1 */

        codigosFeitos[nivel] = '1';
        transversar(r->dir, nivel + 1, codigosFeitos, codigos);
    }
}

/* Imprime um bit único para o arquivo */
void imprime_bit(FILE *f, char b) {

    /* converte um 'byte' a esquerda */

    byte_atual <<= 1;

    /* Coloca um 1 no final do byte se b for 1*/
    /*Quem diria que arquitetura de computadores seria útil para algo, né*/
    if (b == '1') byte_atual |= 1;

    /* adiciona mais um bit */

    nbits++;

    /* escreve o byte */

    if (nbits == 8) {
        fputc(byte_atual, f);
        nbytes++;
        nbits = 0;
        byte_atual = 0;
    }
}

/* Usando a codificação no vetor 'códigos', cria o arquivo codificado/comprimido.
 */
int comprimir_arquivo(FILE *arquivoEntrada, FILE *arquivoSaida, char *codigos[]) {
    unsigned char caractere;
    char *s;

    /* inicializa as variáveis globais para usar no imprime_bit()
     * A minha ideia era não usar para tentar deixar o código mais desacoplado,
     * porém não iria dar boa*/

    byte_atual = 0;
    nbits = 0;
    nbytes = 0;

    /* Continua até o fim do arquivo.
     * De novo um for indefinido. Bem mais legível (e legal) que while.
     * Gosto de usar porque me sinto um programador caro.
     */
    for (;;) {

        /* lê um caractere */

        caractere = fgetc(arquivoEntrada);
        if (feof(arquivoEntrada)) break;

        /* Coloca sua correspondente bitstring no arquivoSaida */

        for (s = codigos[caractere]; *s; s++) imprime_bit(arquivoSaida, *s);
    }

    /* escreve o último byte (0 para indicar final) */

    while (nbits) imprime_bit(arquivoSaida, '0');
    return nbytes;
}

