#include "main.h"
#include "init.h"
#include "check.h"

cell *Build_Cell_Array(cell **cell_array, FILE *file, int *c_cnt, int *s_cnt, int *Smax){
	int cell_n = 0, cell_s = 0;
	cell *c = NULL;
	cell *list_head = NULL;
	
	while(!feof(file)){
		fscanf(file, "c%d %d\n", &cell_n, &cell_s);
		/*printf("%d %d\n", cell_n, cell_s);*/
		
		/*create array*/
		cell_array[cell_n] = (cell *)malloc(sizeof(cell));
		c = cell_array[cell_n];
		
		c->num = cell_n;
		c->size = cell_s;
		
		c->pins = 0;
		c->gain = 0;
		c->lock = false;
		c->prev = NULL;
		c->next = NULL;
		c->partition = PARTITION_B;

		/*printf("%d\n", c->size);*/

		/*list_head = Insert_Cell_list(list_head, c);
		printf("insert cell %d\n", c->num);*/

		if(list_head == NULL){
			list_head = c;
		}
		else{
			c->next = list_head;
			list_head->prev = c;

			list_head = c;
		}

		*c_cnt += 1;
		*s_cnt += cell_s;
		if(cell_s > *Smax) *Smax = cell_s;
	}
	return list_head;
}

cell *Sort_list(cell *list, int Smax){
	cell *result, *curr;
	int cnt = 0;	
	
	cell **arr;
	arr = malloc((Smax+1) * sizeof(cell *));

	while(cnt <= Smax){
		arr[cnt] = NULL;
		cnt++;
	}

	while(list!=NULL){
		curr = list;
		list = list->next;

		curr->next = NULL;
		curr->prev = NULL;
		
		if(arr[curr->size]==NULL){
			arr[curr->size] = curr;
		}
		else{
			curr->next = arr[curr->size];
			arr[curr->size]->prev = curr;
			arr[curr->size] = curr;
		}
		/*printf("%d\n", curr->size);*/
	}
	printf("Bucket list\n");

	cnt = 0;
	while(cnt <= Smax){
		if(arr[cnt]!=NULL){
			result = arr[cnt];
			break;
		}
		cnt++;
	}
	printf("First Bucket.\n");

	cnt++;
	curr = result;

	while(cnt <= Smax){
		if(arr[cnt]!=NULL){
			while(curr->next!=NULL){
				curr = curr->next;
			}
			
			curr->next = arr[cnt];
			arr[cnt]->prev = curr;
		}		
		cnt++;	
	}
	printf("Connect all list.\n");

	return result;
}

cell *Insert_Cell_list(cell *head, cell *c){
	cell *curr = NULL;

	if(head == NULL){
		return c;}
	
	curr = head;

	if(curr->size>c->size){
		curr->prev = c;
		c->next = curr;
		return c;
	}
	
	while(curr->next!=NULL){
		curr = curr->next;
		if(curr->size>c->size){
			c->prev = curr->prev;
			c->next = curr;
			(curr->prev)->next = c;
			curr->prev = c;
			return head;
		}
	}
	
	curr->next = c;
	c->prev = curr;
	
	return head;
}

int Build_Net_Array(net **net_array, cell **cell_array, FILE *file, int *p_cnt, int *Pmax){
	int net_n = 0, c_num = 0, n_cnt = 0;
	char tmp[1000];
	char *tok;
	net *n = NULL;
	
	while(!feof(file)){
		fscanf(file, "NET n%d {", &net_n);
		/*printf("%d\n", net_n);*/		
	
		net_array[net_n] = (net *)malloc(sizeof(net));
		n = net_array[net_n];

		n->num = net_n;
		n->pins = 0;
		n->update = true;
		n->critical = false;
		n->cutstate = false;

		n->A_cnt = 0;
		n->B_cnt = 0;

		while(1){
			fscanf(file, "%s", tmp);
			/*printf("%s\n", tmp);*/
			
			/*set nets list in cell*/
			if(!strcmp(tmp, "}")){
				/*if(net_array[net_n]->pins>5000) printf("%d\n", net_n);*/
				break;}
			else{
				tok = strtok(tmp, "c");
				c_num = atoi(tok);
				/*printf("%d\n", c_num);*/
				cell_array[c_num]->nets[cell_array[c_num]->pins] = net_n;
				cell_array[c_num]->pins++;
				
				if(cell_array[c_num]->pins > *Pmax) *Pmax = cell_array[c_num]->pins;
				*p_cnt += 1;
				
				net_array[net_n]->cells[net_array[net_n]->pins] = c_num;
				/*if(net_array[net_n]->pins>9000)	printf("%d\n", net_array[net_n]->pins);*/
				net_array[net_n]->pins++;
			}
		}
		fscanf(file, " ");

		n_cnt++;
		/*if(n_cnt==100000) break;*/
	}
	return n_cnt;
}

int Find_Init_Partition(cell *list, cell **A, cell **B, int S, net **net_arr, int *Cut){
	/*	|area(A)-area(B)| < n/10.
		area(A) is the sum of all cell sizes in A.
		area(B) is the sum of all cell sizes in B.
		n is the sum of all cell sizes in the circuit.	*/

	/*	a < S*(11/20)	*/
	
	int S_a = 0, p_cnt = 0;
	cell *curr;
	
	*A = list;
	curr = list;
	while(1){
		if(S_a+curr->size > S*11/20){
			(curr->prev)->next = NULL;
			curr->prev = NULL;
			*B = curr;
			break;			
		}
		S_a += curr->size;
		curr->partition = PARTITION_A;
		
		p_cnt = 0;
		while(p_cnt < curr->pins){
			net_arr[curr->nets[p_cnt]]->A_cnt += 1;
			p_cnt++;
		}
				
		curr = curr->next;	
	}
	
	while(curr!=NULL){
		p_cnt = 0;
		while(p_cnt < curr->pins){
			net_arr[curr->nets[p_cnt]]->B_cnt += 1;
			
			if(net_arr[curr->nets[p_cnt]]->A_cnt!=0){
				if(!net_arr[curr->nets[p_cnt]]->cutstate){	
					net_arr[curr->nets[p_cnt]]->cutstate = true;
					*Cut += 1;
				}
			}
			
			p_cnt++;
		}
		curr = curr->next;
	}
	return S_a;
}