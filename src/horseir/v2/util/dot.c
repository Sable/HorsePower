#include "../global.h"

/*
 * TODO:
 *   - add text for each node
 *   - add highlight for fusible sections
 *   - improve lines for function calls (optional)
 */

typedef struct _stringlist{
    S msg;
    struct _stringlist *next;
}StrList;

typedef enum DotKind {
    NormalDot, MermaidDot
}DotKind;

DotKind dotKind;
static StrList *extraList;
static C tempStr[999];
static I curId;
static S curFuncName;

extern List *compiledMethodList;

/* ----------------- declaration above ----------------- */

static StrList *newList(S msg){
    StrList *n = NEW(StrList);
    n->msg = msg?strdup(msg):NULL;
    n->next = NULL;
    R n;
}

static void addList(StrList *src, StrList *n){
    n->next = src->next;
    src->next = n;
}

static void printStrList1(StrList *src){
    if(src){
        printStrList1(src->next);
        P("%s\n", src->msg);
    }
}

static void printStrList(StrList *src){
    printStrList1(src->next);
}

static void printDotMermaid(L x, L y){
    P(Indent4);
    P("N%lld",x);
    P(" --> ");
    P("N%lld",y);
    P("\n");
}

static void printDotNormal(L x, L y){
    P(Indent4);
    P("N%lld",x); //printDotNode(method[x].node, false);
    P(" -> ");
    P("N%lld",y); //printDotNode(method[y].node, false);
    P("\n");
}

static void dotBuildNode(Node *def, Node *use){
    L line_def = def->lineno;
    L line_use = use->lineno;
    switch(dotKind){
        case NormalDot : printDotNormal(line_use, line_def); break;
        case MermaidDot: printDotMermaid(line_use, line_def); break;
    }
}

static void dotBuildCall(I graphId, Node *use, S funcName){
    L nodeId = use->lineno;
    switch(dotKind){
        case NormalDot:
            SP(tempStr, Indent2 "N%lld -> start%d", nodeId, graphId);
            break;
        case MermaidDot:
            {
                S s = tempStr;
                s += SP(s, Indent4 "func%d[\"%s\"] -.-> node%lld((\"N%lld\"))\n",
                        curId, curFuncName, nodeId, nodeId);
                s += SP(s, Indent4 "node%lld --> func%d[\"%s\"]",
                        nodeId, graphId, funcName);
                // optional: round the item to highlight
                P(Indent4 "N%lld((N%lld))\n", nodeId, nodeId);
            }
            break;
    }
    addList(extraList, newList(tempStr));
}

static void dotBuild(Chain *def, Chain *use){
    dotBuildNode(chainNode(def), chainNode(use));
}

static void dotScanParam(Chain *chain){
    Node *n = chainNode(chain);
    List *param = getNodeParams(n);
    while(param){
        Node *p = param->val;
        if(instanceOf(p, nameK)){
            SymbolKind sk = nodeNameKind(p);
            if(sk == localS){
                I c = findDefByName(chain, nodeName2(p));
                if(c >= 0){
                    dotBuild(chain, chain->chain_defs[c]);
                }
            }
        }
        param = param->next;
    }
}

static Node *findMethodByName(List *list, S name){
    while(list){
        Node *method = list->val;
        if(sEQ(nodeMethodName(method), name)){
            R method;
        }
        list = list->next;
    }
    R NULL;
}

static I findMethodId(List *list, S name, I id){
    if(list){
        I k = findMethodId(list->next, name, id+1);
        if(k >= 0) R k;
        else {
            if(sEQ(nodeMethodName(list->val), name)){
                R id;
            }
        }
    }
    R -1;
}

static I findMethodIdByName(List *list, S name){
    R findMethodId(list, name, 0);
}

static void dotScanFunc(Node *n){
    Node *func = getNodeFunc(n);
    if(func){
        SymbolKind sk = nodeNameKind(func);
        if(sk == methodS){
            I graphId = findMethodIdByName(\
                       compiledMethodList->next, \
                       nodeName2(func));
            if(graphId >= 0){
                dotBuildCall(graphId, n, nodeName2(func));
            }
            else
                EP("Function not found: %s", nodeName2(func));
        }
    }
}

static void dotScan(Chain *chain){
    Node *n = chainNode(chain);
    //P("enter dotScan\n");
    //printChain(chain);
    if(instanceOf(n, stmtK)){
        dotScanFunc(n);
        dotScanParam(chain);
    }
}

static void dotChain(ChainList *list){
    if(list){
        dotChain(list->next);
        dotScan(list->chain);
    }
}

static void printDotHeadNormal(S fn, I graphId){
    P(Indent2 "subgraph cluster%d {\n", graphId);
    P(Indent4 "label=\"%s\"\n", fn);
    if(sNEQ(fn, "main")){
        P(Indent4 "start%d\n", graphId);
    }
}

static void printDotHeadMermaid(S fn){
    P(Indent2 "subgraph %s\n", fn);
}

static void printSubgraphHead(S fn, I graphId){
    switch(dotKind){
        case NormalDot:
            printDotHeadNormal(fn, graphId);
            break;
        case MermaidDot: 
            printDotHeadMermaid(fn);
            break;
    }
}

static void printSubgraphTail(){
    switch(dotKind){
        case NormalDot:
            P(Indent2 "}\n");
            break;
        case MermaidDot:
            P(Indent2 "end\n");
            break;
        default:
            EP("Unknown dot kind: %d\n", dotKind);
    }
}

static void dotMethod(Node *n, I graphId){
    //P("// method: %s\n", nodeMethodName(n));
    ChainList *chains = nodeMethodChainList(n);
    //P("pointer check: %lld -> %lld\n", (L)n, (L)chains);
    printSubgraphHead(nodeMethodName(n), graphId);
    curId = graphId;
    curFuncName = nodeMethodName(n);
    dotChain(chains);
    printSubgraphTail();
}

static void dotMethodList(List *list, I id){
    if(list){
        dotMethodList(list->next, id+1);
        dotMethod(list->val, id);
    }
}

static void setDotKind(DotKind k){
    switch(k){
        case NormalDot: break;
        case MermaidDot:break;
        default:
            EP("Unknown dot kind: %d\n", k);
    }
    dotKind = k;
}

static void printDotHead(){
    switch(dotKind){
        case NormalDot : P("digraph G {\n"); break;
        case MermaidDot: P("graph TB\n");    break;
    }
}

static void printDotTail(){
    switch(dotKind){
        case NormalDot : P("}\n"); break;
        case MermaidDot: break;
    }
}

static void printDotCallgraph(){
    switch(dotKind){
        case NormalDot:
            printStrList(extraList); // TODO: clean extraList
            break;
        case MermaidDot:
            printSubgraphHead("callgraph", -1);
            printStrList(extraList);
            printSubgraphTail();
    }
}

static void init(){
    extraList = newList(0);
}

static void runDot(){
    init();
    printDotHead();
    dotMethodList(compiledMethodList->next, 0);
    printDotCallgraph();
    printDotTail();
}

void dotProg(){
    printBanner("DOT Printer");
    setDotKind(NormalDot);
    runDot();
}

void dotProgMermaid(){
    printBanner("DOT Printer for Mermaid Format");
    setDotKind(MermaidDot);
    runDot();
}

