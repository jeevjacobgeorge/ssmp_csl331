#include <stdio.h>
#include <string.h>
#define size 100
char opcode[size], label[size], operand[size], line[size], str[size];
FILE* src, * out, * deftab;
int expanding;
void my_getline();
void define();
void expand();

int main(){

}
void my_getline(){
    FILE * ptr = src;
    if (expanding)
        ptr = deftab;
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

void processline(){
    char label[size];
    int start,end;
    FILE * namtab = fopen("namtab.txt","r");
    while (!feof(namtab)){
        fscanf(namtab,"%s %d %d\n",label,&start,&end);
        if (!strcasecmp(label,opcode)){
            expand();
            return;
        }
    }
    if (!strcasecmp(opcode,"MACRO")){
        define();
    }
    else{
        fprintf(out, "%s\t%s\t%s\n", label, opcode, operand);
    }
}

void define(){
    FILE* namtab = fopen("namtab.txt","a");
    FILE* deftab = fopen("deftab.txt","a");

}

void expand(){
    char label[size];
    int start,end;
    FILE* namtab = fopen("namtab.txt","r");
    FILE* deftab = fopen("deftab.txt","r");
    while(!feof(namtab)){
        fscanf(namtab,"%s %d %d\n",label,&start,&end);
        if (!strcasecmp(label,opcode)){
            fseek(deftab,start,SEEK_SET);
            break;
        }
    }
    while(ftell(deftab) < end){
        
    }
}
