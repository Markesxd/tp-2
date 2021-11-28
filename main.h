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

int toInt(char word[10]);

void printFat(uint16_t *fat);
