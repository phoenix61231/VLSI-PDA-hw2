#include "main.h"
#include "file_op.h"
#include "init.h"
#include "build_list.h"
#include "move_cell.h"
#include "find_min_cut.h"
#include "check.h"

int main(int argc, char *argv[]){
	
	FILE *f_nets = NULL, *f_cells = NULL, *f_out = NULL;
	int C_cnt = 0, N_cnt = 0, P_cnt = 0, S_cnt = 0, Pmax = 0, Smax = 0;
	int S_a = 0, S_b = 0, Cut = 0;
	int A_cnt = 0, B_cnt = 0;
	cell *list_A = NULL, *list_B = NULL;
	blk *A_array, *B_array;
	
	if(argc != 4){
		printf("\n Error. Takes 3 arguments\n");
		return -1;
	}

	f_nets = fopen(argv[1], "r");
	if(f_nets == NULL){
		printf("Unable to open %s.\n", argv[1]);		
		return -1;
	}
	else printf("Open %s successfully.\n", argv[1]);

	f_cells = fopen(argv[2], "r");
	if(f_cells == NULL){
		printf("Unable to open %s.\n", argv[2]);		
		return -1;
	}
	else printf("Open %s successfully.\n", argv[2]);

	f_out = fopen(argv[3], "w");
	printf("Open %s successfully.\n", argv[3]);	
	
	/*f_nets = Read_file(argv[1]);	
	if(f_nets == NULL)	return 1;

	f_cells = Read_file(argv[2]);
	if(f_cells == NULL)	return 1;

	f_out = Write_file(argv[3]);*/

	printf("Open files\n");
	
	/*	read .cell file in
		malloc for each cell and store cells' address into array	
		make a list and sort all cells									*/
	cell *cell_array[CELL_ARRAY_SIZE];
	cell *sort_list;
	sort_list = Build_Cell_Array(cell_array, f_cells, &C_cnt, &S_cnt, &Smax);
	if(sort_list==NULL) printf("sort_list is null.\n");
	printf("Build Cell Array.\n");	
	
	sort_list = Sort_list(sort_list, Smax);
	printf("Sort list.\n");
	/*Traverse_list(sort_list);*/

	/*	read .nets file in
		set cells' nets and link to cell	*/	
	net *net_array[NET_ARRAY_SIZE];
	N_cnt = Build_Net_Array(net_array, cell_array, f_nets, &P_cnt, &Pmax);
	printf("Build Net Array.\n");
	
	/*	find initial partition	*/
	S_a = Find_Init_Partition(sort_list, &list_A, &list_B, S_cnt, net_array, &Cut);
	S_b = S_cnt-S_a;	
	if(list_A==NULL || list_B==NULL) printf("list_A or list_B is NULL.\n");
	printf("Find initial partition.\n");
	
	
	/*	Build list_A and list_B	*/
	A_array = (blk *)malloc((Pmax*2+1) * sizeof(blk));
	B_array = (blk *)malloc((Pmax*2+1) * sizeof(blk));
	printf("Malloc A_array B_array.\n");	

	Build_list(A_array, &list_A, Pmax, cell_array, net_array, &A_cnt);
	Build_list(B_array, &list_B, Pmax, cell_array, net_array, &B_cnt);
	printf("Build list A and list B.\n");


	/*Check_array(A_array, Pmax);
	Check_array(B_array, Pmax);
	return 0;*/
	
	/*printf("\n%d\n", list_A->next->partition);*/

	/*	Start to move	*/
	/*	make a move & update gain & lock the cell
		action list & cut update & partial sum
		calculate the improvement
		next pass
		until no improvement						*/

	/*	A_array, B_array, Pmax, cell_array, net_array	*/
	/*printf("Pmax : %d\n\n", Pmax);*/

	Cut = Find_min_cut(A_array, B_array, Pmax, cell_array, net_array, &S_a, &S_b, S_cnt, &A_cnt, &B_cnt, N_cnt);
	printf("Find min cut.\n");

	fprintf(f_out, "cut_size %d\n", Cut);
	fprintf(f_out, "A %d\n", A_cnt);	
	Output_result_file(A_array, f_out, Pmax);
	fprintf(f_out, "\nB %d\n", B_cnt);
	Output_result_file(B_array, f_out, Pmax);
	printf("Output to file\n");
	
	printf("C_cnt = %d\n", C_cnt);
	printf("Smax = %d, S_cnt = %d, S_a = %d, S_b = %d\n", Smax, S_cnt, S_a, S_b);
	printf("N_cnt = %d\n", N_cnt);
	printf("P_cnt = %d, Pmax = %d\n", P_cnt, Pmax); 
	printf("Cut = %d\n", Cut);
	printf("A_cnt = %d, B_cnt = %d\n", A_cnt, B_cnt);
	
	fclose(f_nets);
	fclose(f_cells);
	fclose(f_out);

	return 0;

}