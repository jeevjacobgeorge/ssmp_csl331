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
	int length;
	int arr[size];
}block;

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
	int indexes[nf]; //contains block no of index blocks
	for(int i = 0; i < nf; i++)
	{
		int k = 0;
		printf("\nEnter file-%d size:",i+1);
		scanf("%d",&file_size);
		int length = ceil((double)file_size/block_size);
		printf("There are %d blocks and a index block to be allocated\n",length);
		//printf("Enter indexes of block to which it should be allocated starting with index block:");
		for(int j = 0; j < length + 1;j++)
		{
			int in;
			printf("\nEnter where blocks should be allocated:");
			scanf("%d",&in);
			if(!memory[in].allocated)
			{
				memory[in].allocated = 1;
				memory[in].file_no = i;
			}
			else
			{
				printf("\nBlock-%d already filled Try again\n",in);
				j--;
				continue;
			}
			if(!j)
			{
				indexes[i] = in;
				memory[indexes[i]].length = length;
			}
			else 
			{
				memory[indexes[i]].arr[k++] = in;
			}
		} 
		printf("File allocation successfull\n");		
	}
	printf("\nFile blocks were allocated as follows:\n");
	for(int i = 0; i < nf; i++)	
	{
		printf("\nFile No:%d\nBlock%d\n",i+1,indexes[i]);
		printf("----------------\n");
		for(int j = 0; j < memory[indexes[i]].length; j++)
			printf("|\t%d\t|\n",memory[indexes[i]].arr[j]);
		printf("----------------\n");
	}
	return 0;
}
