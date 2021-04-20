cell *Build_Cell_Array(cell **cell_array, FILE *file, int *c_cnt, int *s_cnt, int *Smax);
cell *Sort_list(cell *list, int Smax);
cell *Insert_Cell_list(cell *head, cell *c);
int Build_Net_Array(net **net_array, cell **cell_array, FILE *file, int *p_cnt, int *Pmax);
int Find_Init_Partition(cell *list, cell **A, cell **B, int S, net **net_arr, int *Cut);