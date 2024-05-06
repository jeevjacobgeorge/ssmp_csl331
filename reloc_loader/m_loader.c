#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char start[7], length[3], tr[100], pgm_name[7], start_pgm[7], length_pgm[7];
char type;
void read_hr(FILE*obj);
int read(FILE*obj);
int read(FILE*obj);
FILE* memory;

int main()
{
    char str[30];
    char add[7];
    FILE* obj= fopen("obj.txt","r");
    memory = fopen("memory.txt","w+");
    //read header record
    read_hr(obj);
    while (read(obj))
    {
        int locctr = strtol(start,NULL,16);
        for (int i = 0; i < strlen(tr);i+=2)
        {
            sprintf(add,"%X",locctr);
            fprintf(memory,"[%s]:\t",add);
            fprintf(memory,"%c%c\n",tr[i],tr[i+1]);
            locctr++;
        }
    }
    return 0;
}

int read(FILE*obj)
{
    int i;
    char ch, add[7], str[100];
    while(!feof(obj))
    {
        ch  = fgetc(obj);
        if (ch == 'E')
            return 0;
        if (ch == 'T' || ch == 'M')
            break;
    }
    if (ch == 'T')
    {
        fgets(str,sizeof(str),obj);
        sscanf(str,"%6s%2s%[^\n]",start,length,tr);
    }
    else if (ch == 'M')
    {
        char mod[10];
        fgets(str,sizeof(str),obj);
        sscanf(str,"%6s%2s%[^\n]",add,length,mod);
        
	int add_int = strtol(add,NULL,16);
	char key[4];
	char code[3];
	fseek(memory, 0, SEEK_SET);
	while(fscanf(memory,"[%4s]:\t%2s\n",key,code) == 2)
	{
		if (add_int == strtol(key,NULL,16))	//found
		{
			fseek(memory, -11, SEEK_CUR);
			for(int i= 0; i < strtol(length,NULL,16); i+=2)
			{
                char hexString[7]; 
                sprintf(hexString, "%X", add_int++);
				fprintf(memory,"[%s]:\t",hexString);
                fprintf(memory,"%c%c\n",mod[i],mod[i+1]);
			}
            fseek(memory, 0, SEEK_END);
			break;
		}
	}
	tr[0]='\0';
    }
    return 1;
}

void read_hr(FILE*obj)
{   char str[50];
    fgets(str,sizeof(str),obj);
    sscanf(str,"H%6s%6s%[^\n]",pgm_name,start_pgm,length_pgm);
}
