//before executing pass1,do gcc pass2.c -o pass2 
//then do gcc pass1.c and then ./a.out 
// no need to ./pass2 only ./pass1 is required
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>	
char label[20],opcode[20],operands[50];
void readline(FILE* src_file)
{
	label[0] = '\0';
	opcode[0] = '\0';
	operands[0] = '\0';
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
	while (ch != ' ' && ch != '\t' && ch != '\n' && !feof(src_file)) 	//operands
	{
		operands[i++] = ch;
		ch = fgetc(src_file);
		if (ch == 39)
		{
			do
			{	
				operands[i++] = ch;
				ch = fgetc(src_file);
				
			} while (ch != 39 );
		}
	}
	operands[i] = '\0';
	while (ch != '\n')
		ch = fgetc(src_file);	
}
int main()
{
	FILE * src_file,* sym_file,* optab_file,*inter_file;
	char starting_address[50];
	char address_hex[10]; 
	src_file = fopen("source.txt","r");
	sym_file = fopen("symtab.txt","w"); 	//create symtab file
	fclose(sym_file);
	inter_file = fopen("intermediate_file.txt","w");
	if (src_file == NULL)
	{
		perror("Error opening file");
		return 1;
	}
	int locctr;
	readline(src_file);
	if (!strcasecmp(opcode,"START"))
	{
		strcpy(starting_address,operands);
		locctr = strtol(starting_address,NULL,16);
		fprintf(inter_file,"%s\t%s\t%s\t\t%s\n",label,opcode,operands,starting_address);
		readline(src_file);
	}
	else
		locctr = 0;
		
	char str[20];
	while(strcmp(opcode,"END"))
	{
		char code[5];
		int flag = 0;
		if ( label[0] )		//label exists
		{
			
			sym_file = fopen("symtab.txt","r+");
			while(!feof(sym_file))
			{
				str[0] = '\0'; 			//empty string
				fscanf(sym_file,"%s\t%s",str,code);
				if(!strcasecmp(str,label))	//symbol already present in symtab
				{
					perror("duplicate symbol");
					return 1;
				}
			}
			sprintf(str,"%X",locctr);		//convert decimal to hexadecimal
			fprintf(sym_file,"%s\t%s\n",label,str);
			fclose(sym_file);
		}
		optab_file = fopen("optab.txt","r");
		while(!feof(optab_file))
		{	
			fscanf(optab_file,"%s\t%s",str,code);
			if(!strcasecmp(str,opcode))	//opcode found 
			{
				locctr += 3; 
				flag = 1;
				break;
			}
		}
		fclose(optab_file);
		if(!flag)
		{
			if(!strcasecmp(opcode,"WORD"))
			{
				locctr += 3;
			}
			else if(!strcasecmp(opcode,"RESW"))
			{
				locctr += 3 * atoi(operands);
			}
			else if (!strcasecmp(opcode,"RESB"))
			{
				locctr += atoi(operands);
			}
			else if (!strcasecmp(opcode,"BYTE"))
			{
				int j = 0;
				while (operands[j+2] != 39) 
					j++;	
				printf("%d",locctr);				
				if (operands[0] == 'c' || operands[0] == 'C')
					locctr += j;
				else if (operands[0] == 'x' || operands[0] == 'X')
				{
					locctr += (j/2);
				}
			}
			else 
			{
				printf("Invalid operation code %s",opcode);
				return(1);
			}
		}
		sprintf(address_hex,"%X",locctr);
		fprintf(inter_file,"%s\t%s\t%s\t\t%s\n",label,opcode,operands,address_hex);
		readline(src_file);		
	}
	fprintf(inter_file,"\tEND\t%s",operands);
	fclose(src_file);
	fclose(inter_file);
	int prog_length_deci = locctr - (int)strtol(starting_address,NULL,16);	  //decimal
	
	//Convert integer to hex
	char prog_length[6];
	sprintf(prog_length,"%d",prog_length_deci);
			
	char* args[] = {"./pass2",prog_length,NULL};
	execvp("./pass2",args);
	
	perror("execvp failed");
	return 1;
}








