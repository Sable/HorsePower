#include "../global.h"
/* check typeshape.h */

/* constants */
const char *FUNCTIONS[] = {
    /* unary 58 */
    "abs", "neg", "ceil", "floor", "round", "conj", "recip", "signum", "pi" , "not",
    "log", "exp",  "cos",   "sin",   "tan", "acos",  "asin",  "atan", "cosh", "sinh",
    "tanh", "acosh", "asinh", "atanh",
    "date", "date_year", "date_month", "date_day",
    "time", "time_hour", "time_minute", "time_second", "time_mill",
    "unique", "str", "len", "range", "fact", "rand", "seed", "flip", "reverse",
    "where", "group", "count", "sum", "avg", "min", "max", "raze", "enlist", "tolist",
    "format", "keys", "values", "meta", "load_table", "fetch",
    /* binary 35 */
    "lt" ,  "gt", "leq" , "geq"  , "eq", "neq" , "plus", "minus" , "mul", "div",
    "power", "log2", "mod", "and", "or", "nand", "nor" , "xor",
    "datetime_diff", "datetime_add", "datetime_sub",
    "append", "like", "compress", "randk", "index_of", "take", "drop", "order",
    "member", "vector", "match", "index", "column_value", "sub_string",
    /* special 10 */
    "each", "each_item", "each_left", "each_right", "enum", "dict", "table",
    "ktable", "index_a", "list"
};

static ShapeNode *decideShapeElementwise(InfoNode *x, InfoNode *y);

#define CASE(k, x) case k: return x; break;
#define commonTrig commonArith1
/* monadic */
#define ruleAbs     commonArith1
#define ruleNeg     commonArith1
#define ruleCeil    commonArith1
#define ruleFloor   commonArith1
#define ruleRound   commonArith1
#define ruleConj    NULL
#define ruleRecip   NULL
#define ruleSignum  NULL
#define rulePi      NULL
#define ruleNot     commonBool1
#define ruleLog     NULL
#define ruleExp     NULL
#define ruleCos     commonTrig
#define ruleSin     commonTrig
#define ruleTan     commonTrig
#define ruleAcos    commonTrig
#define ruleAsin    commonTrig
#define ruleAtan    commonTrig
#define ruleCosh    commonTrig
#define ruleSinh    commonTrig
#define ruleTanh    commonTrig
#define ruleAcosh   commonTrig
#define ruleAsinh   commonTrig
#define ruleAtanh   commonTrig
#define ruleDate    NULL
#define ruleYear    NULL
#define ruleMonth   NULL
#define ruleDay     NULL
#define ruleTime    NULL
#define ruleHour    NULL
#define ruleMinute  NULL
#define ruleSecond  NULL
#define ruleMill    NULL
#define ruleUnique  specialUnique  //return indices
#define ruleStr     NULL
#define ruleLen     propLen
#define ruleRange   NULL
#define ruleFact    NULL
#define ruleRand    NULL
#define ruleSeed    NULL
#define ruleFlip    NULL
#define ruleReverse NULL
#define ruleWhere   propWhere
#define ruleGroup   propGroup
#define ruleCount   reductionCount
#define ruleSum     reductionSum
#define ruleAvg     propAvg
#define ruleMin     NULL
#define ruleMax     NULL
#define ruleRaze    propRaze
#define ruleEnlist  specialEnlist
#define ruleTolist  NULL
#define ruleFormat  NULL
/* dyadic */ 
#define ruleLt       commonCompare2
#define ruleGt       commonCompare2
#define ruleLeq      commonCompare2
#define ruleGeq      commonCompare2
#define ruleEq       commonCompare2
#define ruleNeq      commonCompare2
#define rulePlus     commonArith2
#define ruleMinus    commonArith2
#define ruleMul      commonArith2
#define ruleDiv      commonArith2
#define rulePower    commonArith2
#define ruleLog2     NULL
#define ruleMod      commonArith2
#define ruleAnd      commonBool2
#define ruleOr       commonBool2
#define ruleNand     commonBool2
#define ruleNor      commonBool2
#define ruleXor      commonBool2
#define ruleDtdiff   NULL
#define ruleDtadd    NULL
#define ruleDtsub    NULL
#define ruleAppend   propAppend
#define ruleLike     propLike
#define ruleCompress specialCompress
#define ruleRandk    NULL
#define ruleIndexof  propIndexof
#define ruleTake     NULL
#define ruleDrop     NULL
#define ruleOrder    propOrder
#define ruleMember   propMember
#define ruleVector   propVector
#define ruleMatch    NULL
#define ruleIndex       propIndex
#define ruleColumnValue specialColumnValue
#define ruleSubString   propSubString
/* special */ 
#define ruleEach        propEach
#define ruleEachItem    propEachItem
#define ruleEachLeft    propEachLeft
#define ruleEachRight   propEachRight
#define ruleEnum        propEnum
#define ruleDict        NULL
#define ruleTable       specialTable
#define ruleKtable      NULL
#define ruleKeys        propKeys
#define ruleValues      propValues
#define ruleMeta        NULL
#define ruleLoadTable   specialLoadTable
#define ruleFetch       NULL
#define ruleIndexA      propIndexA
#define ruleList        propList

