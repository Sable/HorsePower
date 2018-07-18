#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#define P printf

const char SetInt[] = "BHIL";        // size: 4
const int  SetIntSize = 4; 
const char SetFloat[] = "FE";        // size: 2
const int  SetFloatSize = 2; 
const char SetReal[] = "BHILFE";     // size: 6
const int  SetRealSize = 6;
const char SetString[] = "CQS";      // size: 3
const int  SetStringSize = 3; 
const char SetDatetime[] = "MDZUWT"; // size: 6
const int  SetDatetimeSize = 6;
const char SetBasic[] = "BHILFECQSMDZUWTX"; // size: 16
const int  SetBasicSize = 16;
const char SetAll[] = "BHILFECQSMDZUWTXGNYAK"; // size: 21
#define SetAllSize 21
char map[SetAllSize][SetAllSize] = {0};

void pending(char *str, char *type){
    P("pending: %s (%s)\n", str,type); exit(99);
}

int getIntFromChar(char x){
    //P("len = %d\n",strlen(SetAll));
    for(int i=0; i<SetAllSize;i++){
        if(x==SetAll[i]) return i;
    }
    P("char (%c) is not a valid type alias\n", x);
    getchar();
    exit(1);
}

void setMap(int x, int y, char t){
    if(map[x][y]){
        fprintf(stderr, "type %c and %c defined multiple times (%d,%d)\n", SetAll[x], SetAll[y],x,y);
    }
    map[x][y] = t;
}

void printMapText(){
    P("|   |");
    for(int i=0;i<SetAllSize;i++){
        P(" %c |", SetAll[i]);
    }
    P("\n");
    for(int i=0;i<SetAllSize;i++){
        P("| %c |", SetAll[i]);
        for(int j=0;j<SetAllSize;j++){
            if(map[i][j]) P(" %c |", map[i][j]);
            else P("   |");
        }
        P("\n");
    }
}

void printMapLatex(){
    P("\\documentclass[\n");
    P("  convert={\n");
    P("    density=300 -alpha deactivate,\n");
    P("    size=1080x800,\n");
    P("    outext=.png\n");
    P("  },\n");
    P("]{standalone}\n\n");
    P("\\begin{document}\n");
    P("\\begin{small}\n");
    P("\\begin{tabular}{c|");
    for(int i=0; i<SetAllSize;i++) P("|c");
    P("||c}\\hline\n   ");
    for(int i=0;i<SetAllSize;i++){
        P("& %c ", SetAll[i]);
    }
    P("& \\\\ \\hline\\hline\n");
    for(int i=0;i<SetAllSize;i++){
        P(" %c ", SetAll[i]);
        for(int j=0;j<SetAllSize;j++){
            if(map[i][j]) P("& %c ", map[i][j]);
            else P("&   ");
        }
        P("& %c ", SetAll[i]);
        P("\\\\ \\hline\n");
    }
    P("\\end{tabular}\n");
    P("\\end{small}\n");
    P("\\end{document}\n");
}

void processReturn(int idx, int idy, char x, char y, char *return_type){
    if(strlen(return_type)==1){
        setMap(idx, idy, return_type[0]);
    }
    else if(!strcmp(return_type, "MaxType")){
        setMap(idx, idy, idx>=idy?x:y);
    }
    else if(!strcmp(return_type, "Right")){
        setMap(idx, idy, y);
    }
    else if(!strcmp(return_type, "Left")){
        setMap(idx, idy, x);
    }
    else {
        pending("unknown return type found: %s\n", return_type);
    }
}

void processPair(const char *left_set, int left_set_size, char *left_type,
                 const char *right_set,int right_set_size,char *right_type,
                 char *return_type){
    for(int i=0;i<left_set_size;i++){
        for(int j=0;j<right_set_size;j++){
            int id_x = getIntFromChar(left_set[i]);
            int id_y = getIntFromChar(right_set[j]);
            //P("x = %d, y = %d, single = %d\n",id_x,id_y,single);
            processReturn(id_x, id_y, left_set[i], right_set[j], return_type);
        }
    }
}

void processPairLeft(const char *left_set, int left_set_size,
                     char *left_type, char *right_type, char *return_type){
    int id_y = getIntFromChar(right_type[0]);
    //P("right = %s, %s, size = %d\n", left_type, right_type, left_set_size);
    for(int i=0;i<left_set_size;i++){
        int id_x = getIntFromChar(left_set[i]);
        processReturn(id_x, id_y, left_set[i], right_type[0], return_type);
    }
}

void processPairRight(const char *right_set, int right_set_size,
                     char *left_type, char *right_type, char *return_type){
    int id_x = getIntFromChar(left_type[0]);
    //P("right = %s, %s\n", right_str, right_type);
    for(int i=0;i<right_set_size;i++){
        int id_y = getIntFromChar(right_set[i]);
        processReturn(id_x, id_y, left_type[0], right_set[i], return_type);
    }
}

const char* getSetInfo(char *name){
    if(!strcmp(name, "Real")){
        return SetReal;
    }
    else if(!strcmp(name, "Datetime")){
        return SetDatetime;
    }
    else if(!strcmp(name, "Int")){
        return SetInt;
    }
    else if(!strcmp(name, "Float")){
        return SetFloat;
    }
    else if(!strcmp(name, "String")){
        return SetString;
    }
    else if(!strcmp(name, "Any")){
        return SetAll;
    }
    else if(!strcmp(name, "Basic")){
        return SetBasic;
    }
    else {
        P("unknown name: %s\n", name);
        exit(1);
    }
}


void processLine(char *typ_left, char *typ_right, char *typ_return){
    if(strlen(typ_left) > 1){
        const char *left_set  = getSetInfo(typ_left);
        if(strlen(typ_right) > 1){
            const char *right_set = getSetInfo(typ_right);
            processPair(left_set, strlen(left_set), typ_left,\
                        right_set,strlen(right_set),typ_right, typ_return);
        }
        else {
            processPairLeft(left_set, strlen(left_set), typ_left, typ_right, typ_return);
        }
    }
    else {
        if(strlen(typ_right) > 1){
            const char *right_set = getSetInfo(typ_right);
            processPairRight(right_set, strlen(right_set), typ_left, typ_right, typ_return);
        }
        else {
            int id_x = getIntFromChar(typ_left[0]);
            int id_y = getIntFromChar(typ_right[0]);
            processReturn(id_x, id_y, typ_left[0], typ_right[0], typ_return);
        }
    }
}

int main(int argc, char **argv){
    if(argc != 2){
        P("Usage: %s <file_name>.txt\n", argv[0]);
        exit(1);
    }
    FILE* fp = fopen(argv[1], "r");
    char line[999]; char typ_left[99], typ_right[99], typ_return[99];
    int lineno = 0;
    while(fgets(line, 999, fp)){
        sscanf(line, "%[^,] , %s -> %s", typ_left, typ_right, typ_return);
        if(strlen(typ_left) < 1 || strlen(typ_right) < 1 || strlen(typ_return) < 1){
            P("[Line %2d] the len of input string must >= 1, %s, %s, %s\n", lineno,typ_left, typ_right, typ_return);
            exit(1);
        }
        processLine(typ_left, typ_right, typ_return);
        lineno++;
    }
    printMapLatex();
    return 0;
}
