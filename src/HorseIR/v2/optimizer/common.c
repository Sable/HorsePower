#include "../global.h"

extern bool *ElementwiseUnaryMap;
extern bool *ElementwiseBinaryMap;

S obtainMaxValue(C c){
    switch(c){
        case 'E': return "MAX_DBL";
        case 'I': return "MAX_INT";
        default: EP("Add impl.");
    }
}

S obtainMinValue(C c){
    switch(c){
        case 'E': return "MIN_DBL";
        case 'I': return "MIN_INT";
        default: EP("Add impl.");
    }
}

List *getParamsIndex(List *list, I pos){
    if(list){
        return (pos>0)?getParamsIndex(list->next, pos-1):list;
    }
    return NULL;
}

List *getNodeParams(Node *n){
    if(instanceOf(n, stmtK)){
        Node *rhs = n->val.assignStmt.expr;
        if(instanceOf(rhs, castK)){
            rhs = rhs->val.cast.exp;
        }
        if(instanceOf(rhs, callK)){
            return rhs->val.call.param->val.listS;
        }
        // TODO: any other cases?
    }
    return NULL;
}

Node *getNodeFunc(Node *n){
    if(instanceOf(n, stmtK)){
        Node *rhs = n->val.assignStmt.expr;
        if(instanceOf(rhs, callK)){
            return rhs->val.call.func;
        }
        else if(instanceOf(rhs, castK)){
            Node *t = rhs->val.cast.exp;
            return instanceOf(t, callK)?t->val.call.func:NULL;
        }
    }
    return NULL;
}

Node *getNodeFirstParam(Node *n){
    List *params = nodeList(n->val.call.param);
    while(params && params->next){
        params = params->next;
    }
    R params?params->val:0;
}

Node *getSingleFunc(Node *funcs){
    if(totalList(nodeList(funcs)) == 1){
        return nodeList(funcs)->val;
    }
    else R 0;
}

Node *getEachFuncNode(Node *n){
    Node *call = getStmtCall(n);
    return getSingleFunc(getNodeFirstParam(call));
}

/*
 * 0: targ
 * 1: 1st param
 * 2: 2nd param
 */
Node *getNodeItemIndex(Node *n, I pos){
    if(instanceOf(n, stmtK)){
        List *vars = n->val.assignStmt.vars;
        Node *targ = NULL;
        if(1 == totalList(vars)) targ = vars->val;
        else EP("only one targ expected");
        if(pos == 0) return targ;
        Node *func = getNodeFunc(n);
        if(!func) return NULL;
        List *params  = getNodeParams(n);
        I reversedPos = totalList(params) - pos;
        List *p0      = getParamsIndex(params, reversedPos);
        if(p0 && instanceOf(p0->val, nameK))
            return p0->val;
        else {
            printNode(n);
            P("\n looking for pos %d, but find: ", reversedPos);
            if(p0) printNode(p0->val);
            //printNode(params->val);
            //printNode(params->next->val);
            //printNode(params->next->next->val);
            P("total parameters: %d, pos: %d\n", totalList(params), pos);
            EP("only variable allowed (no func/literal)");
        }
    }
    return NULL;
}

SymbolName *getNodeSymbolName(Node *n){
    SymbolName *sn = NULL;
    if(instanceOf(n, nameK)){
        sn = n->val.name.sn;
    }
    else if(instanceOf(n, varK)){
        sn = n->val.param.sn;
    }
    else TODO("Add impl. %d",n->kind);
    return sn;
}

S getNameStr(Node *n){
    // input nameK
    SymbolName *sn = getNodeSymbolName(n);
    switch(sn->kind){
        case   localS: return sn->name;
        default: TODO("Add impl.");
    }
}

static char obtainTypeCodeByIn(InfoNode *in){
#define isBasicType(x) (!(x)->subInfo && !(x)->next)
    if(isBasicType(in)){
        switch(in->type){
            case  boolT: return 'B';
            case   i32T: return 'I';
            case   i64T: return 'L';
            case   f32T: return 'F';
            case   f64T: return 'E';
            case monthT: return 'M';
            case  charT: return 'C';
            case   symT: return 'Q';
            case   strT: return 'S';
            case  dateT: return 'D';
            default: TODO("add type: %d\n", in->type);
        }
    }
    else if(isListT(in)){
        // TODO: condition appplied with single cell type
        return obtainTypeCodeByIn(in->subInfo);
    }
    else TODO("Only basic type supported: %d\n", in->type);
}