#define isT(t) (t==n->type)

#define isBT isT(boolT)
#define isIT isT(i64T)||isT(i32T)||isT(i16T)||isT(i8T)
#define isFT isT(f64T)||isT(f32T)
#define isCT isT(charT)
#define isST isT(symT)||isT(strT)
#define isDT isT(monthT)||isT(dateT)
#define isNT isT(dictT)
#define isTT isT(tableT)||isT(ktableT)
#define sameT(x,y) ((x)->type==(y)->type)

bool isIntIN   (InfoNode *n) {return isIT;}
bool isFloatIN (InfoNode *n) {return isFT;}
bool isBoolIN  (InfoNode *n) {return isBT;}
bool isCharIN  (InfoNode *n) {return isCT;}
bool isRealIN  (InfoNode *n) {return isIT||isFT||isBT;}
bool isStringIN(InfoNode *n) {return isST;}
bool isDateIN  (InfoNode *n) {return isDT;}
bool isBasicIN (InfoNode *n) {return 
    isRealIN(n)||isCharIN(n)||isStringIN(n)||isDateIN(n);}
bool isDictIN  (InfoNode *n) {return isNT;}
bool isFuncIN  (InfoNode *n) {return isT(funcT);}
bool isTableIN (InfoNode *n) {return isTT;}
#define isU(n) (unknownT==(n)->type)
#define isListT(n) (listT==(n)->type)
#define isEnumT(n) (enumT==(n)->type)

#define isS(n, t) (t == n->type)
#define isShapeU(n) isS(n, unknownH)
#define isShapeV(n) isS(n,  vectorH)
#define isShapeL(n) isS(n,    listH)
#define isShapeT(n) isS(n,   tableH)
#define sameH(x,y) ((x)->type==(y)->type)

#define decideShapeLeft(x,y) x->shape
#define decideShapeRight(x,y) y->shape

typedef bool (*TypeCond)(InfoNode *);
int shapeId = 0;

static InfoNode *newInfoNode(pType type, ShapeNode *shape){
    InfoNode *in = NEW(InfoNode);
    in->type     = type;
    in->shape    = shape;
    return in;
}

static InfoNode *newInfoNodeList(pType type, pType sub, ShapeNode *shape){
    InfoNode *in = NEW(InfoNode);
    in->type     = type;
    in->subtype  = sub;
    in->shape    = shape;
    return in;
}

ShapeNode *newShapeNode(pShape type, bool isId, int size){
    ShapeNode *sn = NEW(ShapeNode);
    sn->type = type;
    sn->isId = isId;
    switch(type){
        case unknownH: sn->size   = -1;   break;
        case  vectorH: 
        case    listH:
        case   tableH:
              if(isId) sn->sizeId = size<0?(shapeId++):size;
              else sn->size = size; break;
        default: sn->size   = -2;   break;
    }
    return sn;
}

