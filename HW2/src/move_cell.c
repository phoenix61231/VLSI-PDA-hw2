#include "main.h"
#include "move_cell.h"
#include "build_list.h"
#include "check.h"

void Cut(blk *arr, cell *c, int Pmax){
	/*	adjust link */
	/*	Cut	*/
	
	if((c->prev==NULL) && (c->next!=NULL)){
		arr[c->gain + Pmax].head = c->next;
		c->next->prev = NULL;
		c->next = NULL;
	}else if((c->prev!=NULL) && (c->next==NULL)){
		arr[c->gain + Pmax].tail = c->prev;		
		c->prev->next = NULL;
		c->prev = NULL;
	}else if((c->prev!=NULL) && (c->next!=NULL)){
		c->prev->next = c->next;
		c->next->prev = c->prev;
		c->next = NULL;
		c->prev = NULL;
	}else if((c->prev==NULL) && (c->next==NULL)){
		arr[c->gain + Pmax].tail = NULL;
		arr[c->gain + Pmax].head = NULL;
	}
}

void Link(blk *arr, cell *c, int Pmax){

	/*	Paste	*/
	if((arr[c->gain + Pmax].head==NULL))
	{	/* && (arr[c->gain + Pmax].tail==NULL)){*/
		arr[c->gain + Pmax].head = c;
		arr[c->gain + Pmax].tail = c;
	}else{
		if(c->lock){
			arr[c->gain + Pmax].tail->next = c;
			c->prev = arr[c->gain + Pmax].tail;
			arr[c->gain + Pmax].tail = c;
		}else{
			arr[c->gain + Pmax].head->prev = c;
			c->next = arr[c->gain + Pmax].head;
			arr[c->gain + Pmax].head = c;
		}
	}
}

void Update_gains(blk *arr, int Pmax, net **net_arr){
	/*	unlock all cells and count cut	*/
	int cnt = 0;
	cell *curr;

	while(cnt <= 2*Pmax){
		curr = arr[cnt].head;

		while(curr!=NULL){	
			if(curr->lock){		
				Cut(arr, curr, Pmax);

				curr->gain = 0;
				Find_gain(net_arr, curr);
			
				Link(arr, curr, Pmax);
			}
			curr = curr->next;
		}

		cnt++;
	}
}

void Move_cell(blk *a_arr, blk *b_arr, cell *c, cell **cell_arr, net **net_arr, int Pmax){
	/*	move cell from A to B or from B to A	
		and adjust gain					
		and record the step list				*/
	
	/*	move cell & adjust gain	*/	
	int cnt_n = 0, cnt_c = 0;
	net *curr_n = NULL;
	cell *curr_c = NULL;	

	bool critical_old = false, critical_new = false;	

	bool update_cells[CELL_ARRAY_SIZE] = {false};

	if(c == NULL){
		printf("NULL\n");
		exit(1);
	}
	
	while(cnt_n < c->pins){
		curr_n = net_arr[c->nets[cnt_n]];
		
		critical_old = false;
		if(curr_n->A_cnt==1 || curr_n->B_cnt==0 || curr_n->A_cnt==0 || curr_n->B_cnt==1) critical_old = true;

		if(c->partition == PARTITION_A){
			curr_n->A_cnt-=1;
			curr_n->B_cnt+=1;
		}
		else if(c->partition == PARTITION_B){
			curr_n->A_cnt+=1;
			curr_n->B_cnt-=1;
		}
		
		critical_new = false;
		if(curr_n->A_cnt==1 || curr_n->B_cnt==0 || curr_n->A_cnt==0 || curr_n->B_cnt==1) critical_new = true;

		/*	cut state	*/
		if(curr_n->A_cnt!=0 && curr_n->B_cnt!=0){
			curr_n->cutstate = true;
		}
		else{
			curr_n->cutstate = false;
		}

		curr_n->update = true;
		if(!critical_old && !critical_new) curr_n->update = false;
		curr_n->critical = critical_new;		
		
		cnt_n++;
	}	

	cnt_n = 0;
	while(cnt_n < c->pins){
		curr_n = net_arr[c->nets[cnt_n]];
		cnt_c = 0;		
		
		while(cnt_c < curr_n->pins){
			curr_c = cell_arr[curr_n->cells[cnt_c]];
			if(curr_c->num==c->num){
				cnt_c++;
				continue;
			}

			/*if(curr_c->lock){
				cnt_c++;
				continue;
			}*/		
			
			if(!update_cells[curr_c->num]){
				if(curr_c->partition == PARTITION_A){
					Cut(a_arr, curr_c, Pmax);

					curr_c->gain = 0;
					Find_gain(net_arr, curr_c);
				
					Link(a_arr, curr_c, Pmax);
				}else if(curr_c->partition == PARTITION_B){
					Cut(b_arr, curr_c, Pmax);

					curr_c->gain = 0;
					Find_gain(net_arr, curr_c);	
					
					Link(b_arr, curr_c, Pmax);
				}

				update_cells[curr_c->num] = true;
			}

			cnt_c++;
		}
		
		cnt_n++;
	}

	c->lock = true;	
	if(c->partition == PARTITION_A){ 
		Cut(a_arr, c, Pmax);

		c->gain = 0;
		Find_gain(net_arr, c);

		Link(b_arr, c, Pmax);
		c->partition = PARTITION_B;
	}else if(c->partition == PARTITION_B){
		Cut(b_arr, c, Pmax);

		c->gain = 0;
		Find_gain(net_arr, c);
		
		Link(a_arr, c, Pmax);
		c->partition = PARTITION_A;
	}
}