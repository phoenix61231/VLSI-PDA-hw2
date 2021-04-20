cell *Extract_max(blk *arr, bool *all_lock, int Pmax, int S_p, int S_cnt);
void Unlock_cells(blk *arr, int Pmax, net **net_arr);
step *Free_step_list(step *list);
step *Reverse_step(blk *a_arr, blk *b_arr, step *list, cell **cell_arr, net **net_arr, int Pmax, int *S_a, int *S_b, int *A_cnt, int *B_cnt);
int Find_min_cut(blk *a_arr, blk *b_arr, int Pmax, cell **cell_arr, net **net_arr, int *S_a, int *S_b, int S_cnt, int *A_cnt, int *B_cnt, int N_cnt);