/* monadic */
static InfoNode *commonElemementSingle(InfoNode *x, TypeCond cond, pType t){
    pType rtnType; 
    if(cond(x)||isU(x)) rtnType = t;
    else return NULL;
    return newInfoNode(rtnType, x->shape);
}

static InfoNode *commonReduction(InfoNode *x, TypeCond cond, pType t){
    pType rtnType;
    if(cond(x)||isU(x)) rtnType = t;
    else return NULL;
    return newInfoNode(rtnType, newShapeNode(vectorH, false, 1));
}

static InfoNode *commonArith1(InfoNode *x){
    return commonElemementSingle(x, &isRealIN, x->type);
} 

static InfoNode *commonBool1(InfoNode *x){
    return commonElemementSingle(x, &isBoolIN, boolT);
}

static InfoNode *reductionSum(InfoNode *x){
    pType rtnType;
    if(isIntIN(x)||isBoolIN(x)) rtnType = i64T;
    else if(isFloatIN(x)) rtnType = f64T;
    else if(isU(x)) rtnType = unknownT;
    else return NULL;
    return newInfoNode(rtnType, newShapeNode(vectorH, false, 1));
}

static InfoNode *reductionCount(InfoNode *x){
    return commonReduction(x, &isBasicIN, i64T);
}

static InfoNode *specialEnlist(InfoNode *x){
    return commonReduction(x, &isBasicIN, listT);
}

static InfoNode *specialUnique(InfoNode *x){
    return commonElemementSingle(x, &isBasicIN, i64T);
}

static InfoNode *propGroup(InfoNode *x){
    return newInfoNode(dictT, newShapeNode(listH, false, 2));
}

static InfoNode *propRaze(InfoNode *x){
    if(isListT(x)){
        return newInfoNode(x->subtype, newShapeNode(vectorH, true, -1));
    }
    else if(isBasicIN(x)){
        return x;
    }
    else return NULL;
}

/* return a scalar */
static InfoNode *propLen(InfoNode *x){ 
    return newInfoNode(i64T, newShapeNode(vectorH, false, 1));
}

static InfoNode *propWhere(InfoNode *x){
    pType rtnType;
    if(isBoolIN(x)){
        rtnType = i64T;
    }
    else if(isU(x)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, newShapeNode(vectorH, true, -1));
}

static InfoNode *propAvg(InfoNode *x){
    pType rtnType;
    if(isRealIN(x)){
        rtnType = f64T;
    }
    else if(isU(x)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, newShapeNode(vectorH, false, 1));
}

/* dyadic */
static InfoNode *commonArith2(InfoNode *x, InfoNode *y){
    pType rtnType;
    if(isRealIN(x)&&isRealIN(x)){
        rtnType = MAX(x->type, y->type);
    }
    else if(isU(x)||isU(y)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, decideShapeElementwise(x,y));
}

static InfoNode *commonBool2(InfoNode *x, InfoNode *y){
    pType rtnType;
    if(isBoolIN(x)&&isBoolIN(y)){
        rtnType = boolT;
    }
    else if(isU(x) || isU(y)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, decideShapeElementwise(x,y));
}

static InfoNode *propMember(InfoNode *x, InfoNode *y){
    pType rtnType;
    if(sameT(x,y) && isBasicIN(x)){
        rtnType = boolT;
    }
    else if(isU(x) || isU(y)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, decideShapeRight(x,y));
}

static ShapeNode *decideShapeV(ShapeNode *x, ShapeNode *y){
    ShapeNode *rtnShape = NULL;
    if(!(x->isId)){
        if(x->size == 1) rtnShape = y;
        else if(isShapeV(y) && !(y->isId)){
            if(x->size == y->size) rtnShape = y;
            else error("length of both sides should obey the elemetwise rule");
        }
        else rtnShape = newShapeNode(unknownH, true, -1);
    }
    else {
        if(isShapeV(y)){
            if(y->isId && x->sizeId == y->sizeId) rtnShape = x;
            else if(!(y->isId) && y->size == 1) rtnShape = x;
            else rtnShape = newShapeNode(unknownH, true, -1);
        }
        else rtnShape = newShapeNode(unknownH, true, -1);
    }
    return rtnShape;
}

