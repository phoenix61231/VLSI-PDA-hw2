#include "main.h"
#include "build_list.h"
#include "check.h"

void Find_gain(net **net_arr, cell *c){
	int cnt = 0;
	
	/*printf("%d %d\n", c->pins, c->partition);*/
	while(cnt < c->pins){
		if(c->partition == PARTITION_A){
			if(net_arr[c->nets[cnt]]->A_cnt == 1){
				c->gain += 1;
			}
			else if(net_arr[c->nets[cnt]]->B_cnt == 0){
				c->gain -= 1;
			}	
		}
		else if(c->partition == PARTITION_B){
			if(net_arr[c->nets[cnt]]->B_cnt == 1){
				c->gain += 1;
			}
			else if(net_arr[c->nets[cnt]]->A_cnt == 0){
				c->gain -= 1;
			}
		}
		cnt++;
	}
	/*printf("%d\n", c->gain);*/
}

void Insert_array(blk *arr, cell *c, int Pmax){	
	if(arr[c->gain + Pmax].head == NULL){
		arr[c->gain + Pmax].head = c;
		arr[c->gain + Pmax].tail = c;
		
		c->next = NULL;
		c->prev = NULL;
	}
	else{
		c->next = arr[c->gain + Pmax].head;
		c->prev = NULL;

		arr[c->gain + Pmax].head->prev = c;
		arr[c->gain + Pmax].head = c;
		

		/*arr[c->gain + Pmax].tail->next = c;
		
		c->next = NULL;
		c->prev = arr[c->gain + Pmax].tail;
		arr[c->gain + Pmax].tail = c; */
	}
	/*if(c->gain+Pmax < 0) printf("%d\n", c->gain+Pmax);*/
}

void Build_list(blk *arr, cell **list, int Pmax, cell **cell_arr, net **net_arr, int *P_cnt){
	int cnt = 0;
	
	/*	initial arr	*/
	while(cnt <= 2*Pmax){
		arr[cnt].head = NULL;
		arr[cnt].tail = NULL;
		cnt++;
	}
	
	/*Check_array(arr, Pmax);*/
	
	cell *next = NULL, *curr = *list;
	
	while(curr != NULL){
		/*printf("find gain: C%d\n", curr->num);*/
		Find_gain(net_arr, curr);
		next = curr->next;
		Insert_array(arr, curr, Pmax);
		curr = next;
		*P_cnt += 1;
	}	

	/*arr[0] = (*list)-> next;*/
}