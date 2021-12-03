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
    int i = 0;
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
            if(i % 2 == 0){
                clusters->data[0] 10;
            } else {
                clusters->dir->attributes = 1;
            }
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

// void makeDir(data_cluster **dataCluster, uint16_t *fat, char* path){
//     int dirI = navegate(dataCluster, path);
//     int i = 0;
//     strcpy((*dataCluster)[dirI].dir->filename, name); 
//     while(fat[i] != 0x0) i++;
//     fat[i] = 0xffff;
// }

int navigate(data_cluster *dataCluster, char* path){
    int k = 0, dirI = 0;
    char name[18];
    for(int j = 1; j < strlen(path); j++){
        if(path[j] == '/') {
            dirI = navegate1(name, dataCluster[dirI]);
            k = 0;
        };    
        name[k++] = path[j];
    }
    return dirI;
}

int navegate1(char name[18], data_cluster cluster){
    for(int i = 0; i < 32; i++){
        if(strcmp(cluster.dir[i].filename, name) == 0) return i;
    }
    printf("Diretorio ou arquivo nao encontrado\n");
}
//fat.part

void dataFlush(data_cluster *data_cluster, uint16_t *fat){
    FILE *pont_arq;
    pont_arq = fopen("fat.part", "wb"); //create binary file

    if (pont_arq == NULL){ //error file
        printf("arquivo com problemas");
    }
    fwrite(fat, 4096, sizeof(fat), pont_arq);
    fwrite(data_cluster, 4087, sizeof(data_cluster), pont_arq);
    fclose(pont_arq);
}

void readFile(data_cluster **data_cluster, uint16_t **fat){
    FILE *pont_arq;
    pont_arq = fopen("fat.part","rb");
    
    if(pont_arq == NULL){
        printf("Erro ao ler arquivo");
    }
    fread(fat, sizeof(fat), 4096, pont_arq);
    fread(data_cluster,sizeof(data_cluster), 4087,pont_arq);
    fclose(pont_arq);
}

// void ls(data_cluster *dataCluster, char* path){
//     int ID = navigate(dataCluster, path);
//     for(int i = 0; i < 32; i++){
//         for(int j = 0; j < 18; j++){
//             printf("%c",dataCluster[ID].dir[i].filename[j]);
//             printf("\n");
//         }        
//     }   
// }
// while(i == 32){
//     int i;
//     if(datacluster[id] == i){
//         printf(data_cluster);
//     }
//     else
//     i++;
// }

// init - inicializar o sistema de arquivos com as estruturas de dados, semelhante a formatar o sistema de
// arquivos virtual
//  load - carregar o sistema de arquivos do disco         FEITO
//  ls [/caminho/diretorio] - listar diretorio           COMEÇOU
//  mkdir [/caminho/diretorio] - criar diretorio         METADE
//  create [/caminho/arquivo] - criar arquivo          
//  unlink [/caminho/arquivo] - excluir arquivo ou diretorio (o diretorio precisa estar vazio)
//  write "string"[/caminho/arquivo] - escrever dados em um arquivo (sobrescrever dados)   
//  append "string"[/caminho/arquivo] - anexar dados em um arquivo
//  read [/caminho/arquivo] - ler o conteudo de um arquivo    