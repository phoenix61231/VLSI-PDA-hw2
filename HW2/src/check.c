#include "main.h"
#include "check.h"

void Check_array(blk *arr, int Pmax){
	int cnt = 0;
	while(cnt <= 2*Pmax){
		if(arr[cnt].tail==NULL && arr[cnt].head!=NULL){
			printf("head != n, tail = n\n");
			exit(1);
		}
		else if(arr[cnt].head==NULL && arr[cnt].tail!=NULL){
			printf("head = n, tail != n\n");
			exit(1);
		}
		else{
			printf("normal\n");
		}
		cnt++;
	}
}

int Traverse_array(blk *arr, int Pmax, int *A_cnt, int *B_cnt, int *S_cnt){
	int a_cnt = 0, b_cnt = 0, s_cnt = 0;
	int cnt = 0;
	cell *curr;

	while(cnt <= 2*Pmax){
		curr = arr[cnt].head;
		while(curr!=NULL){
			
			if(curr->partition == PARTITION_A) a_cnt+=1;
			else if(curr->partition == PARTITION_B) b_cnt+=1;

			s_cnt+=1;

			curr = curr->next;
		}	

		cnt++;	
	}

	if(s_cnt == (a_cnt+b_cnt)){
		*A_cnt = a_cnt;
		*B_cnt = b_cnt;
		*S_cnt = s_cnt;

		return s_cnt;
	}
	else return -1;	
}

void Traverse_list(cell *list){
	cell *curr;
	curr = list;
	
	while(curr!=NULL){
		printf("%d\n", curr->size);
		curr = curr->next;
	}
}

int Net_cut(net **net_arr, int N_cnt){
	int cnt = 0, i;

	for(i = 1; i<=N_cnt; i++){
		if(net_arr[i]==NULL) continue;
		if(net_arr[i]->A_cnt!=0 &&net_arr[i]->B_cnt!=0)	cnt++;
	}
	return cnt;
}