C getTypeCodeByName(Node *n){
    SymbolName *sn = getNodeSymbolName(n);
    switch(sn->kind){
        case localS: {
                Node * typ = sn->val.local->val.param.typ;
                return obtainTypeCodeByIn(typ->val.type.in);
             }
        default: TODO("add impl.");
    }
}

static S obtainFuncElem(S fn){
    // elementwise
    if(sEQ(fn, "plus"))  R "PLUS" ;
    if(sEQ(fn, "minus")) R "MINUS";
    if(sEQ(fn, "div"))   R "DIV";
    if(sEQ(fn, "mul"))   R "MUL";
    if(sEQ(fn, "not"))   R "NOT";
    if(sEQ(fn, "and"))   R "AND";
    if(sEQ(fn, "or"))    R "OR" ;
    if(sEQ(fn, "lt"))    R "LT" ;
    if(sEQ(fn, "leq"))   R "LEQ";
    if(sEQ(fn, "gt"))    R "GT" ;
    if(sEQ(fn, "geq"))   R "GEQ";
    if(sEQ(fn, "eq"))    R "EQ" ;
    if(sEQ(fn, "neq"))   R "NEQ";
    R NULL;
}

static S obtainFuncAuto(S fn){
    S x = obtainFuncElem(fn);
    if(x) R x;
    if(sEQ(fn, "index"))  R "INDEX";
    if(sEQ(fn, "like"))   R "LIKE";
    if(sEQ(fn, "member")) R "MEMBER";
    if(sEQ(fn, "len"))    R "vn";
    R NULL;
}

S getFuncNameC(S fn){
    S macro = obtainFuncElem(fn);
    if(macro) return macro;
    else TODO("Add impl. for %s", fn);
}

S getFuncNameAuto(S fn){
    S macro = obtainFuncAuto(fn);
    if(macro) return macro;
    else TODO("Add impl. for %s\n", fn);
}

B isElementwise(S funcName){
    FuncUnit x;
    getFuncIndexByName(funcName, &x);
    switch(x.kind){
        case 1: return ElementwiseUnaryMap [x.u];
        case 2: return ElementwiseBinaryMap[x.b];
    }
    return false;
}

/* ---------- common in optimizations ---------- */
extern C code[CODE_MAX_SIZE], *ptr;
extern I varNum;
extern S varNames[99];

S genDeclSingle(S func, C del){
    C temp[199]; 
    SP(temp, "static I %s(V z, V *x)%c", func, del);
    return strdup(temp);
}

L searchName(S *names, S s){
    DOI(varNum, if(!strcmp(names[i],s))R i) R -1;
}

B isDuplicated(S *names, S s){
    return searchName(names, s) >= 0;
}

// copy from: compiler.c/scanConst
O genCodeConst(Node *n){
    ConstValue *v = n->val.nodeC;
    switch(v->type){
        case    symC:
        case    strC: glueAny("%s",v->valS); break;
        case   dateC:
        case  monthC:
        case   timeC:
        case minuteC:
        case secondC:
        case    intC: glueAny("%d"  , v->valI); break;
        case  floatC: glueAny("%g"  , v->valF); break;
        case     dtC:
        case   longC: glueAny("%lld", v->valL); break;
        case   clexC: glueAny(v->valX[1]>=0?"%g+%g":"%g%g", \
                              v->valX[0], v->valX[1]); break;
        default: EP("Add more constant types: %d\n", v->type);
    }
}

static O genCodeVector(Node *n){
    genCodeList(n->val.vec.val);
}

O genCodeName(Node *n, I id){
    C typeCode = getTypeCodeByName(n);
    //P("looking for "); printNode(n); getchar();
    glueAny("v%c(x%d,i)", typeCode, id);
}

O genCodeNode(Node *n){
    switch(n->kind){
        case vectorK: genCodeVector(n); break;
        case  constK: genCodeConst (n); break;
        default: TODO("Add impl. for %s", getNodeTypeStr(n));
    }
}

O genCodeList(List *list){
    if(list){ genCodeList(list->next); genCodeNode(list->val); }
}

S genInvcSingle(S targ, S func, S *names, I num){
    C temp[199]; S ptr = temp;
    ptr += SP(ptr, "%s(%s, (V[]){",func,targ);
    DOI(num, ptr+=SP(ptr,(i>0?",%s":"%s"),names[i]))
    SP(ptr, "})");
    return strdup(temp);
}

