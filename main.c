#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "main.h"

int main(){
    uint16_t *fat;
    fat = (uint16_t*) malloc(sizeof(uint16_t) * 4096);
    initFat(&fat);
    data_cluster *clusters;
    clusters = (data_cluster*) malloc(sizeof(data_cluster) * 4087);
    initRootDir(&(clusters[0]));
    char input[100], command[10], path[90], string[50];
    int att;
    do{
        printf(">");
        fgets(input, 100, stdin);
        extractPath(input, command, path);
        
        switch(toInt(command)){
            case INIT:
                printf("init\n");
                initFat(&fat);
                printFat(fat);
            break;

            case LOAD:
                readFile(&clusters, &fat);
            break;

            case LS:
                ls(clusters, path);
            break;

            case MKDIR:
                att = 1;
                create(&clusters, &fat, path, att);
            break;

            case CREATE:
                att = 0;
                create(&clusters, &fat, path, att);
            break;

            case UNLINK:
                printf("unlink\n");
            break;

            case WRITE:
                extractString(string, path);
                write(&clusters, path, string);
            break;

            case APPEND:
                extractString(string, path);
                append(&clusters, path, string);
            break;

            case READ:
                read(clusters, path);
            break;
            
            case QUIT:
            break;

            default:
                printf("Malformed command\n");
            break;
        }
        dataFlush(clusters, fat);
    }while(toInt(command) != QUIT);
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

void initRootDir(data_cluster *root){
    for(int i = 0; i < 32; i++){
        (*root).dir[i].size = 0;
    }
}

void extractPath(char input[100], char command[10], char path[90]){
    int i = 0, j = 0;
    while(input[i] != ' '){
        if(input[i] == '\n') {
            command[i] = '\0';
            path[0] = '/';
            path[1] = '\0';  
            return;
        }
        command[i] = input[i];
        i++;
    };
    command[i++] = '\0';
    while(input[i] != '\n') path[j++] = input[i++];
    path[j] = '\0';
}

void extractString(char string[50], char path[90]){
    int i = 1, j = 0;
    while(path[i] != '"'){
        string[j++] = path[i++];
    }
    string[j] = '\0';
    i++;
    j = 0;
    while(path[i] != '\0'){
        path[j++] = path[i++];
    }
    path[j] = '\0';
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

void create(data_cluster **dataCluster, uint16_t **fat, char path[90], int attribute){
    int dirI = navegate(*dataCluster, path);

    char name[20];
    extractName(path, name);

    int newDirI = 0;
    while((*fat)[newDirI] != 0x0) newDirI++;
    (*fat)[newDirI] = 0xffff;

    int j = 0;
    while((*dataCluster)[dirI].dir[j].size != 0 && j++ < 32);
    if(j >= 32){
        printf("Diretorio cheio\n");
        return;
    }
    strcpy((*dataCluster)[dirI].dir[j].filename, name);
    (*dataCluster)[dirI].dir[j].attributes = attribute;
    (*dataCluster)[dirI].dir[j].first_block = newDirI;
    (*dataCluster)[dirI].dir[j].size = 1;

    for(int k = 0; k < 32; k++){
        (*dataCluster)[newDirI].dir[k].size = 0;
    }
}

void extractName(char path[90], char name[20]){

    int i = strlen(path), j = 0;
    char nameAux[20];
    while(path[--i] != '/' && i >= 0){
        nameAux[j++] = path[i];
    }
    i = 0;
    while(--j >= 0){
        name[i++] = nameAux[j];
    }
    name[i] = '\0';
    if(strlen(name) == 0){
        name[0] = '/';
        name[1] = '\0';
    }
}

int navegate(data_cluster *dataCluster, char *path){
    int k = 0, dirI = 0;
    char name[19];
    for(int j = 1; j < strlen(path); j++){
        if(path[j] == '/') {
            name[k] = '\0';
            dirI = navegate1(name, dataCluster[dirI]);
            if(dirI == -1) return -1;
            k = 0;
            continue;
        };    
        name[k++] = path[j];
    }
    return dirI;
}

int navegate1(char name[19], data_cluster cluster){
    for(int i = 0; i < 32; i++){
        if(strcmp(cluster.dir[i].filename, name) == 0) return cluster.dir[i].first_block;
    }
    printf("Diretorio ou arquivo nao encontrado\n");
    return -1;
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
        perror("failed to read file");
    }
    fread(fat, sizeof(fat), 4096, pont_arq);
    fread(*data_cluster,sizeof(data_cluster), 4087,pont_arq);
    fclose(pont_arq);
}

void ls(data_cluster *dataCluster, char path[90]){
    int dirI = navegate(dataCluster, path);
    char name[20];
    extractName(path, name);
    if(strlen(name) == 1 && name[0] == '/'){} else {
    int inDirI = locateInDir(name, dataCluster[dirI]);
    dirI = dataCluster[dirI].dir[inDirI].first_block;
    } 
    for(int i = 0; i < 32; i++){
        if(dataCluster[dirI].dir[i].size == 0) continue;
        printf("%s ",dataCluster[dirI].dir[i].filename);
    }
    printf("\n");
}

void printDir(data_cluster *clusters, char path[90]){
    int dirI = navegate(clusters, path);
    char name[20];
    extractName(path, name);
    int fileInDir;
    if(strlen(name) != 0) 
    fileInDir = locateInDir(name, clusters[dirI]);
    printf("%s\n", clusters[dirI].dir[fileInDir].filename);
    printf("atribute: %d\n", clusters[dirI].dir[fileInDir].attributes);
    printf("first block: %d\n", clusters[dirI].dir[fileInDir].first_block);
    printf("size: %d\n", clusters[dirI].dir[fileInDir].size);
}

int locateInDir(char name[20], data_cluster cluster){
    for(int i = 0; i < 32; i++){
        if(strcmp(cluster.dir[i].filename, name) == 0) return i;
    }
    printf("No such file in directory\n");
}



void write(data_cluster **dataCluster, char path[90], char str[30]){
    int id = navegate(*dataCluster, path);
    char name[20];
    extractName(path, name);
    int inDir = locateInDir(name, (*dataCluster)[id]);
    id = (*dataCluster)[id].dir[inDir].first_block;
    if(str[0] == '\0'){
        printf("Empty string\n");
    }else{
        strcpy((*dataCluster)[id].data,str);
    }
}

void read(data_cluster *dataCluster, char path[90]){
    int id = navegate(dataCluster, path);
    char name[20];
    extractName(path, name);
    int inDir = locateInDir(name, dataCluster[id]);
    id = dataCluster[id].dir[inDir].first_block;
    puts(dataCluster[id].data);
}

void append(data_cluster **dataCluster, char path[90], char str2[30]){
    int id = navegate(*dataCluster, path);
    char name[20];
    extractName(path, name);
    int inDir = locateInDir(name, (*dataCluster)[id]);
    id = (*dataCluster)[id].dir[inDir].first_block;
    if(str2[0] == '\0'){
        printf("Empty string\n");
    }else{
        strcat((*dataCluster)[id].data, str2);
    }
}


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
//  load - carregar o sistema de arquivos do disco      FEITO (TESTE)
//  ls [/caminho/diretorio] - listar diretorio        FEITO (APROVADO PELO INMETRO)
//  mkdir [/caminho/diretorio] - criar diretorio      FEITO (APROVADO PELO INMETRO)
//  create [/caminho/arquivo] - criar arquivo           FEITO (APROVADO PELO INMETRO)
//  unlink [/caminho/arquivo] - excluir arquivo ou diretorio (o diretorio precisa estar vazio)   FALTANDO
//  write "string"[/caminho/arquivo] - escrever dados em um arquivo (sobrescrever dados)             FEITO (APROVADO PELO INMETRO)
//  append "string"[/caminho/arquivo] - anexar dados em um arquivo      FEITO (APROVADO PELO INMETRO)
//  read [/caminho/arquivo] - ler o conteudo de um arquivo            FEITO (APROVADO PELO INMETRO)