static ShapeNode *decideShapeElementwise(InfoNode *x, InfoNode *y){
    ShapeNode *rtnShape = NULL;
    if(isShapeV(x->shape) || isShapeV(y->shape)){
        rtnShape = decideShapeV(x->shape,y->shape);
    }
    else if(isShapeU(x->shape) || isShapeU(y->shape)){
        rtnShape = newShapeNode(unknownH, true, -1);
    }
    else error("unknown shape case for elementwise");
    return rtnShape;
}

static ShapeNode *decideShapeAppendV(ShapeNode *x, ShapeNode *y){
    if(x->isId || y->isId){
        return newShapeNode(vectorH, true, -1);
    }
    else {
        return newShapeNode(vectorH, false, x->size+y->size);
    }
}

static ShapeNode *decideShapeAppend(InfoNode *x, InfoNode *y){
    ShapeNode *rtnShape = NULL;
    if(isShapeV(x->shape) && isShapeV(y->shape)){
        rtnShape = decideShapeAppendV(x->shape, y->shape);
    }
    else if(isShapeU(x->shape) || isShapeU(y->shape)){
        rtnShape = newShapeNode(unknownH, true, -1);
    }
    else error("unknown shape case for append");
    return rtnShape;
}

static InfoNode *commonCompare2(InfoNode *x, InfoNode *y){
    pType rtnType; 
    if(isRealIN(x) && isRealIN(y)){
        rtnType = boolT;
    }
    else if(sameT(x,y) && isBasicIN(x)){
        rtnType = boolT;
    }
    else if(isU(x) || isU(y)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, decideShapeElementwise(x,y));
}

static InfoNode *propOrder(InfoNode *x, InfoNode *y){
    if(isListT(x) && isBoolIN(y)){
        return newInfoNode(i64T, newShapeNode(vectorH, true, -1));
    }
    else return NULL;
}

static InfoNode *propVector(InfoNode *x, InfoNode *y){
    if(isIntIN(x) && isBasicIN(y)){
        return newInfoNode(y->type, newShapeNode(vectorH, true, -1));
    }
    else return NULL;
}

// like(x,y)
static InfoNode *propLike(InfoNode *x, InfoNode *y){
    pType rtnType;
    if(isStringIN(x) && isStringIN(y)){
        rtnType = boolT;
    }
    else if(isU(x)||isU(y)) {
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, decideShapeElementwise(x,y));
}

// indexof(x,y)
static InfoNode *propIndexof(InfoNode *x, InfoNode *y){
    pType rtnType;
    if(sameT(x,y) && isBasicIN(x) && isBasicIN(y)){
        rtnType = i64T;
    }
    else if(isU(x)||isU(y)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, y->shape);
}

static InfoNode *propAppend(InfoNode *x, InfoNode *y){
    pType rtnType;
    if(isU(x) || isU(y)){
        rtnType = unknownT;
    }
    else if(isBasicIN(x) && isBasicIN(y)){
        rtnType = MAX(x->type, y->type);
    }
    else if(sameT(x,y)){
        rtnType = x->type;
    }
    else return NULL;
    return newInfoNode(rtnType, decideShapeAppend(x,y));
}

// sub_string(x, (1,2):i64)
//    --> can be more precise if literal values are known
static InfoNode *propSubString(InfoNode *x, InfoNode *y){
    pType rtnType;
    if(isStringIN(x) && isIntIN(y)){
        rtnType = x->type;
    }
    else if(isU(x) || isU(y)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, newShapeNode(vectorH, true, -1));
}


/* special */

