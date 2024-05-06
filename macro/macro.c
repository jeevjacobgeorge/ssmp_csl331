#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define size 100
char opcode[size], label[size], operand[size], line[size], str[size];
FILE* src, * out;

void my_getline(FILE* ptr)
{
    fgets(line, sizeof(line), ptr);
    int n = sscanf(line, "%s %s %[^\n]",label, opcode, operand);
    if (n == 2)
    {
        sscanf(line,"%s %[^\n]",opcode,operand);
        label[0] = '\0';
    }
    else if (n == 1)
    {
        sscanf(line,"\t%[^\n]",opcode);
        label[0] = '\0';
        operand[0] = '\0';
    }
}

void define(char macro_name[])
{  
    FILE* namtab = fopen("namtab.txt", "a");
    fprintf(namtab, "%s",macro_name);
    FILE * deftab = fopen("deftab.txt", "a");
    int start = ftell(deftab);
    my_getline(src);
    while (strcasecmp(opcode, "MEND"))
    {
        fprintf(deftab, "\t%s\t%s\n",opcode, operand);
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
    FILE* deftab = fopen("deftab.txt","r");
    fseek(deftab,start,SEEK_SET);
    while(ftell(deftab) < end)
    {
        my_getline(deftab);
        fprintf(out, "\t%s\t%s\n",opcode, operand);
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
        fgets(str, sizeof(str), namtab);
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
        define(label);
    else 
        fprintf(out, "%s\t%s\t%s\n", label, opcode, operand);
    
}
int main()
{
    src = fopen("source.txt", "r");
    out = fopen("expanded_source.txt", "w");
    while(!feof(src))
    {
        my_getline(src);
        processline(); 
    }
    return 0;
}