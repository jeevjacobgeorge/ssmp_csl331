#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>	
char label[20],opcode[20],operand[50];
FILE * src_file,* sym_file,* optab_file,*obj_file;
typedef struct node
{
	int add;
	struct node* next;
}node;
char str_tr[50];
node* list_arr[100];

void readline(FILE* src_file)
{
	label[0] = '\0';
	opcode[0] = '\0';      
	operand[0] = '\0';
	char ch;
	int i = 0;	
	ch = fgetc(src_file);
	if(ch != ' ' && ch != '\t') 	//label exists
	{
		while (ch != ' ' && ch != '\t')
		{
			label[i++] = ch;
			ch = fgetc(src_file);
		}
		label[i] = '\0';
	}
	while (ch == ' ' || ch == '\t')
		ch = fgetc(src_file);
		
	i = 0;
	while (ch != ' ' && ch != '\t' && ch != '\n')	//opcode
	{
		opcode[i++] = ch;
		ch = fgetc(src_file);
		if(ch == -1)
		{
			opcode[i] = '\0';
			return;
		}
	}
	opcode[i] = '\0';
	while (ch == ' ' || ch == '\t')
		ch = fgetc(src_file);
	i = 0;
	while (ch != ' ' && ch != '\t' && ch != '\n' && ch) 	//operand
	{
		operand[i++] = ch;
		ch = fgetc(src_file);
		if (ch == 39)
		{
			do
			{	
				operand[i++] = ch;
				ch = fgetc(src_file);
				
			} while (ch != 39 );
		}
	}
	operand[i] = '\0';
	while (ch != '\n')
		ch = fgetc(src_file);	
}

