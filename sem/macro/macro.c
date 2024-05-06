#include <stdio.h>
#include <string.h>
FILE *src, *deftab, *namtab, *out, *argtab;
char str[100];
char label[10], opcode[10], operand[10];
void my_getline(FILE *ptr){
    fgets(str,sizeof(str),ptr);
    int n = sscanf(str,"%s\t%s\t%[^\n]",label,opcode,operand);
    if (n == 2){
        sscanf(str,"\t%s\t%[^\n]",opcode,operand);
        label[0] = '\0';
    }
    else if (n == 1)
    {
        sscanf(str,"\t%[^\n]",opcode);
        label[0] = '\0';
        operand[0] = '\0';
    }
}
void define(){
    namtab = fopen("namtab.txt","a");
    fprintf(namtab,"%s",label);
    deftab = fopen("deftab.txt","a");
    int start = ftell(deftab);
    my_getline(src);
    while(strcasecmp(opcode,"MEND")){
        fprintf(deftab,"\t%s\t%s\n",opcode,operand);
        my_getline(src);
    }
    int end = ftell(deftab);
    fclose(deftab);
    fprintf(namtab," %d %d\n",start,end);
    fclose(namtab);
}

void expand(int start,int end){
    fprintf(out,".%s\t%s\t%s\n",label,opcode,operand);  //macro invocation as comment
    char args[10];
    char * token = strtok(operand, ",");
    while (token != NULL){
        fprintf(argtab,"%s\n",token);
        token = strtok(NULL, ",");
    }
    deftab = fopen("deftab.txt","r");
    fseek(deftab,start,SEEK_SET);
    while(ftell(deftab) < end){
        my_getline(deftab);
        fprintf(out,"\t%s\t%s\n",opcode,operand);
    }
    fclose(deftab);
}
void process_line(){
    namtab = fopen("namtab.txt","r");
    while(!feof(namtab)){
        char def[10];
        int start, end;
        fgets(str,sizeof(str),namtab);
        sscanf(str,"%s %d %d",def,&start,&end);
        if(!strcasecmp(def,opcode)){
            expand(start,end);
            fclose(namtab);
            return;
        }
    }
    fclose(namtab);
    if (!strcasecmp(opcode,"MACRO")){
        define();
    }
    else 
        fprintf(out,"%s\t%s\t%s\n",label,opcode,operand);
}
int main()
{
    src = fopen("source.txt","r");
    deftab = fopen("deftab.txt","w");
    namtab = fopen("namtab.txt","w");
    out = fopen("expanded.txt","w");
    argtab = fopen("argtab.txt","w");
    fclose(namtab);
    fclose(deftab);
    while(!feof(src))
    {
        my_getline(src);
        process_line();
    }
    fclose(out);
    fclose(src);
    fclose(argtab);
    return 0;
}