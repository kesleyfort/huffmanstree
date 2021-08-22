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

int main(int argc, char *argv[]) {
    FILE *f, *g;
    arvore *r;           /* Cria uma struct com a raiz da árvore de Huffman */
    unsigned int n,           /* número de bytes no arquivo */
    frequenciaDeCaracteres[NUM_CARACTERES];  /* Frequência em que cada carácter aparece no arquivo */
    char *codigos[NUM_CARACTERES], /* Vetor de códigos. 1 por posição */
    codigo[NUM_CARACTERES],   /* um vetor pra guardar um código */
    nomeArquivo[100];       /* vetor pra registrar o nome do arquivo de saída */

    /* força a frequência a ser resetada a zero */
    memset(frequenciaDeCaracteres, 0, sizeof(frequenciaDeCaracteres));

    /* Abre o arquivo passado como parâmetro */

    f = fopen(argv[1], "r");
    if (!f) {
        printf("Houve um erro ao abrir o arquivo: %s", argv[1]);
        perror(argv[1]);
        exit(1);
    }

    /* Calcula a frequência em que os caracteres aparecem no arquivo */

    n = buscarFrequencias(f, frequenciaDeCaracteres);
    /*Fecha o arquivo (Referência aos puxões de orelha em Fundamentos I)*/
    fclose(f);

    /* Cria a árvore de huffman */

    r = constroi_arv_Huffman(frequenciaDeCaracteres);

    /* Itera sobre a árvore preenchendo o vetor, preenchendo o vetor códigos */

    transversar(r, 0, codigo, codigos);

    /* Função para criar um arquivo vazio com extensão ".comp" (comprimido) */

    sprintf(nomeArquivo, "%s.comp", argv[1]);
    g = fopen(nomeArquivo, "w");
    if (!g) {
        printf("Houve um erro ao criar o arquivo: %s.comp", argv[1]);
        perror(nomeArquivo);
        exit(1);
    }

    /* Escreve/salva os dados comprimidos no arquivo */

    fwrite(frequenciaDeCaracteres, NUM_CARACTERES, sizeof(int), g);

    /* Escreve o número de caracteres originais no arquivo */

    fwrite(&n, 1, sizeof(int), g);

    /* Abre o arquivo novamente. Necessário, pois operação de codificação e preparação do arquivo
     * não podem ser feitas no mesmo passo.*/

    f = fopen(argv[1], "r");
    if (!f) {
        printf("Houve um erro ao abrir o arquivo: %s", argv[1]);
        perror(argv[1]);
        exit(1);
    }

    /* Faz a conversão do arquivo f no arquivo g usando o vetor códigos */

    int size = comprimir_arquivo(f, g, codigos);
    fclose(f);
    fclose(g);

    /* Saída para o usuário saber quão efetiva foi a conversão do arquivo */
    printf("Conversão realizada com sucesso. \nO arquivo gerado %s é %0.2f%% menor do que o arquivo original %s\n",
           nomeArquivo, ((float) size / (float) n) * 100, argv[1]);
    exit(0);
}

