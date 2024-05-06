//Jeev Jacob George 
//Sequential File allocation 
#include <stdio.h>
#include <math.h>
#define size 500
typedef struct block
{
	int file_no;
	int allocated;
}block;
block memory[size];
int mem_size,block_size,n;
void initialize_memory()
{
	for( int i = 0; i < n; i++)
		memory[i].allocated = 0;
}
int first_fit(int file_no,int length)
{
	for(int i = 0; i <= n-length; i++)
	{
		int count = 0;
		int start = i;
		while((i < n) && !memory[i].allocated && count < length)
		{
			count++;
			i++;
		}
		if (count == length)
		{
			int j;
			for(j = start; j < start + length; j++)
			{
				memory[j].file_no = file_no;
				memory[j].allocated = 1;
			}
			printf("File-%d allocated\nStart block:",file_no);
			printf("%d End block:%d Length:%d\n",start,j - 1,length);
			return start;
	}
}
printf("File cannot be allocated due to insufficient space");
return -1;
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
	int start[nf];
	int length[nf];
	for(int i = 0; i < nf; i++)
	{
		printf("Enter file-%d size:",i+1);
		scanf("%d",&file_size);
		length[i] = ceil((double)file_size/block_size);
		start[i] = first_fit(i+1,length[i]);
		if(start[i] == -1)
		{
			printf("Try again\n");
			i--;
		}		
	}
	
	return 0;
}
