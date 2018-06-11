#include "../global.h"

Node *findMethod(List *body, char *expected){
    List *m = body, *main=NULL;
    int c = 0;
    while(m){
        Node *n = m->val;
        if(instanceOf(n, methodK) && !strcmp(expected,fetchName(n->val.method.name))){
            c++;
            if(c>1) error("Multiple main functions found.");
            main = m;
        }
        m = m->next;
    }
    if(main) return main->val;
    else EP("method <%s> expected\n",expected);
} 

Node *findModule(List *modules, char *expected){
    List *m = modules, *main=NULL;
    int c = 0;
    while(m){
        if(!strcmp(expected,fetchName(m->val->val.module.name))){
            c++;
            if(c>1) error("Multiple main functions found.");
            main = m;
        }
        m = m->next;
    }
    if(main) return main->val;
    else EP("module <%s> expected\n",expected);
} 


