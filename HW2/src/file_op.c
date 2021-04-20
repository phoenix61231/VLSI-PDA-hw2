#include "main.h"
#include "file_op.h"
#include "check.h"

FILE *Read_file(char *file){
	FILE *f;
	char path[50] = ("../testcases/");
	
	strcat(path, file);
	f = fopen(path, "r");
	if(f == NULL){
		printf("Unable to open %s.\n", file);		
		return NULL;
	}

	printf("Open %s successfully.\n", file);
	return f;
}

FILE *Write_file(char *file){
	FILE *f;
	char path[50] = ("../output/");

	strcat(path, file);
	
	if(fopen(path, "r")!=NULL) remove(path);
	f = fopen(path, "w");

	printf("Open %s successfully.\n", file);
	return f;
}

void Output_result_file(blk *arr, FILE *out, int Pmax){
	int cnt = 0;
	cell *curr = NULL;
	
	while(cnt <= 2*Pmax){
		curr = arr[cnt].head;
		while(curr!=NULL){
			fprintf(out, "c%d\n", curr->num);		
			curr = curr->next;
		}
		cnt++;
	}
}