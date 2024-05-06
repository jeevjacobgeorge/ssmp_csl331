//Jeev Jacob George 
//Sequential File allocation 
#include <stdio.h>
#include <math.h>
#define size 500
typedef struct block
{
	int no;
	int file_no;
	int allocated;
	struct block* next;
}block;
block memory[size];
int mem_size,block_size,n;
void initialize_memory()
{
	for( int i = 0; i < n; i++)
	{
		memory[i].no = i;
		memory[i].allocated = 0;
		memory[i].next = NULL;
	}
}
int main()
{
	int nf;
	int file_size;
	printf("Enter memory size: ");
	scanf("%d",&mem_size);
	printf("Enter block size: ");
	scanf("%d",&block_size);
	n = mem_size/block_size;
	printf("Block from 0 to %d\n",n - 1);
	initialize_memory();
	printf("Enter no of files: ");
	scanf("%d",&nf);
	block* start[nf];
	for(int i = 0; i < nf; i++)
	{
		printf("Enter file-%d size:",i+1);
		scanf("%d",&file_size);
		int length= ceil((double)file_size/block_size);
		printf("There are %d blocks to be allocated\n",length);
		printf("Enter indexes of block to which it should be allocated:");
		int in,prev;
		for(int j = 0; j < length;j++)
		{
			scanf("%d",&in);
			if(!memory[in].allocated)
			{	
				memory[in].allocated = 1;
				memory[in].file_no = i+1;
				if(!j)
					start[i] = &memory[in];
				else
					memory[prev].next = &memory[in];
				prev = in;
			}		
			else
			{
				printf("\nBlock-%d already filled Try again\n",in);
				j--;
			}
		}
		memory[in].next = NULL;	
	}
	printf("\nFile blocks were allocated as follows:\n");
	for(int i = 0; i < nf; i++)	
	{
		printf("\nFile No:%d\n",start[i]->file_no);
		for(block* ptr = start[i]; ptr!=NULL; ptr = ptr->next)
			printf("%d->",ptr->no);
		printf("Null\n");
	}
	printf("\n");
	return 0;
}
