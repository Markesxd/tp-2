#include <stdint.h>

#define CLUSTER_SIZE 1024
#define INIT 1
#define LOAD 2
#define LS 3
#define MKDIR 4
#define CREATE 5
#define UNLINK 6
#define WRITE 7
#define APPEND 8
#define READ 9
#define QUIT 10


typedef struct {
    uint8_t filename[18];
    uint8_t attributes;
    uint8_t reserved[7];
    uint16_t first_block;
    uint32_t size;
} dir_entry_t;

typedef union{
    dir_entry_t dir[CLUSTER_SIZE / sizeof(dir_entry_t)];
    uint8_t data[CLUSTER_SIZE];
}data_cluster;


void initFat(uint16_t **fat);

void initRootDir(data_cluster *root);

int toInt(char word[10]);

void printFat(uint16_t *fat);

int navegate(data_cluster *dataCluster, char* path);

int navegate1(char name[18], data_cluster cluster);

void extractName(char* path,char name[20]);

void make(data_cluster **dataCluster, uint16_t **fat, char* path, int attribute);

void extractPath(char input[100], char command[10], char path[90]);

void ls(data_cluster *dataCluster, char path[90]);

void printDir(data_cluster *cluster, char path[90]);

int locateInDir(char name[20], data_cluster cluster);

void extractString(char string[50], char path[90]);

void append(data_cluster **dataCluster, char path[90], char str[30], uint16_t **fat);

void write(data_cluster **dataCluster, char path[90], char str[30]);

void read(data_cluster *dataCluster, char path[90]);

void readFile(data_cluster **data_cluster, uint16_t **fat);

void dataFlush(data_cluster *data_cluster, uint16_t *fat);

void unlink(char path[90], data_cluster **clusters, uint16_t **fat);