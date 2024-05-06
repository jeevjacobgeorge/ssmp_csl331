//Jeev Jacob George 
//Hierarchical Directory
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 40
typedef struct entry
{
	char name[size];
	int no;     //contain no of entries
	int type; //1 for folders/directories 0 for files  
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
	printf("\nSelect :");
	scanf("%d",&in);
	if(in == -1)
	{
		return;
	}
	else if( !in || in > pt->no)
	{
		printf("Enter valid command Try again\n");
		display(pt);
		return;
	}
	p = pt->ptr;
	for(int	i = 1; i < in;i++)
		p = p->next;
	if(p->type)
		display(p);
	else 
	{
		printf("\nNot a directory Try again\n");
		display(pt);
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
	if(in == -1)
	{
		display_mfd(dir);
	}
		
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
		printf("\nEnter 0 for file and 1 for directory:");
		scanf("%d",&type);
		if (!(type == 1 || type == 0))
		{
			printf("Enter valid command\n");
			i--;
			continue;
		}
		p->type = type;
		char str [size];
		printf("Enter name:");
		scanf("%s",str);
		while(search_duplicates_ufd(usr,str))	
		{
			printf("\nName already exists Try again");
			scanf("%s",str);
		}
		strcpy(p->name,str);
		if(i)
			prev->next = p;
		else
			prev->ptr = p; 
		p->next = NULL;
	}
	p = usr->ptr;
	for(int i = 0; i < n && p; i++)
	{
		if(p->type)
		{
			input_ufd(p);
		}
		p = p->next;
	}
}

void input(entry *dir)
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
	for(int i = 1; i <= dir->no; i++)
	{
		printf("\nWelcome %s",dir[i].name);
		input_ufd(dir+i);
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
	input(dir);
	display_mfd(dir);	
}
//2 jeev jeni 2 0 file 1 folder 2 0 hi 0 hello 1 1 tutu_fol 1 text
//2 jeni jeev 2 1 f1 0 file 1 1 foldx 1 0 filex 2 0 fil1 0 file2
