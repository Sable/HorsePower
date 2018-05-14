/* constants */

const char *FUNCTIONS[] = {
    /* unary */
    "abs", "neg", "ceil", "floor", "round", "conj", "recip", "signum", "pi" , "not",
    /* binary */
    "lt" ,  "gt", "leq" , "geq"  , "eq"   , "neq" , "plus" , "minus" , "mul", "div"
};

typedef enum FunctionType {
    /* unary */
    absF, negF, ceilF, floorF, roundF, conjF, recipF, signumF,  piF, notF,
    /* binary */
     ltF,  gtF,  leqF,   geqF,    eqF,  neqF,  plusF,  minusF, mulF, divF,
    totalFunc
}pFunc;

#define CASE(c, k, x) case k: *num=c; return x; break;
/* monadic */
#define ruleAbs    commonArith
#define ruleNeg    commonArith
#define ruleCeil   commonArith
#define ruleFloor  commonArith
#define ruleRound  commonArith
#define ruleConj   NULL
#define ruleRecip  NULL
#define ruleSignum NULL
#define rulePi     NULL
#define ruleNot    NULL
/* dyadic */ 
#define ruleLt    NULL
#define ruleGt    NULL
#define ruleLeq   NULL
#define ruleGeq   NULL
#define ruleEq    NULL
#define ruleNeq   NULL
#define rulePlus  NULL
#define ruleMinus NULL
#define ruleMul   NULL
#define ruleDiv   NULL
/* others */ 

#define isT(t) (t==x)

#define isBT isT(boolT)
#define isIT isT(i64T)||isT(i32T)||isT(i16T)||isT(i8T)
#define isFT isT(f64T)||isT(f32T)
#define isUT isT(unknownT)

bool isArith(pType x) {return isIT||isFT||isBT||isUT;}

InfoNode *commonArith(InfoNode *x){
    if(isArith(x->type)){
        InfoNode *in = NEW(InfoNode);
        in->type = x->type;
        // in->shape = copyShape(x->shape)
        return in;
    }
    else return NULL;
} 

int findInBuiltinSet(char *funcName, const char *set[]){
    DOI(strlen((char*)set), if(!strcmp(funcName, set[i])) return i)
    return -1;
}

void *fetchTypeRules(char *name, int* num){
    int k = findInBuiltinSet(name, FUNCTIONS);
    if(k>=0){
        switch(k){
            CASE(1,   absF, ruleAbs)
            CASE(1,   negF, ruleNeg)
            CASE(1,  ceilF, ruleCeil)
            CASE(1, floorF, ruleFloor)
            CASE(1, roundF, ruleRound)
            CASE(1,  conjF, ruleConj)
            CASE(1, recipF, ruleRecip)
            CASE(1,signumF, ruleSignum)
            CASE(1,    piF, rulePi)
            CASE(1,   notF, ruleNot)
            CASE(2,    ltF, ruleLt)
            CASE(2,    gtF, ruleGt)
            CASE(2,   leqF, ruleLeq)
            CASE(2,   geqF, ruleGeq)
            CASE(2,    eqF, ruleEq)
            CASE(2,   neqF, ruleNeq)
            CASE(2,  plusF, rulePlus)
            CASE(2, minusF, ruleMinus)
            CASE(2,   mulF, ruleMul)
            CASE(2,   divF, ruleDiv)
        }
    }
    else return NULL;
}



