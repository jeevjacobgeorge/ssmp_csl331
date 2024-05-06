#include <stdio.h>
#include <string.h>
#define size 30
typedef struct entry
{
	char name[size];
	int type; //1 for folders/directories 0 for files and may contain no of entries
	struct entry* ptr;
}entry;

int search_for_duplicates(entry* arr,int stop,char name[size])
{
	for (int i = 1; i < stop; i++)
	{
		if (!strcmp((arr+i)->name,name))
			return 1;
	}
	return 0;
}

void display(entry* arr)
{
	int in;
	printf("File directory structure is as follows\n\n");
	for (int i = 1; i <= arr->type; i++)
	{
		entry block = *(arr + i);	
		if (block.type == 1)
			printf("%d:[%s]\t",i,block.name);
		else 
			printf("|%s|\t",block.name);
	}
	printf("\n\nEnter -1 for exit/back and corresponding dir indexes to open directory:");
	scanf("%d",&in);
	if (in != -1 &&  (arr+in)->type && in)
	{
		display((arr+in)->ptr);
	}
	else if (in == -1)
		return;
	display(arr);
	
}
void input_single_level_dir(entry*dir)
{
	for(int i = 1; i <= dir->type; i++)
	{
		char str[size];
		printf("Enter file-%d name:",i);
		scanf("%s",str);
		if (!search_for_duplicates(dir,i,str))
		{
			dir[i].type = 0;
			strcpy(dir[i].name,str);
		}
		else 
		{
			printf("File name already exists Try another name\n");
			i--;
		}
	}
}
void input_2_level_dir(entry*dir,entry ufds[][size])
{
	
	for(int i = 1; i <= dir->type; i++)
	{
		char str[size];
		printf("Enter user-%d name:",i);
		scanf("%s",str);
		if (!search_for_duplicates(dir,i,str))
		{
			dir[i].type = 1;
			strcpy(dir[i].name,str);
		}
		else 
		{
			printf("User name already exists Try another name\n");
			i--;
		}
	}
	for(int i = 1; i <= dir->type; i++)
	{
		int nf;
		printf("\nWelcome %s",(dir+i)->name);
		printf("\nEnter no of files:");
		scanf("%d",&nf);
		ufds[i][0].type = nf;
		dir[i].ptr = &ufds[i][0];
		input_single_level_dir(&ufds[i][0]);	
	}	
	
}
int n;
int main()
{
	printf("Welcome User\n");
	printf("Enter no of users to create to create in mfd:");
	scanf("%d",&n);
	entry dir[size];
	entry ufds[n+1][size];
	dir->type = n;
	input_2_level_dir(dir,ufds);
	display(dir);
	
}
