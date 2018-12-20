#include "../global.h"

#define sameName(n1,n2) (!strcmp(n1->val.idS, n2->val.idS))
#define sameType(t1,t2) (t1->val.typeS == t2->val.typeS)
#define fetchNameStr(n) (n)->val.idS 
#define fetchModuleName(m) fetchNameStr(m->val.module.name)
#define fetchMethodName(m) fetchNameStr(m->val.method.name)

#define MAIN "main"
static int main_total = 0;

static int countParam(List *p){
    L x = 0; while(p){ x++; p = p->next; } return x;
}

static bool sameParam(List *p1, List *p2){
    int x1 = countParam(p1);
    int x2 = countParam(p2);
    if(x1 == x2){
        while(p1){
            if(!sameName(p1->val, p2->val))
                return false;
            p1 = p1->next;
            p2 = p2->next;
        }
        return true;
    }
    return false;
}

static int countSameItem(List *n_list, Node *n){
/* find a method/...(n) in a module.body(n_list) */
    List *p = n_list;
    int c = 0;
    while(p){
        Node *t = p->val;
        if(t->kind == n->kind){
            if(t->kind == methodK){
                // check name
                if(sameName(t->val.method.name,n->val.method.name)){
                    List *t_param = t->val.method.param;
                    List *n_param = n->val.method.param;
                    // check type
                    int t_num = countParam(t_param);
                    int n_num = countParam(n_param);
                    if(t_num == n_num && sameParam(t_param, n_param)){
                        c++; if(c > 1) return c;
                        //return t; // match!
                    }
                }
            }
            else EP("Kind not found: %d\n", t->kind);
        }
        p = p->next;
    }
    return c;
}

static bool isBodyConflict(List *n_list, List *x_list, bool isSelf){
    bool canReturn = !isSelf;
    while(x_list){
        Node *x0 = x_list->val;
        switch(x0->kind){
            case methodK:
            {
                int c = countSameItem(n_list, x0);
                if(c > 1){
                    P("c = %d\n",c);
                    WP("Method pre-defined: %s\n", fetchMethodName(x0));
                    return true;
                }
            } break;
            case importK: break; /* skip */
            default: EP("TODO: support new kind: %d\n", x0->kind);
        }
        x_list = x_list->next;
    }
    return false;
}

static void checkModuleContent(Node *m){
    List *body = m->val.module.body;
    if(isBodyConflict(body, body, true)){
        EP("Methods conflict\n");
    }
}

static void weedMethod(Node *method){
    char *name = fetchMethodName(method);
    if(!strcmp(name, MAIN)){
        main_total++;
    }
}

static void checkModuleMain(Node *m){
    List *x_list = m->val.module.body;
    while(x_list){
        Node *x0 = x_list->val;
        switch(x0->kind){
            case methodK: weedMethod(x0); break;
        }
        x_list = x_list->next;
    }
}

static void weedModule(Node *module){
    main_total = 0;
    // 1. check conflict method signatures
    checkModuleContent(module);
    //P("1. main_total = %d\n",main_total);
    // 2. check methods/...
    checkModuleMain(module);
    //P("2. main_total = %d\n",main_total);
}

/* merge n and x, and return n */
static Node *mergeModule(Node *n, Node *x){
    List *n_list = n->val.module.body;
    List *x_list = x->val.module.body;
    while(n_list->next)
        n_list = n_list->next;
    n_list->next = x_list;
    return n;
}

static void weedModuleMerged(List *module_list){
    List *p = module_list;
    List *t = p; int size = 0; while(t){ size++; t = t->next; }
    bool *isVisited = (bool*)malloc(size);
    int i = 0;
    while(p){
        if(!isVisited[i]){
            Node *p0 = p->val;
            char *name = fetchModuleName(p0);
            List *x = p;
            while(x && x->next){
                int j = i + 1;
                Node *x0 = x->next->val;
                if(!strcmp(name, fetchModuleName(x0)) && p0!=x0){
                    mergeModule(p0, x0);
                    List *temp = x->next;
                    x->next = x->next->next;
                    free(temp); // free(x0);
                    isVisited[j] = true;
                }
                else x = x->next;
                j++;
            }
            isVisited[i] = true;
        }
        p = p->next; i++;
    }
    free(isVisited);
}

static void weedModuleList(List *module_list){
    weedModuleMerged(module_list);
    List *p = module_list;
    while(p){
        weedModule(p->val);
        p = p->next;
    }
}

static void validateMain(){
    if(main_total != 1){
        EP("Only one main is expected, but %d found!\n", main_total);
    }
}

void weedProg(Prog *root){
    weedModuleList(root->module_list);
    validateMain();
    //prettyProg(root);
}
