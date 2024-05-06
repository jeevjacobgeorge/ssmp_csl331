#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char str[100],tr[100];
char label[10],opcode[10],operand[10];
char start_tr[10];
FILE * src, *optab, *symtab,*out;
int locctr;
void readline(){
    fgets(str,sizeof(str),src);
    int n = sscanf(str,"%s\t%s\t%[^\n]",label,opcode,operand);
    if (n == 2){
        label[0] = '\0';
        sscanf(str,"\t%s\t%[^\n]",opcode,operand);
    }
}
void write_tr(){
    int l = strlen(tr);
    if (l < 3) return;
    char len_hex[2];
    sprintf(len_hex,"%X",l);
    fprintf(out,"T%s.%s.%s\n",start_tr,len_hex,tr);
    sprintf(start_tr,"%X",locctr);
    tr[0] = '\0';
}
int main(){
    char start_addr[10];
    int start_dec;
    char pgm_name[10];
    char lh[10];
    src = fopen("source.txt","r");
    out = fopen("obj.txt","w");
    symtab = fopen("symtab.txt","w");
    fclose(symtab);
    fprintf(out,"                     \n");
    readline();
    if (!strcasecmp(opcode,"START")){
        strcpy(pgm_name,label);
        strcpy(start_addr,operand);
        strcpy(start_tr,start_addr);
        start_dec = strtol(start_addr,NULL,16);
        locctr = start_dec;
        readline();
    }
    else{
       return 1;
    }
    while(strcasecmp(opcode,"END")){
        if (label[0]){
            symtab = fopen("symtab.txt","a");
            sprintf(lh,"%X",locctr);
            fprintf(symtab,"%s\t%s\n",label,lh);
            fclose(symtab); 
        }
        int flag = 0;
        optab = fopen("optab.txt","r");
        while(!feof(optab)){
            char op[10],code[4];
            fscanf(optab,"%s\t%s\n",op,code);
            if (!strcasecmp(op,opcode)){
                strcat(tr,code);
                flag = 1;
                break;
            }
        }
        fclose(optab);
        if (flag){
            symtab= fopen("symtab.txt","r");
            while(!feof(symtab)){
                char sym[10],addr[10];
                fscanf(symtab,"%s\t%s\n",sym,addr);
                if(!strcasecmp(sym,operand)){
                    strcat(tr,addr);
                    locctr += 3;
                    break;
                }
            }
        }
        else{
            if (!strcasecmp(opcode,"RESW")){
                locctr += 3*strtol(operand,NULL,10);
            }
            else if (!strcasecmp(opcode,"RESB")){
                locctr += strtol(operand,NULL,10);
            }
            else if (!strcasecmp(opcode,"word")){
                locctr += 3;
                strcat(tr,operand);
            }
            else if (!strcasecmp(opcode,"BYTE")){
                if (operand[0] == 'C'){
                    locctr += strlen(operand)-3;
                    for(int i = 2; i < (strlen(operand)-1); i++){
                        char ascii[4];
                        sprintf(ascii,"%X",(int)operand[i]);
                        strcat(tr,ascii);  
                    }
                }
                else if (operand[0] == 'X'){
                    locctr += (strlen(operand)-3)/2;
                    char str2[50];
                    sscanf(operand,"X'%[^']",str2);
                    strcat(tr,str2); 
                }
            }
        }
        write_tr();
        readline();
    }
    fprintf(out,"E%s",operand);
    fseek(out,0,SEEK_SET);
    int length_pgm = locctr - start_dec;
    char length_hex_pgm[8];
    sprintf(length_hex_pgm,"%X",length_pgm);
    fprintf(out,"H%s%s.%s",pgm_name,start_addr,length_hex_pgm);
    fclose(src);
    fclose(out);
    return 0;
}