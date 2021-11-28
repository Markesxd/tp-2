#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "main.h"

int main(){
    uint16_t *fat;
    fat = (uint16_t*) malloc(sizeof(uint16_t) * 4096);
    data_cluster *clusters;
    clusters = (data_cluster*) malloc(sizeof(data_cluster) * 4087);

    char escolha[10];
    do{
    printf(">");
    scanf(" %s", escolha);
    
    switch(toInt(escolha)){
        case INIT:
            printf("init\n");
            initFat(&fat);
            printFat(fat);
        break;

        case LOAD:
            printf("load\n");
        break;

        case LS:
            printf("ls\n");
        break;

        case MKDIR:
            printf("mkdir\n");
        break;

        case CREATE:
            printf("create\n");
        break;

        case UNLINK:
            printf("unlink\n");
        break;

        case WRITE:
            printf("write\n");
        break;

        case APPEND:
            printf("append\n");
        break;

        case READ:
            printf("read\n");
        break;
    }
    }while(toInt(escolha) != QUIT);
    return 0;
}

void initFat(uint16_t **fat){
    (*fat)[0] = 0xfffd;
    for(int i = 1; i < 9; i++){
        (*fat)[i] = 0xfffe;
    }
    (*fat)[9] = 0xffff;
    for(int i = 10; i < 4096; i++){
        (*fat)[i] = 0x0;
    }
}

int toInt(char word[10]){
    if(strcmp(word, "init") == 0) return INIT;
    if(strcmp(word, "load") == 0) return LOAD;
    if(strcmp(word, "ls") == 0) return LS;
    if(strcmp(word, "mkdir") == 0) return MKDIR;
    if(strcmp(word, "create") == 0) return CREATE;
    if(strcmp(word, "unlink") == 0) return UNLINK;
    if(strcmp(word, "write") == 0) return WRITE;
    if(strcmp(word, "append") == 0) return APPEND;
    if(strcmp(word, "read") == 0) return READ;
    if(strcmp(word, "quit") == 0) return QUIT;
}

void printFat(uint16_t *fat){
    for(int i = 0; i < 4096; i++){
        printf("%x\n", fat[i]);
    }
}

void makeDir(data_cluster **data_cluster){
    
}

// init - inicializar o sistema de arquivos com as estruturas de dados, semelhante a formatar o sistema de
// arquivos virtual
//  load - carregar o sistema de arquivos do disco
//  ls [/caminho/diretorio] - listar diretorio
//  mkdir [/caminho/diretorio] - criar diretorio
//  create [/caminho/arquivo] - criar arquivo
//  unlink [/caminho/arquivo] - excluir arquivo ou diretorio (o diretorio precisa estar vazio)
//  write "string"[/caminho/arquivo] - escrever dados em um arquivo (sobrescrever dados)
//  append "string"[/caminho/arquivo] - anexar dados em um arquivo
//  read [/caminho/arquivo] - ler o conteudo de um arquivo