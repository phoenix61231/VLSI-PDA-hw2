#include "main.h"
#include "find_min_cut.h"
#include "move_cell.h"
#include "check.h"
#include "build_list.h"

cell *Extract_max(blk *arr, bool *all_lock, int Pmax, int S_p, int S_cnt){
	/*	extract max from a_arr or b_arr	
		if c==NULL find a new one
		or find the max after c			*/
	cell *curr = NULL;
	int cnt = 2*Pmax;
	
	while(cnt >= 0){
		curr = arr[cnt].head;
		while(curr!=NULL){
			if(curr->lock) break;

			if(!curr->lock && (S_p+curr->size)<(S_cnt*11/20)){
				*all_lock = false;
				return curr;
			} 			
			curr = curr->next;
		}
		cnt--;
	}

	return NULL;
}

void Unlock_cells(blk *arr, int Pmax, net **net_arr){
	/*	unlock all cells and count cut	*/
	int cnt = 0;
	cell *curr;

	/*Check_array(arr, Pmax);*/
	while(cnt <= 2*Pmax){
		curr = arr[cnt].head;

		while(curr!=NULL){			
			curr->lock = false;			
			curr = curr->next;
		}

		cnt++;
	}
}

step *Free_step_list(step *list){
	step *rm_step;
	int cnt = list->num;
	
	while(cnt>=1){
		rm_step = list;
		list = list->next;
		
		free(rm_step);
		cnt--;
	}

	return NULL;
}

step *Reverse_step(blk *a_arr, blk *b_arr, step *list, cell **cell_arr, net **net_arr, int Pmax, int *S_a, int *S_b, int *A_cnt, int *B_cnt){
	/*	reverse step and output cut improve	*/
	/*	while move cell	*/
	step *step_t = list;
	
	while(step_t!=NULL){
		if(step_t->best_num == step_t->num){			
			return step_t;}
		
		if(step_t->c->partition==PARTITION_A){
			Move_cell(a_arr, b_arr, step_t->c, cell_arr, net_arr, Pmax);
			/*printf("%d A back to B\n", step_t->c->num);*/
			*S_b = *S_b + step_t->c->size;
			*S_a = *S_a - step_t->c->size;
			*A_cnt -= 1;
			*B_cnt += 1;
		}
		else if(step_t->c->partition==PARTITION_B){
			Move_cell(a_arr, b_arr, step_t->c, cell_arr, net_arr, Pmax);
			/*printf("%d A back to B\n", step_t->c->num);*/
			*S_a = *S_a + step_t->c->size;
			*S_b = *S_b - step_t->c->size;
			*A_cnt += 1;
			*B_cnt -= 1;
		}

		/*printf("Reverse step %d\n", step_t->num);*/

		step_t = step_t->next;
	}
	return NULL;	
}

int Find_min_cut(blk *a_arr, blk *b_arr, int Pmax, cell **cell_arr, net **net_arr, int *S_a, int *S_b, int S_cnt, int *A_cnt, int *B_cnt, int N_cnt){
	int curr_cut = 0, curr_gain = 0;
	cell *curr = NULL, *curr_A = NULL, *curr_B = NULL;
	step *list = NULL, *step_tmp = NULL;
	bool all_lock = false;
	int Pass = 1;
	int step_cnt = 1;
	
	while(1){
		printf("Pass : %d\n", Pass);
		/*printf(". ");	*/	
		step_cnt = 1;
	
		while(1){/*step_cnt <= *A_cnt+*B_cnt){*/
			all_lock = true;
			curr_A = Extract_max(a_arr, &all_lock, Pmax, *S_b, S_cnt);
			curr_B = Extract_max(b_arr, &all_lock, Pmax, *S_a, S_cnt);
			if(all_lock) break;
			
			if(curr_A!=NULL && curr_B!=NULL){
				if(curr_A->gain > curr_B->gain) curr = curr_A;
				else if(curr_A->gain < curr_B->gain) curr = curr_B;
				else{
					if(curr_A->size > curr_B->size) curr = curr_A;
					else curr = curr_B;
				}
			}
			else if(curr_A!=NULL && curr_B==NULL)	curr = curr_A;
			else if(curr_B!=NULL && curr_A==NULL)	curr = curr_B;
			else break;

			curr_gain = curr->gain;

			/*printf("Max: %d\n", curr->gain);*/

			if(curr->partition==PARTITION_A){ /*&& (*S_b+curr->size)<S_cnt*11/20){*/
				curr->lock = true;
				Move_cell(a_arr, b_arr, curr, cell_arr, net_arr, Pmax);
				*S_b = *S_b + curr->size;
				*S_a = *S_a - curr->size;
				*A_cnt -= 1;
				*B_cnt += 1;
			}
			else if(curr->partition==PARTITION_B){ /*&& (*S_a+curr->size)<S_cnt*11/20){*/
				curr->lock = true;
				Move_cell(a_arr, b_arr, curr, cell_arr, net_arr, Pmax);
				*S_a = *S_a + curr->size;
				*S_b = *S_b - curr->size;
				*A_cnt += 1;
				*B_cnt -= 1;
			}

			/*printf("pass %d, step %d\n", Pass, step_cnt);*/

			/*Update_gains(a_arr, Pmax, net_arr);
			Update_gains(b_arr, Pmax, net_arr);*/

			/*	cut improve & record movement	*/
			step_tmp = malloc(sizeof(step));
			step_tmp->num = step_cnt;
			step_tmp->c = curr;
			step_tmp->gain = curr_gain;
			
			if(step_cnt==1){
				step_tmp->next = NULL;
				step_tmp->partial_sum = step_tmp->gain;

				step_tmp->best_psum = step_tmp->partial_sum;
				step_tmp->best_num = step_cnt;				
			}
			else{
				step_tmp->next = list;

				step_tmp->partial_sum = step_tmp->gain + list->partial_sum;
				
				if(step_tmp->partial_sum >= list->best_psum){
					step_tmp->best_psum = step_tmp->partial_sum;
					step_tmp->best_num = step_tmp->num;
				}
				else{
					step_tmp->best_psum = list->best_psum;
					step_tmp->best_num = list->best_num;
				}
			}	

			list = step_tmp;
			step_cnt+=1;						
		}/*	137	*/
		
		step_tmp = Reverse_step(a_arr, b_arr, list, cell_arr, net_arr, Pmax, S_a, S_b, A_cnt, B_cnt);	/*	155	*/	
		
		if(list->best_psum == 0) break;	

		Update_gains(a_arr, Pmax, net_arr);
		Update_gains(b_arr, Pmax, net_arr);	

		Unlock_cells(a_arr, Pmax, net_arr);
		Unlock_cells(b_arr, Pmax, net_arr);
			
		list = Free_step_list(list);

		if(Pass>=(670000/N_cnt)) break;
		Pass++;
	}
	
	curr_cut = Net_cut(net_arr, N_cnt);
	return curr_cut;
}