static InfoNode *specialLoadTable(InfoNode *x){
    if(isStringIN(x)){
        return newInfoNode(tableT, newShapeNode(tableH,true,-1));
    }
    else return NULL;
}
static InfoNode *specialColumnValue(InfoNode *x, InfoNode *y){
    //P("type: column value\n"); printType(x->type); P(" "); printType(y->type); P("\n");
    ShapeNode *rtnShape = newShapeNode(vectorH, true, x->shape->sizeId);
    if(isTableIN(x) && isStringIN(y)){
        return newInfoNode(unknownT, rtnShape);
    }
    else if(isU(x) || isU(y)){
        return newInfoNode(unknownT, rtnShape);
    }
    else return NULL;
}

static InfoNode *specialCompress(InfoNode *x, InfoNode *y){
    pType rtnType; ShapeNode *rtnShape=y->shape;
    if(isBoolIN(x) && (isBasicIN(y)||isEnumT(y))){
        rtnType = y->type;
    }
    else if(isU(x) || isU(y)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(y->type, rtnShape);
}
static InfoNode *specialTable(InfoNode *x, InfoNode *y){
    ShapeNode *rtnShape = newShapeNode(tableH, true, -1);
    if(isStringIN(x) && isListT(y)){
        return newInfoNode(tableT, rtnShape);
    }
    else if(isU(x) || isU(y)){
        return newInfoNode(unknownT, rtnShape);
    }
    else return NULL;
}

static InfoNode *propList(InfoNodeList *in_list){
    /* get length */
    int k = 0; InfoNodeList *temp = in_list; pType t = -1;
    while(temp->next){
        k++; temp=temp->next;
        if(t==-1) t=temp->in->type;
        else if(t>0 && t!=temp->in->type) t=-2;
    }
    ShapeNode *rtnShape = newShapeNode(vectorH, false, k);
    return newInfoNodeList(listT, 0>t?unknownT:t, rtnShape);
}

static InfoNode *propKeyValues(InfoNode *x, bool f){
    if(isDictIN(x)){
        if(f) return newInfoNode(i64T , newShapeNode(vectorH, true, -1));
        else  return newInfoNodeList(listT, i64T, newShapeNode(listH  , true, -1));
    }
    else if(isEnumT(x)){ /* TODO: need to pass info to keys */
        if(f) return newInfoNode(unknownT, newShapeNode(unknownH, true, -1));
        else  return newInfoNode(    i64T, x->shape);
    }
    else EP("[propKeyValues] adding support for type %d\n", x->type);
}

static InfoNode *propValues(InfoNode *x){
    return propKeyValues(x, false);
}

static InfoNode *propKeys(InfoNode *x){
    return propKeyValues(x, true);
}

static InfoNode *propIndex(InfoNode *x, InfoNode *y){
    if(isIntIN(y)){
        return newInfoNode(x->type, y->shape);
    }
    else return NULL;
}

/* copy from typeshape.c, TODO: combine later */
static InfoNode *getNode(InfoNodeList *rt, int k){
    while(rt->next && k>=0){ rt=rt->next; k--; }
    return rt->in;
}

static InfoNode *propEachDya(InfoNodeList *in_list, int side){
    InfoNode *fn = getNode(in_list, 0);
    InfoNode *x  = getNode(in_list, 1);
    InfoNode *y  = getNode(in_list, 2);
    if(isFuncIN(fn)){ /* TODO: add more accurate rules */
        if(side == 0){
            if(isListT(x) && isListT(y))
                return newInfoNodeList(listT, x->subtype, x->shape);
            else return NULL;
        }
        else if(side == 1){
            if(isListT(x))
                return newInfoNodeList(listT, x->subtype, x->shape);
            else return NULL;
        }
        else if(side == 2){
            if(isListT(y))
                return newInfoNodeList(listT, y->subtype, y->shape);
            else return NULL;
        }
        else return NULL;
    }
    else {
        printInfoNode(fn);
        return NULL;
    }
}

static InfoNode *propEachItem(InfoNodeList *in_list){
    return propEachDya(in_list,0);
}
static InfoNode *propEachLeft(InfoNodeList *in_list){
    return propEachDya(in_list,1);
}
static InfoNode *propEachRight(InfoNodeList *in_list){
    return propEachDya(in_list,2);
}

static InfoNode *propEach(InfoNode *fn, InfoNode *x){
    if(isFuncIN(fn) && isListT(x)){ /* TODO: add more accurate rules */
        return newInfoNodeList(listT, x->subtype, x->shape);
    }
    else return NULL;
}

static InfoNode *propEnum(InfoNode *x, InfoNode *y){
    pType rtnType;
    if(sameT(x,y) && isBasicIN(x) && isBasicIN(y)){
        rtnType = enumT;
    }
    else if(isU(x) || isU(y)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, y->shape);
}

/* v[x] = y; */
static InfoNode *propIndexA(InfoNodeList *in_list){
    InfoNode *v = getNode(in_list, 0);
    InfoNode *x = getNode(in_list, 1);
    InfoNode *y = getNode(in_list, 2);
    if(sameT(v,y) && isIntIN(x) && sameH(x->shape,y->shape)){
        return v;
    }
    else if(isU(v)||isU(x)||isU(y)){
        return v;
    }
    else return NULL;
}

/* main */ 
static int findInBuiltinSet(char *funcName, const char *set[]){
    DOI(totalFunc, if(!strcmp(funcName, set[i])) return i)
    return -1;
}

int getFuncIndexByName(char *name){
    if(sizeof(FUNCTIONS)/8 != totalFunc){
        P("%d vs. %d\n", sizeof(FUNCTIONS)/8, totalFunc);
        error("FUNCTIONS and FunctionType should have the same # of elem.");
    }
    int k = findInBuiltinSet(name, FUNCTIONS);
    if(k>=0) return k;
    else {
        if(!strcmp(name, "le")) P("Do you mean 'leq' instead of 'le'?\n");
        else if(!strcmp(name, "ge")) P("Do you mean 'geq' instead of 'ge'?\n");
        else if(!strcmp(name, "sub")) P("Do you mean 'minus' instead of 'sub'?\n");
        EP("primitive not defined: %d, %s\n", k,name);
    }
}

void *fetchTypeRules(char *name, int* num){
    int k = getFuncIndexByName(name);
    if(k>=0){
        *num = getValence(k);
        //P("valence = %d, k = %d\n", *num,k);
        switch(k){
            /* monadic */
            CASE(    absF, ruleAbs)
            CASE(    negF, ruleNeg)
            CASE(   ceilF, ruleCeil)
            CASE(  floorF, ruleFloor)
            CASE(  roundF, ruleRound)
            CASE(   conjF, ruleConj)
            CASE(  recipF, ruleRecip)
            CASE( signumF, ruleSignum)
            CASE(     piF, rulePi)
            CASE(    notF, ruleNot)
            CASE(    logF, ruleLog)
            CASE(    expF, ruleExp)
            CASE(    cosF, ruleCos)
            CASE(    sinF, ruleSin)
            CASE(    tanF, ruleTan)
            CASE(   acosF, ruleAcos)
            CASE(   asinF, ruleAsin)
            CASE(   atanF, ruleAtan)
            CASE(   coshF, ruleCosh)
            CASE(   sinhF, ruleSinh)
            CASE(   tanhF, ruleTanh)
            CASE(  acoshF, ruleAcosh)
            CASE(  asinhF, ruleAsinh)
            CASE(  atanhF, ruleAtanh)
            CASE(   dateF, ruleDate)
            CASE(   yearF, ruleYear)
            CASE(  monthF, ruleMonth)
            CASE(   timeF, ruleTime)
            CASE(   hourF, ruleHour)
            CASE( minuteF, ruleMinute)
            CASE( secondF, ruleSecond)
            CASE(   millF, ruleMill)
            CASE( uniqueF, ruleUnique)
            CASE(    strF, ruleStr)
            CASE(    lenF, ruleLen)
            CASE(  rangeF, ruleRange)
            CASE(   factF, ruleFact)
            CASE(   randF, ruleRand)
            CASE(   seedF, ruleSeed)
            CASE(   flipF, ruleFlip)
            CASE(reverseF, ruleReverse)
            CASE(  whereF, ruleWhere)
            CASE(  groupF, ruleGroup)
            CASE(  countF, ruleCount)
            CASE(    sumF, ruleSum)
            CASE(    avgF, ruleAvg)
            CASE(    minF, ruleMin)
            CASE(    maxF, ruleMax)
            CASE(   razeF, ruleRaze)
            CASE( enlistF, ruleEnlist)
            CASE( tolistF, ruleTolist)
            CASE( formatF, ruleFormat)
            /* dyadic */
            CASE(      ltF, ruleLt)
            CASE(      gtF, ruleGt)
            CASE(     leqF, ruleLeq)
            CASE(     geqF, ruleGeq)
            CASE(      eqF, ruleEq)
            CASE(     neqF, ruleNeq)
            CASE(    plusF, rulePlus)
            CASE(   minusF, ruleMinus)
            CASE(     mulF, ruleMul)
            CASE(     divF, ruleDiv)
            CASE(   powerF, rulePower)
            CASE(    log2F, ruleLog2)
            CASE(     modF, ruleMod)
            CASE(     andF, ruleAnd)
            CASE(      orF, ruleOr)
            CASE(    nandF, ruleNand)
            CASE(     norF, ruleNor)
            CASE(     xorF, ruleXor)
            CASE(  dtdiffF, ruleDtdiff)
            CASE(   dtaddF, ruleDtadd)
            CASE(   dtsubF, ruleDtsub)
            CASE(  appendF, ruleAppend)
            CASE(    likeF, ruleLike)
            CASE(compressF, ruleCompress)
            CASE(   randkF, ruleRandk)
            CASE( indexofF, ruleIndexof)
            CASE(    takeF, ruleTake)
            CASE(    dropF, ruleDrop)
            CASE(   orderF, ruleOrder)
            CASE(  memberF, ruleMember)
            CASE(  vectorF, ruleVector)
            CASE(   matchF, ruleMatch)
            CASE(   indexF, ruleIndex)
            CASE(columnValueF, ruleColumnValue)
            CASE(  subStringF, ruleSubString)
            /* special */
            CASE(       eachF, ruleEach)
            CASE(   eachItemF, ruleEachItem)
            CASE(   eachLeftF, ruleEachLeft)
            CASE(  eachRightF, ruleEachRight)
            CASE(       enumF, ruleEnum)
            CASE(       dictF, ruleDict)
            CASE(      tableF, ruleTable)
            CASE(     ktableF, ruleKtable)
            CASE(       keysF, ruleKeys)
            CASE(     valuesF, ruleValues)
            CASE(       metaF, ruleMeta)
            CASE(  loadTableF, ruleLoadTable)
            CASE(      fetchF, ruleFetch)
            CASE(     indexAF, ruleIndexA)
            CASE(       listF, ruleList)
            default: EP("type rules not defined.: %s\n", name);
        }
    }
    else EP("primitive not defined: %s\n", name);
}

int findFuncIndex(char *funcName){
    return findInBuiltinSet(funcName, FUNCTIONS);
}

int getValence(pFunc k){
    /* debug: check, removable */
    if(fetchF+1 != ltF)  { EP("ltF must follow fetchF\n");  }
    if(subStringF+1 != eachF){ EP("eachF must follow subStringF\n"); }
    if(k>=0 && k<ltF) return 1;
    else if(k>=ltF && k<eachF) return 2;
    else {
        switch(k){
            case        eachF: return 2;
            case    eachItemF: return 3;
            case    eachLeftF: return 3;
            case   eachRightF: return 3;
            case        enumF: return 2;
            case        dictF: return 0;
            case       tableF: return 2;
            case      ktableF: return 0;
            case      indexAF: return 3;
            case        listF: return -1; //any
            default: EP("[getValence] Func (%d) not defined yet.\n",k);
        }
    }
}

