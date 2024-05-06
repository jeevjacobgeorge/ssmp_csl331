#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *src, *mem;
char str[100];
char start[10],length[4],mask[5];
char tr[50];
int bits[12];
void convert_mask();
void main()
{
	src = fopen("obj.txt","r");
	mem = fopen("memory.txt","w");
	int locctr;
	char lh[8];
	fgets(str,sizeof(str),src);
	int progadr = 4096;
	while (str[0] != 'E')
	{
		fgets(str,sizeof(str),src);
		while(str[0] == 'T')
		{
			sscanf(str,"T%6s%2s%3s%[^\n]",start,length,mask,tr);
			int start_dec =strtol(start,NULL,16)+ progadr;
			int length_dec = strtol(length,NULL,16);
			int last = start_dec + length_dec;
			char opcode [3];
			char operand[5];
			convert_mask();
			int j = 0;
			for(locctr = start_dec ; locctr < last; locctr+=3){
				sscanf(tr,"%2s%4s",opcode,operand);
				if (bits[j] == 1){
					int op = strtol(operand,NULL,16);
					op = op + progadr;
					sprintf(operand,"%X",op);
				}
				sprintf(lh,"%X",locctr);
				fprintf(mem,"[%s]:%s\n",lh,opcode);
				sprintf(lh,"%X",locctr+1);
				fprintf(mem,"[%s]:%c%c\n",lh,operand[0],operand[1]);
				sprintf(lh,"%X",locctr+2);
				fprintf(mem,"[%s]:%c%c\n",lh,operand[2],operand[3]);
			}
			j++;
			fgets(str,sizeof(str),src);
		}
	}
		
}
void convert_mask(){
	int value = strtol(mask,NULL,16);
	for ( int i = 11; i >= 0; i--)
	{
		bits[i] = value % 2;
		value = value / 2;
	}
}