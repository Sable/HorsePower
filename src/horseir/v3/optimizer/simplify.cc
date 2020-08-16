#include "../global.h"

extern List *compiledMethodList;
extern sHashTable *hashDel;

/*
 * Description:
 *    t = x
 *    y = t + 1
 *    ==>
 *    y = x + 1
 */

static void replaceNode(Node *n, Node *key, Node *rhs){
    List *param = n->val.listS;
    S keyStr = key->val.param.id;
    while(param){
        Node *p = param->val;
        if(instanceOf(p, nameK) && sEQ(nodeName2(p), keyStr)){
            param->val = rhs;
        }
        param = param->next;
    }
}

static void replaceCopyStmt(Chain *def_chain, Chain *use_chain){
    Node *def = chainNode(def_chain);
    Node *use = chainNode(use_chain);
    List *lhs = nodeStmtVars(def);
    Node *rhs = nodeStmtExpr(def);
    //WP("size of vars: %d\n", totalList(lhs));
    if(totalList(lhs) == 1 && instanceOf(nodeStmtExpr(use),callK)){
        replaceNode(nodeCallParam(nodeStmtExpr(use)), lhs->val, rhs);
        // update chain
        L c = 0;
        DOI(use_chain->defSize,
              if(use_chain->chain_defs[i] != def_chain)
                use_chain->chain_defs[c++] = use_chain->chain_defs[i])
        use_chain->defSize = c;  // actual defSize-1
    }
}


static B scanChainList(ChainList *list){
    //printChainList(list->next);
    //getchar();
    ChainList *prev = list;
    list = list->next;
    B found = false;
    while(list){
        Chain *chain = list->chain;
        Node *stmt = chainNode(chain);
        printNode(stmt);
        if(instanceOf(stmt,stmtK)){
            Node *rhs = nodeStmtExpr(stmt);
            if(instanceOf(rhs,nameK) || instanceOf(rhs,vectorK)){
            // if(instanceOf(rhs,nameK)){
                //WP("removed: "); printNode(stmt); getchar();
                // WP("before\n"); DOI(chain->useSize, printNode(chainNode(chain->chain_uses[i])))
                DOI(chain->useSize, replaceCopyStmt(chain, chain->chain_uses[i]))
                addToSimpleHash(hashDel, (L)stmt, 1); // add to delete
                prev->next = list->next;
                // TODO: free current list
                found = true;
            }
        }
        prev = list;
        list = list->next;
    }
    R found;
}

static void compileMethod(Node *n){
    ChainList *chains = nodeMethodChainList(n);
    ChainList *dummy = (ChainList*)malloc(sizeof(ChainList));
    dummy->next = chains;
    // printChainList(dummy->next);
    B found = scanChainList(dummy);
    // if(found){
    //     printChainList(dummy->next);
    //     getchar();
    // }
    nodeMethodChainList(n) = dummy->next;
    free(dummy);
}

static void scanMethodList(List *list){
    if(list) {
        scanMethodList(list->next);
        compileMethod(list->val);
    }
}

static void init(){
}

static void clean(){
}

void simplifyUDChain(){
    printBanner("Optimization: Simplify copy statements");
    init();
    scanMethodList(compiledMethodList->next);
    clean();
}
