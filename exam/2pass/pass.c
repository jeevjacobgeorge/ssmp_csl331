#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#define size 100
char addr[size],label[size],opcode[size],operand[size],line[5*size];
char tr [200];
FILE* src;
FILE* optab;
FILE* inter;
FILE* symtab;
FILE* out;
void readline1()
{
	fgets(line,sizeof(line),src);
	int n = sscanf(line,"%s\t%s\t%[^\n]",label,opcode,operand);
	if (n == 2)
	{
		label[0] = '\0';
		sscanf(line,"\t%s\t%[^\n]",opcode,operand);
		}
}
void readline2()
{
	fgets(line,sizeof(line),src);
	int n = sscanf(line,"%s\t%s\t%s\t%[^\n]",addr,label,opcode,operand);
	if (!strcasecmp(label,"start"))
	{
		sscanf(line,"\t%s\t%s\t%[^\n]",label,opcode,operand);
		addr[0] = '\0';
	}
	else if (n == 3)
	{
		
		sscanf(line,"%s\t\t%s\t%[^\n]",addr,opcode,operand);
		label[0] = '\0';
	}
}

void write_tr()
{
	int l = strlen(tr);
	if (l < 4)
		return;
	char len_hex [4];
	sprintf(len_hex,"%X",l);
	fprintf(out,"\nT%s.%s.%s",addr,len_hex,tr);
	tr[0]='\0';
}

int main()
{
	int locctr,start_dec;
	char start_add_hex[10];
	char name [10];
	char lh[10];
	src = fopen("source.txt","r");
	inter = fopen("intermediate.txt","w");
	symtab = fopen("symtab.txt","w");
	fclose(symtab);
	readline1();
	if (!strcasecmp(opcode,"start"))
	{
		strcpy(start_add_hex,operand);
		locctr = strtol(operand,NULL,16);
		start_dec = locctr;
		strcpy(name,label);
		fprintf(inter,"\t%s\t%s\t%s\n",label,opcode,operand);
	}
	readline1();
	while (strcasecmp(opcode,"END"))
	{
		sprintf(lh,"%X",locctr);
		fprintf(inter,"%s",lh);
		if (label[0])
		{
			symtab = fopen("symtab.txt","a");
			sprintf(lh,"%X",locctr);
			fprintf(symtab,"%s\t%s\n",label,lh);
			fclose(symtab);
		}
		optab = fopen("optab.txt","r");
		int flag = 0;
		while(!feof(optab))
		{
			char operator[10],code[10];
			fscanf(optab,"%s\t%s\n",operator,code);
			if (!strcasecmp(operator,opcode))
			{
				locctr += 3;
				flag = 1;
				break;
			}
		}
		fclose(optab);
		if (!flag)
		{
			if (!strcasecmp(opcode,"WORD"))
				locctr += 3;
			else if(!strcasecmp(opcode,"RESW"))
				locctr += 3 * strtol(operand,NULL,10);
			else if(!strcasecmp(opcode,"RESB"))
				locctr += strtol(operand,NULL,10);
			else if (!strcasecmp(opcode,"BYTE"))
			{
				if (operand[0] == 'C')
					locctr += (strlen(operand) - 3);
				else if (operand[0] == 'X')
					locctr += (strlen(operand) - 3) / 2;
			}
			else 
			{
				printf("Invalid opcode");
				return 6;
			}
		}	
		fprintf(inter,"\t%s\t%s\t%s\n",label,opcode,operand);
		readline1();
	}
	sprintf(lh,"%X",locctr);
	fprintf(inter,"%s\t%s\t%s\t%s\n",lh,label,opcode,operand);
	fclose(src);
	fclose(inter);
	int prog_length = locctr - start_dec;
	char length_hex[10];
	sprintf(length_hex,"%X",prog_length);
	printf("Program length is %d",prog_length);	
	//START OF PASS 2
	src = fopen("intermediate.txt","r");
	out = fopen("obj.txt","w");
	readline2();
	fprintf(out,"H%s.%s.%s",name,start_add_hex,length_hex);
	readline2();
	while(strcasecmp(opcode,"end"))
	{
		optab = fopen("optab.txt","r");
		int flag = 0;
		while(!feof(optab))
		{
			char operator[10],code[10];
			fscanf(optab,"%s\t%s\n",operator,code);
			if (!strcasecmp(opcode,operator))	
			{
				strcat(tr,code);		//strcat is used to append to a string
				flag = 1;
				break;
			}
		}
		fclose(optab);
		if (flag)		//opcode exists so change the operand to obj code
		{
			char symbol[10],sym_add[10];
			symtab = fopen("symtab.txt","r");
			while (!feof(symtab))
			{
				fscanf(symtab,"%s\t%s",symbol,sym_add);
				if (!strcasecmp(symbol,operand))
				{
					strcat(tr,sym_add);
					break;
				}
			}
			fclose(symtab);
		}
		if (!flag)
		{
			if(!strcasecmp(opcode,"byte"))
			{
				for(int i = 2; i < (strlen(operand) - 1) ;i++)
				{
					if (operand[0] == 'C')
					{
						char ascii[4];
						sprintf(ascii,"%X",(int)operand[i]);
						strcat(tr,ascii);
					}
					else if (operand[0] == 'X')
					{
						char str[2] = {operand[i], '\0'};
						strcat(tr,str);
					}
				}
			}
			else if (!strcasecmp(opcode,"word"))
			{
				strcat(tr,operand);
			}
		}
		write_tr();
		readline2();
	}
	write_tr();
	fclose(src);
	fprintf(out,"\nE%s",operand);
	fclose(out);
}
