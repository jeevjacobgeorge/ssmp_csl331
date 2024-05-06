#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define size 50
char label[size],opcode[size],operand[size],line[5*size];
char tr [200],start_tr[10];
FILE* src;
FILE* optab;
FILE* inter;
FILE* symtab;
FILE* out;
int locctr;
void readline()
{
	fgets(line,sizeof(line),src);
	int n = sscanf(line,"%s\t%s\t%[^\n]",label,opcode,operand);
	if (n == 2)
	{
		label[0] = '\0';
		sscanf(line,"\t%s\t%[^\n]",opcode,operand);
	}	
}
void write_tr()
{
	int l = strlen(tr)/2;
	if (l < 3)
		return;
	char len_hex[7];
	sprintf(len_hex,"%X",l);
	fprintf(out,"T%s.%s.%s\n",start_tr,len_hex,tr);
	sprintf(start_tr,"%X",locctr);
	tr[0] = '\0';	//empty textrecord;
}
void main()
{
	char name[10],start_add_hex[10];
	src = fopen("source.txt","r");
	out = fopen("object.txt","w");
	symtab = fopen("symtab.txt","w");
	fclose(symtab);
	fprintf(out,"                               \n"); //space for header record
	readline();
	strcpy(start_add_hex,operand);
	locctr = strtol(operand,NULL,16);
	strcpy(start_tr,start_add_hex);
	strcpy(name,label);
	readline();
	while(strcasecmp(opcode,"end"))
	{
		printf("%d \n",locctr);
		if (label[0])		//if label exists
		{
			char lh[7];
			symtab = fopen("symtab.txt","a");
			sprintf(lh,"%X",locctr);
			fprintf(symtab,"%s\t%s\n",label,lh);
			fclose(symtab);
		}
		optab = fopen("optab.txt","r");
		int flag = 0;
		while(!feof(optab))		//searching for opcode in optab
		{
			char operator[10],code[4];
			fscanf(optab,"%s\t%s\n",operator,code);
			if (!strcasecmp(operator,opcode))		//opcode found in optab
			{
				flag = 1;
				strcat(tr,code);
				locctr += 3;
				break;		//found so stop searching
			}
		}
		fclose(optab);
		if (flag) //opcode is like STL CLOOP so now going to change CLOOP TO objcode
		{
			symtab = fopen("symtab.txt","r");
			char symbol[20],sym_add[10];
			while (!feof(symtab))
			{
				fscanf(symtab,"%s\t%s\n",symbol,sym_add);
				if (!strcasecmp(symbol,operand))
				{
					strcat(tr,sym_add);
					break;
				}
			}
		}
		if(!flag) //opcode is one of RESW,RESB,BYTE,WORD
		{
			if (!strcasecmp(opcode,"resw"))
				locctr += 3 * atoi(operand);	
			else if (!strcasecmp(opcode,"resb"))
				locctr+=atoi(operand);
			else if (!strcasecmp(opcode,"word"))
			{
				locctr += 3;
				strcat(tr,operand);
			}
			else if (!strcasecmp(opcode,"byte"))
			{
				if (operand[0] == 'C')
				{
					for( int i = 2; i < (strlen(operand) - 1); i++)	//we need to convert EOF character of C'EOF' 
					{						//so removing C''
						char ascii[4];
						sprintf(ascii,"%X",(int)operand[i]); //converts characters to corresponding ascii hex code
						strcat(tr,ascii);	
					}
					locctr += (strlen(operand) - 3);
				}
				else if (operand[0] == 'X')
				{
					for( int i = 2; i < (strlen(operand) - 1); i++)	//we need to convert 69 of X'69' 
					{						//so removing X''
						char str[2] = {operand[i], '\0'}; 	////convert char to string to use strcat
						strcat(tr,str);
					}
					locctr += (strlen(operand) - 3)/2;
				}
			}	
		} 
		write_tr();
		readline();
	}
	write_tr();
	fprintf(out,"E%s",operand);
	int length = locctr - strtol(start_add_hex,NULL,16);
	char len_hex[10];
	sprintf(len_hex,"%X",length);
	fseek(out,0,SEEK_SET);
	fprintf(out,"H%s.%s.%s",name,start_add_hex,len_hex);
	fclose(src);
	fclose(out);	
}