void write_tr(int start_add_tr,FILE*fptr)
{
	char str[100];
	int len = strlen(str_tr) / 2;
	snprintf(str,sizeof(str),"\nT%X.%X.%s",start_add_tr,len,str_tr);
	fprintf(fptr,"%s",str);
	str_tr[0] = '\0';
}
void pad(char str[])
{
	int n = strlen(str);
	while (n < 6)
	{
		strcat(str_tr,"0");
		n++;
	}
}
void pad_print(char str[])
{
	int n = strlen(str);
	while (n < 6)
	{
		fputc('0',obj_file);
		n++;
	}
	fprintf(obj_file,"%s",str);
}
int main()
 {
	char start[10]; 
	char prog_name[10];
	int start_add_tr;
	int locctr,start_add_dec;
	char code[20];
	char sym[20];
	int add;
 	src_file = fopen("source.txt","r");   
 	sym_file = fopen("symtab.txt","w"); 	//create symtab file
	fclose(sym_file);
	obj_file = fopen("object.txt","w");   
	fprintf(obj_file,"                             ");			//space for header record
	if (src_file == NULL)
	{
		perror("Error opening file");
		return 1;
	}	
 	readline(src_file);
 	if (!strcasecmp(opcode,"START"))
 	{
		strcpy(prog_name,label);
		strcpy(start,operand);
 		locctr = strtol(operand,NULL,16);
		start_add_dec = locctr;
		start_add_tr = locctr;
 		readline(src_file);
 	}
 	else 
 		locctr = 0;
	while(strcmp(opcode,"END"))
	{
		int flag = 0;
		if ( label[0] )		//label exists
		{
			sym_file = fopen("symtab.txt","r+");
			int k = 0;
			while(!feof(sym_file))
			{
				fscanf(sym_file,"%s\t%d\n",sym,&add);
				if(!strcasecmp(sym,label))	//symbol found in symtab
				{
					if (add == -1)	//symbol address is null (forward ref)
					{
						write_tr(start_add_tr,obj_file);				//write old text record to file
						start_add_tr = locctr;
						fseek(sym_file,-6,SEEK_CUR);
						fprintf(sym_file,"%d\n",locctr);
						node *ptr = list_arr[k];
						while (ptr)			//for each address in linked list create a New text record
						{
							sprintf(str_tr,"%X", locctr);				//empty text record string	
							write_tr(ptr->add,obj_file);
							ptr=ptr->next;
						}
					}
					flag = 1;
					break;							
				}
				else		//symbol not found for time being but may be found
					k++;
			}
			if (!flag)		//label not present in symtab
				fprintf(sym_file,"%s\t%d\n",label,locctr);		
			fclose(sym_file);
		}		
		flag = 0; 	//flag set to 1 if opcode is present in optab
		optab_file = fopen("optab.txt","r");
		char str[20];
		while(!feof(optab_file))
		{	
			fscanf(optab_file,"%s\t%s\n",str,code);
			if(!strcasecmp(str,opcode))	//opcode found 
			{
				locctr += 1;
				strcat(str_tr,code);
				int k = 0;			//k is the serial no of symbol used to access the linked list
				sym_file = fopen("symtab.txt","r+");
				int flag_sym = 0;
				while(!feof(sym_file))
				{
					fscanf(sym_file,"%s\t%d\n",sym,&add);
					if(!strcasecmp(sym,operand))	//operand found in symtab
					{	
						if (add != -1)	
						{	
							char hexString[20];
							sprintf(hexString, "%X", add);
							strcat(str_tr,hexString);
						}
						else
						{
							node *new = (node*) malloc(sizeof(node));
							new->add = locctr;
							new->next = NULL;
							node *ptr = list_arr[k];
							while (ptr->next)	ptr = ptr->next;	//go to end of linked list
							ptr->next = new;
						}
						flag_sym = 1;
						break;
					}
					if (sym[0])
						k++;
				}
				if (!flag_sym)		//operand not present in symtab forward ref
				{
					fseek(sym_file,0,SEEK_END);
					fprintf(sym_file,"%s\t%d   \n",operand,-1);
					strcat(str_tr,"0000");
					node *new = (node*) malloc(sizeof(node));
					new->add = locctr;
					new->next = NULL;
					if (!list_arr[k])		//empty linked list
						list_arr[k] = new;
					else		//at least one element
					{
						node *ptr = list_arr[k];
						while (ptr->next)	ptr = ptr->next;	//go to end of linked list
						ptr->next = new;
					}
					
				}
				fclose(sym_file);
				locctr += 2	;
				flag = 1;
				break;
			}	
		}
		fclose(optab_file);
		if (!flag)		//opcode not found
		{
			if(!strcasecmp(opcode,"WORD"))
			{
				char hex[20];
				sprintf(hex,"%X",atoi(operand));
				pad(operand);
				strcat(str_tr,hex);
				locctr += 3;
			}
			else if(!strcasecmp(opcode,"RESW"))
			{
				locctr += 3 * atoi(operand);
			}
			else if (!strcasecmp(opcode,"RESB"))
			{
				locctr += atoi(operand);
			}
			else if (!strcasecmp(opcode,"BYTE"))
			{	
				int j;
				if (operand[0] == 'X' || operand[0] == 'x')
				{
					int n = strlen(str_tr);
					for(j = 0; operand[j+2]!= 39; j++)
					{
						str_tr[n+j]=operand[j+2];
					}
					str_tr[n+j] = '\0';
					locctr += (j/2);

				}
				else if (operand[0] == 'c' || operand[0] == 'C')
				{
					for(j = 0; operand[j+2]!= 39; j++)
					{
						int c = operand[j+2];
						char h[10];
						sprintf(h,"%X",c);
						strcat(str_tr,h);	
					}
					locctr += j;
				}
			}			
		}
		if (strlen(str_tr) > 20  )
		{
			write_tr(start_add_tr,obj_file);
			start_add_tr = locctr;
		}
		readline(src_file);			
	}
	write_tr(start_add_tr,obj_file);
	fprintf(obj_file,"\nE");
	pad_print(operand);
 	fseek(obj_file, 0, SEEK_SET);
	int length = locctr - start_add_dec;
	char length_hex [8];
	snprintf(length_hex,sizeof(length_hex),"%X",length);
	fputc('H',obj_file);
	pad_print(prog_name);
	fputc('.',obj_file);
	pad_print(start);
	fputc('.',obj_file);
	pad_print(length_hex);
	fclose(obj_file);
	fclose(src_file);
 }
