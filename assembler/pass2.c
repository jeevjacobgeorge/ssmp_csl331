#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char opcode[50],operands[50],label[50],address[50];
int program_length;
char start_add[50];
int prev;
void pad(FILE*ptr,char str[])
{
	int n = strlen(str);
	while (n < 6)
	{
		fputc('0',ptr);
		n++;
	}
}
void readline(FILE* src_file)
{
	label[0] = '\0';
	opcode[0] = '\0';
	operands[0] = '\0';
	address[0] = '\0';
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
	while ((!feof(src_file)) && ch != ' ' && ch != '\t' && ch != '\n') 	//operands
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
	if(!strcasecmp(opcode,"END"))
		return;
	while (ch == ' ' || ch == '\t')
		ch = fgetc(src_file);
	i = 0;
	while (ch != ' ' && ch != '\t' && ch != '\n')	//address
	{
		address[i++] = ch;
		ch = fgetc(src_file);
	}
	address[i] = '\0';
	while (ch != '\n')
		ch = fgetc(src_file);	
}

int init_txt_rec(FILE*obj_file)
{

	
	int left = strtol(address,NULL,16) - prev;
	char left_hex[20];
	sprintf(left_hex,"%X",left);
	fprintf(obj_file,"\nT");
	pad(obj_file,address);
	fprintf(obj_file,"%s",address);
	if (strlen(left_hex))
		fputc('0',obj_file);
	fprintf(obj_file,"%s",left_hex);
	return left;


}


int main(int argc,char* argv[])
{
	program_length = atoi(argv[1]);	//int varialble
	FILE * src_file,* sym_file,* optab_file,*obj_file;
	src_file = fopen("intermediate_file.txt","r");
	sym_file = fopen("symtab.txt","r");
	obj_file = fopen("obj.txt","w");
	readline(src_file);
	if (!strcasecmp(opcode,"START"))	//Opcode == START
	{
	//write header record to object program
		fputc('H',obj_file);
		pad(obj_file,label);
		fprintf(obj_file,"%s",label);		//program name
		strcpy(start_add,operands);		//start_add is hex
		pad(obj_file,start_add);
		fprintf(obj_file,"%s",start_add);	//starting_address
		char prog_length_hex[6];
		sprintf(prog_length_hex,"%X",program_length);
		pad(obj_file,prog_length_hex);
		fprintf(obj_file,"%s",prog_length_hex);		//program length
		prev = strtol(address,NULL,16);			
		readline(src_file);
	}
	//initialise first text record	
	int count = init_txt_rec(obj_file);
	while(strcasecmp(opcode,"END"))
	{
		char str[20];
		char code[10];
		char sym_add[10];
		optab_file = fopen("optab.txt","r");
		while(!feof(optab_file))
		{
			fscanf(optab_file,"%s\t%s",str,code);
			if(!strcasecmp(str,opcode))		//opcode found in optab
			{
		
				sym_file = fopen("symtab.txt","r+");
				while(!feof(sym_file))
				{
					fscanf(sym_file,"%s\t%s",str,sym_add);
					if(!strcasecmp(str,operands))	//operand is a label
					{
						if (count < 3)
							count = init_txt_rec(obj_file); 	//create new text record
							
						fprintf(obj_file,"%s%s",code,sym_add);
						count -= 3;
						break; //label found break
					}
				}
				if(feof(sym_file))
				{
					printf("Undefined symbol %s",operands);
					return 1;
				}
				break; 	//opcode found break
			}
		}
		if (!strcasecmp(opcode,"WORD"))
		{
			if (count < 3)
				count = init_txt_rec(obj_file); 	//create new text record
			char hex[5];
			sprintf(hex,"%X",atoi(operands));
			pad(obj_file,operands);
			fprintf(obj_file,"%s",hex);
			count -= 3;
		}
		else if (!strcasecmp(opcode,"BYTE"))
		{
			if (!count)
				count = init_txt_rec(obj_file);
			if (operands[0] == 'X' || operands[0] == 'x')
			{
				for(int j = 2; operands[j]!= 39; j++)
				{
					fputc(operands[j],obj_file);
					count--;
				}
			}
			else if (operands[0] == 'c' || operands[0] == 'C')
			{
				for(int j = 2; operands[j]!= 39; j++)
				{
					if (!count)
						count = init_txt_rec(obj_file);
					int c = operands[j];
					char h[10];
					sprintf(h,"%X",c);
					fprintf(obj_file,"%s",h);
					count--;
				}
			}
				
		}
		prev = strtol(address,NULL,16);
		readline(src_file);
	}
fprintf(obj_file,"\nE");
pad(obj_file,operands);
fprintf(obj_file,"%s",operands);
fclose(sym_file);
fclose(src_file);
fclose(obj_file);	 
}
