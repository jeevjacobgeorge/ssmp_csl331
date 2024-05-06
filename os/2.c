#include <stdio.h>
#include <string.h>
#define size 30
typedef struct entry
{
	char name[size];
	int type; //1 for folders/directories 0 for files
	struct entry* ptr;
}entry;
int search_for_duplicates(entry* arr,int stop,char name[size])
{
	for (int i = 0; i < stop; i++)
	{
		if (!strcmp((arr+i)->name,name))
			return 1;
	}
	return 0;
}

void display(entry* arr,int n)
{
	int in;
	printf("File directory structure is as follows\n\n");
	for (int i = 0; i < n; i++)
	{
		entry block = arr[i];	
		if (block.type)
			printf("|%d[%s]|\t",i,block.name);
		else 
			printf("|%s|\t",block.name);
	}

}

void input_single_level_dir(entry*dir,int n)
{
	for(int i = 0; i < n; i++)
	{
		char str[size];
		printf("Enter file-%d name:",i+1);
		scanf("%s",str);
		if (!search_for_duplicates(dir,i,str))
		{
			dir[i].type = 0;
			strcpy(dir[i].name,str);
		}
		else 
		{
			printf("File name already exists Try another name");
			i--;
		}
	}
}

int main()
{
	int n;
	printf("Welcome User\n");
	printf("Enter no of files: to create");
	scanf("%d",&n);
	entry dir[size];
	input_single_level_dir(dir,n);
	display(dir,n);
	
}
