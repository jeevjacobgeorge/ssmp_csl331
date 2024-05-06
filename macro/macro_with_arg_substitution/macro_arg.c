#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define size 10
char opcode[size], label[size], operand[size*5], line[size*10], str[size*10];
char argtab[5][size];
FILE* src, * out;

int search_arg(int n)
{
    for (int i = 0; i < n; i++)
    {
        if (!strcasecmp(argtab[i], operand))
            return i+1;
    }
    return 0;
}

void my_getline(FILE* ptr){
    fgets(line, sizeof(line), ptr);
    int n = sscanf(line, "%s %s %[^\n]",label, opcode, operand);
    if (n == 2){
        sscanf(line,"%s %[^\n]",opcode,operand);
        label[0] = '\0';
    }
    else if (n == 1){
        sscanf(line,"\t%[^\n]",opcode);
        label[0] = '\0';
        operand[0] = '\0';
    }
}
void define(){  
    FILE* namtab = fopen("namtab.txt", "a");
    fprintf(namtab, "%s",label);    //label contains macro name
    int i = 0;                      //index for arguments
    char * token = strtok(operand, ",");
    while (token != NULL && i < 5){
        strcpy(argtab[i++],token);
        token = strtok(NULL, ",");
    }
    FILE * deftab = fopen("deftab.txt", "a");
    int start = ftell(deftab);
    my_getline(src);
    while (strcasecmp(opcode, "ENDM"))
    {   
        if (search_arg(i) != 0){
            sprintf(operand,"&%d",search_arg(i));
        }

        fprintf(deftab, "%s %s\n", opcode, operand);
        my_getline(src);
    }
    int end = ftell(deftab);
    fclose(deftab);
    fprintf(namtab, " %d %d\n",start,end);
    fclose(namtab);
}
void expand()
{
    char name[size];
    int start,end;
    sscanf(str, "%s %d %d", name, &start, &end);
    fprintf(out, ".%s\t%s\t%s\n", label, opcode, operand);
    int i = 0;                      //index for arguments
    char * token = strtok(operand, ",");
    while (token != NULL){
        strcpy(argtab[i++],token);
        token = strtok(NULL, ",");
    }
    FILE* deftab = fopen("deftab.txt","r");
    fseek(deftab,start,SEEK_SET);
    while(ftell(deftab) < end)
    {
        my_getline(deftab);
        if (operand[0] = '&'){
            operand[0] = '0';
            strcpy(operand,argtab[strtol(operand,NULL,10)-1]);
        }
        fprintf(out, "\t%s\t%s\n", opcode, operand);
    }
    fclose(deftab);
}
void processline()
{
    char name[size];
    int start,end;
    FILE * namtab = fopen("namtab.txt", "r");
    while (!feof(namtab))
    {
        fgets(str, sizeof(line), namtab);
        sscanf(str, "%s %d %d", name, &start, &end);
        if (!strcasecmp(name,opcode)) //opcode found in namtab
        {
            expand();
            fclose(namtab);
            return;
        }
    }
    fclose(namtab);
    if (!strcasecmp(opcode,"MACRO"))
        define();
    else 
        fprintf(out, "%s\t%s\t%s\n", label, opcode, operand);
    
}
int main()
{
    src = fopen("source.txt", "r");
    out = fopen("expanded_source.txt", "w");
    FILE* namtab = fopen("namtab.txt", "w");
    fclose(namtab);
    FILE* deftab = fopen("deftab.txt", "w");
    fclose(deftab);
    while(!feof(src))
    {
        my_getline(src);
        processline(); 
    }
    return 0;
}