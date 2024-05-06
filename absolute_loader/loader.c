# include <stdio.h>
# include <string.h>
#include <stdlib.h>
char start[7];
char length[3];
char tr[100];
char pgm_name[7];
char start_pgm[7];
char length_pgm[7];
char type;
int read_tr(FILE*obj);
void read_hr(FILE*obj);
int main()
{
    char str[30];
    FILE* obj= fopen("obj.txt","r");
    FILE* memory = fopen("memory.txt","w");
    //read header record
    read_hr(obj);
    char add[8];
    int locctr;
    while (read_tr(obj))
    {   
        locctr = strtol(start,NULL,16);
        for (int i = 0; i < strlen(tr);i+=2)
        {
            sprintf(add,"%X",locctr);
            fprintf(memory,"[%s]:\t",add);
            fprintf(memory,"%c%c",tr[i],tr[i+1]);
            locctr++;
            fprintf(memory,"\n");
        }
    }
    fclose(memory);

    return 1;
}
int read_tr(FILE*obj)
{
    char ch;
    int i;
    do
    {
        ch = fgetc(obj);
        if (ch == 'E')
            return 0;
    }while (ch != 'T');    
    for (i = 0; i < 6; i++) //starting address
        start[i] = fgetc(obj);
    start[i] = '\0';

    for (i = 0; i < 2; i++)     //length 
        length[i] = fgetc(obj);
    length[i] = '\0'; 
    int l = strtol(length,NULL,16);
    for (i = 0; i < 2 * l; i++)    //object code
    {
        ch = fgetc(obj);
        tr[i] = ch;
    }
    tr[i] = '\0';
    return 1;
}

void read_hr(FILE*obj)
{   
    char ch = fgetc(obj);
    int i;
    for (i = 0; i < 6; i++) //program name
        pgm_name[i] = fgetc(obj);
    pgm_name[i] = '\0';

    for (i = 0; i < 6; i++) //starting address
        start_pgm[i] = fgetc(obj);
    start_pgm[i] = '\0';

    for (i = 0; i < 6; i++) //length
        length_pgm[i] = fgetc(obj);
    length_pgm[i] = '\0';
}