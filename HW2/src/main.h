#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define nets_array_size 200
#define cells_array_size 10000

#define PARTITION_A 53
#define PARTITION_B 72

#define CELL_ARRAY_SIZE 200000
#define NET_ARRAY_SIZE 200000

#define INF 0x3f3f3f3f

struct cells{	
	int num;
	int size;
	int pins;	
	int nets[nets_array_size];
	
	int gain;
	bool lock;
	int partition;
	
	struct cells *next;
	struct cells *prev;
};
typedef struct cells cell;

struct nets{
	int num;
	int pins;
	int cells[cells_array_size];

	bool cutstate;
	bool update;
	bool critical;
	
	int A_cnt;
	int B_cnt;
};
typedef struct nets net;

struct steps{
	int num;
	cell *c;

	int gain;
	int partial_sum;

	int best_psum;
	int best_num;

	struct steps *next;
};
typedef struct steps step;

struct blks{
	cell *head;
	cell *tail;
};
typedef struct blks blk;