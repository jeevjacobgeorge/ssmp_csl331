//Jeev Jacob George
//Indexed file allocation
#include <stdio.h>
#include <math.h>
#define size 500
int mem_size,block_size,n;
typedef struct block
{
	int file_no;
	int allocated;
}block;
typedef struct index_block
{
	int length;
	int arr[size];
}index_block;
block memory[size];
void initialize_memory()
{
	for( int i = 0; i < n; i++)
		memory[i].allocated = 0;
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
	printf("Memory divided into Blocks from 0 to %d\n",n - 1);
	initialize_memory();
	printf("Enter no of files: ");
	scanf("%d",&nf);
	index_block files[nf];
	for(int i = 0; i < nf; i++)
	{
		int k = 0;
		printf("\nEnter file-%d size:",i+1);
		scanf("%d",&file_size);
		int length = ceil((double)file_size/block_size);
		printf("There are %d blocks to be allocated\n",length);
		printf("Enter indexes of block to which it should be allocated:");
		files[i].length = length;
		for(int j = 0; j < length;j++)
		{
			int in;
			scanf("%d",&in);
			if(!memory[in].allocated)
			{
				memory[in].allocated = 1;
				memory[in].file_no = i;
				files[i].arr[k++] = in;
			}
			else
			{
				printf("\nBlock-%d already filled Try again\n",in);
				j--;
			}
		} 
		printf("File allocation successfull\n");		
	}
	printf("\nFile blocks were allocated as follows:\n");
	for(int i = 0; i < nf; i++)	
	{
		printf("\nFile No:%d\nIndex Table\n",i+1);
		printf("----------------\n");
		for(int j = 0; j < files[i].length; j++)
			printf("|\t%d\t|\n",files[i].arr[j]);
		printf("----------------\n");
	}
	return 0;
}
