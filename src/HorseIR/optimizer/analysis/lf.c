#include "../global.h"

/* signatures */

extern Chain *exitChain;

#define chainNode(c) (c->cur)
#define isSimpleStmt(n) instanceOf(n,simpleStmtK)
#define isCastStmt(n) instanceOf(n,castStmtK)
#define isAssignment(n) (isSimpleStmt(n)||isCastStmt(n))
#define isReturnStmt(n) instanceOf(n,returnK)
#define setVisited(c, v) c->isVisited=v
#define isVisited(c) (c->isVisited)

static int findParamName(char *pName, Chain *chain){
    NameList *ptr = chain->uses;
    int c = 0;
    while(ptr->next){
        ptr = ptr->next;
        if(!strcmp(pName, ptr->name)) return c;
        c++;
    }
    return -1;
}

static bool findFusion(Chain *chain, char *buff){
    //PN("------------");
    //printChainInfo(chain, 'a');
    //PN("+----------+");
    setVisited(chain, true);
    if(isSimpleStmt(chainNode(chain))){
        Node *expr = chainNode(chain)->val.simpleStmt.expr;
        Node *func = expr->val.expr.func;  // function node
        if(!func) return false;            // literal assignment -> true
        Node *param= expr->val.expr.param; // parameter node
        char *funcName = fetchName(func);
        List *params   = param->val.listS;
        sprintf(buff,"%s(",funcName);
        int pId = 0;
        while(params){
            if(pId>0) strcat(buff, ",");
            Node *pLiteral = params->val;  //literalParamK
            Node *p = pLiteral->val.nodeS;
            if(instanceOf(p, idK)){
                char *pName = fetchName(p);
                int c = findParamName(pName, chain);
                if(c>=0){
                    if(!findFusion(chain->chain_defs[c], buff+strlen(buff))){
                        strcat(buff, pName);
                    }
                }
                else error("<0 index found\n");
            }
            params=params->next;
            pId++;
        }
        strcat(buff, ")");
        return true;
    }
    else if(isReturnStmt(chainNode(chain))){
        DOI(chain->defSize, findFusion(chain->chain_defs[i], buff+strlen(buff)))
    }
    else {
        //printNodeKind(chainNode(chain));
    }
    return false;
}

static void analyzeChain(Chain *chain){
    //if(isAssignment(chainNode(chain))){
    //}
    char buff[512]; buff[0]=0;
    findFusion(chain, buff);
    P("buff = %s\n",buff);
    //prettyNode(chainNode(chain));
    //DOI(chain->defSize, analyzeChain(chain->chain_defs[i]))
}

/* entry */
void analyzeLF(){
    analyzeChain(exitChain);
}

