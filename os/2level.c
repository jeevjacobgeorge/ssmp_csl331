//Jeev
//2level directory
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 40
typedef struct entry
{
	char name[size];
	int no;
	int type; //1 for folders/directories 0 for files and may contain no of entries
	struct entry* ptr;
	struct entry* next;
}entry;
int in;
void display(entry* pt)
{
	
	entry*p = pt->ptr;
	printf("\nWelcome %s\n",pt->name);
	for(int i = 1; i <= (pt->no); i++)
	{
		if (p->type)
			printf("%d:[%s]\t",i,p->name);
		else 
			printf("|%s|\t",p->name);
		p = p->next;
	}
	printf("\nSelect (-1 for back):");
	scanf("%d",&in);
	if( !in || in > pt->no)
	{
		printf("Enter valid command Try again\n");
		return;
	} 	
}

void display_mfd(entry* dir)
{
	printf("Welcome\n");
	for(int i = 1; i <= dir->no;i++)
		printf("%d:[%s]\t",i,dir[i].name);
	printf("\nSelect user:");
	scanf("%d",&in);
	if( !in || in >dir->no)
	{
		printf("Enter valid command Try again\n");
		display_mfd(dir);
		return;
	} 
	while(in != -1)
		display(&dir[in]);	
	if (in == -1)
		display_mfd(dir);	
}

int search_for_duplicates(entry* arr,int stop,char name[size])
{
	for (int i = 1; i < stop; i++)
	{
		if (!strcmp((arr+i)->name,name))
			return 1;
	}
	return 0;
}

int search_duplicates_ufd(entry*usr,char name[size])
{
	entry*p = usr->ptr;
	while(p)
	{
		if (!strcmp(p->name,name))
			return 1;
		p = p->next;
	}
	return 0;
}

void input_mfd(entry *dir)
{
	
	for(int i = 1; i <= dir->no; i++)
	{
		char str[size];
		printf("\nEnter user-%d name:",i);
		scanf("%s",str);
		if (!search_for_duplicates(dir,i,str))
		{
			dir[i].type = 1;
			dir[i].ptr = NULL;
			dir[i].next = NULL;			
			strcpy(dir[i].name,str);
		}
		else 
		{
			printf("User name already exists Try another name\n");
			i--;
		}
	}
}

void input_ufd(entry*usr)
{
	int n;
	printf("\nEnter no of entries in %s:",usr->name);
	scanf("%d",&n);;
	usr->no = n;
	entry*p = usr;
	printf("Enter entries of %s",usr->name);
	for(int i = 0; i < n; i++)
	{
		int type;
		entry*prev = p;
		p = (entry*)malloc(sizeof(entry));
		p->next = NULL;
		p->type = 0;
		char str [size];
		printf("Enter name:");
		scanf("%s",str);
		if (!search_duplicates_ufd(usr,str))
		{
			strcpy(p->name,str);
		}
		else 
		{
			printf("Name already exists Try another name\n");
			i--;
			continue;
		}
		if(i)
			prev->next = p;
		else
			prev->ptr = p; 
	}
}
int main()
{
	int n;
	printf("Welcome User\n");
	printf("Enter no of users to create");
	scanf("%d",&n);
	entry dir[n];
	dir->no = n; //dir[0].no contains no of users
	input_mfd(dir);
	for(int i = 1; i <= dir->no; i++)
	{
		printf("\nWelcome %s",dir[i].name);
		input_ufd(dir+i);
	}
	display_mfd(dir);	
}
