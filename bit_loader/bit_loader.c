#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int prog_add_os, bits[12];
char bit_hex[4], length[3], pgm_name[7], start[7], start_pgm[7], length_pgm[7], tr[100];
void read_hr(FILE*obj);
int read(FILE*obj);
void conv_to_bin(int b);
FILE* memory;
int main()
{
    FILE* obj= fopen("obj.txt","r");
    memory = fopen("memory.txt","w+");
    read_hr(obj);
    prog_add_os = 4096; //assuming it was given by os
    while(read(obj));
    return 0;
}
void read_hr(FILE*obj)
{   char str[50];
    fgets(str,sizeof(str),obj);
    sscanf(str,"H%6s%6s%[^\n]",pgm_name,start_pgm,length_pgm);
}
int read(FILE*obj)
{
    while(!feof(obj))
    {
        char str[200]; 
        char opcode [3];
        char addr[5];
        fgets(str,sizeof(str),obj);
        
        if (str[0] == 'E')
            return 0;

        int n = sscanf(str,"T%6s%2s%3s%[^\n]",start,length,bit_hex,tr);
        int start_dec = strtol(start,NULL,16) + prog_add_os;
        int l = strtol(length,NULL,16);
        int b = strtol(bit_hex,NULL,16);
        int last = start_dec + l;
        conv_to_bin(b);
        int j = 0;
        for (int locctr = start_dec; locctr < last; locctr += 3)
        {
            sscanf(tr,"%2s%4s",opcode,addr);
            addr[4] = '\0';
            opcode[2] = '\0';
            if (bits[j++] == 1)
            {
                int addr_dec = strtol(addr,NULL,16) + prog_add_os;
                sprintf(addr,"%X",addr_dec);
            }
            char lh[8];
            sprintf(lh,"%X",locctr);
            fprintf(memory,"[%s]:\t%s\n",lh,opcode);
            if (locctr + 1 < last)
            {   
                sprintf(lh,"%X",locctr+1);
                fprintf(memory,"[%s]:\t%c%c\n",lh,addr[0],addr[1]);
            }
            if (locctr + 2 < last)
            {
                sprintf(lh,"%X",locctr+2);
                fprintf(memory,"[%s]:\t%c%c\n",lh,addr[2],addr[3]);
            }  
        }
    }
    return 1;
}
void conv_to_bin(int b)
{
    for (int i = 11; i >= 0; i--)
    {
        bits[i] = b % 2;
        b = b / 2;
    }
}

