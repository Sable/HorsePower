#include "../global.h"
/* check typeshape.h */

/* constants */
const char *FUNCTIONS[] = {
    /* unary 53 */
    "abs", "neg", "ceil", "floor", "round", "conj", "recip", "signum", "pi" , "not",
    "log", "exp",  "cos",   "sin",   "tan", "acos",  "asin",  "atan", "cosh", "sinh",
    "tanh", "acosh", "asinh", "atanh",
    "date", "date_year", "date_month", "date_day",
    "time", "time_hour", "time_minute", "time_second", "time_mill",
    "unique", "str", "len", "range", "fact", "rand", "seed", "flip", "reverse",
    "where", "group", "count", "sum", "avg", "min", "max", "raze", "enlist", "tolist",
    "format",
    /* binary 32 */
    "lt" ,  "gt", "leq" , "geq"  , "eq", "neq" , "plus", "minus" , "mul", "div",
    "power", "log2", "mod", "and", "or", "nand", "nor" , "xor",
    "datetime_diff", "datetime_add", "datetime_sub",
    "append", "like", "compress", "randk", "index_of", "take", "drop", "order",
    "member", "vector", "match",
    /* special 16 */
    "each", "each_item", "each_left", "each_right", "enum", "dict", "table",
    "ktable", "keys", "values", "meta", "column_value", "load_table", "fetch",
    "index", "index_a"
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
#define ruleLen     NULL
#define ruleRange   NULL
#define ruleFact    NULL
#define ruleRand    NULL
#define ruleSeed    NULL
#define ruleFlip    NULL
#define ruleReverse NULL
#define ruleWhere   NULL
#define ruleGroup   NULL
#define ruleCount   reductionCount
#define ruleSum     reductionSum
#define ruleAvg     NULL
#define ruleMin     NULL
#define ruleMax     NULL
#define ruleRaze    NULL
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
#define ruleAppend   NULL
#define ruleLike     NULL
#define ruleCompress specialCompress
#define ruleRandk    NULL
#define ruleIndexof  NULL
#define ruleTake     NULL
#define ruleDrop     NULL
#define ruleOrder    NULL
#define ruleMember   NULL
#define ruleVector   NULL
#define ruleMatch    NULL
/* special */ 
#define ruleEach        NULL
#define ruleEachItem    NULL
#define ruleEachLeft    NULL
#define ruleEachRight   NULL
#define ruleEnum        NULL
#define ruleDict        NULL
#define ruleTable       specialTable
#define ruleKtable      NULL
#define ruleKeys        NULL
#define ruleValues      NULL
#define ruleMeta        NULL
#define ruleColumnValue specialColumnValue
#define ruleLoadTable   specialLoadTable
#define ruleFetch       NULL
#define ruleIndex       NULL
#define ruleIndexA      NULL

#define isT(t) (t==n->type)

#define isBT isT(boolT)
#define isIT isT(i64T)||isT(i32T)||isT(i16T)||isT(i8T)
#define isFT isT(f64T)||isT(f32T)
#define isST isT(symT)||isT(strT)
#define isDT isT(monthT)||isT(dateT)
#define isTT isT(tableT)||isT(ktableT)
#define sameT(x,y) ((x)->type==(y)->type)

bool isInt   (InfoNode *n) {return isIT;}
bool isFloat (InfoNode *n) {return isFT;}
bool isBool  (InfoNode *n) {return isBT;}
bool isReal  (InfoNode *n) {return isIT||isFT||isBT;}
bool isString(InfoNode *n) {return isST;}
bool isDate  (InfoNode *n) {return isDT;}
bool isBasic (InfoNode *n) {return isReal(n)||isDate(n);}
bool isTable (InfoNode *n) {return isTT;}
#define isU(n) (unknownT==(n)->type)
#define isList(n) (listT==(n)->type)

#define isS(n, t) (t == n->type)
#define isShapeU(n) isS(n, unknownH)
#define isShapeV(n) isS(n,  vectorH)
#define isShapeL(n) isS(n,    listH)
#define isShapeT(n) isS(n,   tableH)

typedef bool (*TypeCond)(InfoNode *);
int shapeId = 0;

static InfoNode *newInfoNode(pType type, ShapeNode *shape){
    InfoNode *in = NEW(InfoNode);
    in->type     = type;
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
    return commonElemementSingle(x, &isReal, x->type);
} 

static InfoNode *commonBool1(InfoNode *x){
    return commonElemementSingle(x, &isBool, boolT);
}

static InfoNode *reductionSum(InfoNode *x){
    pType rtnType;
    if(isInt(x)||isBool(x)) rtnType = i64T;
    else if(isFloat(x)) rtnType = f64T;
    else if(isU(x)) rtnType = unknownT;
    else return NULL;
    return newInfoNode(rtnType, newShapeNode(vectorH, false, 1));
}

static InfoNode *reductionCount(InfoNode *x){
    return commonReduction(x, &isBasic, i64T);
}

static InfoNode *specialEnlist(InfoNode *x){
    return commonReduction(x, &isBasic, listT);
}

static InfoNode *specialUnique(InfoNode *x){
    return commonElemementSingle(x, &isBasic, i64T);
}

/* dyadic */
static InfoNode *commonArith2(InfoNode *x, InfoNode *y){
    pType rtnType;
    if(isReal(x)&&isReal(x)){
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
    if(isBool(x)&&isBool(y)){
        rtnType = boolT;
    }
    else if(isU(x) || isU(y)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, decideShapeElementwise(x,y));
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

static InfoNode *commonCompare2(InfoNode *x, InfoNode *y){
    pType rtnType; 
    if(isReal(x) && isReal(y)){
        rtnType = boolT;
    }
    else if(isDate(x) && sameT(x,y)){
        rtnType = boolT;
    }
    else if(isU(x) || isU(y)){
        rtnType = unknownT;
    }
    else return NULL;
    return newInfoNode(rtnType, decideShapeElementwise(x,y));
}

/* special */

static InfoNode *specialLoadTable(InfoNode *x){
    if(isString(x)){
        return newInfoNode(tableT, newShapeNode(tableH,true,-1));
    }
    else return NULL;
}
static InfoNode *specialColumnValue(InfoNode *x, InfoNode *y){
    //P("type: column value\n"); printType(x->type); P(" "); printType(y->type); P("\n");
    ShapeNode *rtnShape = newShapeNode(vectorH, true, x->shape->sizeId);
    if(isTable(x) && isString(y)){
        return newInfoNode(unknownT, rtnShape);
    }
    else if(isU(x) || isU(y)){
        return newInfoNode(unknownT, rtnShape);
    }
    else return NULL;
}

static InfoNode *specialCompress(InfoNode *x, InfoNode *y){
    pType rtnType; ShapeNode *rtnShape=y->shape;
    if(isBool(x) && (isBasic(y)||isU(y))){
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
    if(isString(x) && isList(y)){
        return newInfoNode(tableT, rtnShape);
    }
    else if(isU(x) || isU(y)){
        return newInfoNode(unknownT, rtnShape);
    }
    else return NULL;
}

/* main */ 
static int findInBuiltinSet(char *funcName, const char *set[]){
    DOI(totalFunc, if(!strcmp(funcName, set[i])) return i)
    return -1;
}

void *fetchTypeRules(char *name, int* num){
    if(sizeof(FUNCTIONS)/8 != totalFunc){
        P("%d vs. %d\n", sizeof(FUNCTIONS)/8, totalFunc);
        error("FUNCTIONS and FunctionType should have the same # of elem.");
    }
    int k = findInBuiltinSet(name, FUNCTIONS);
    if(k>=0){
        *num = getValence(k);
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
            CASE(columnValueF, ruleColumnValue)
            CASE(  loadTableF, ruleLoadTable)
            CASE(      fetchF, ruleFetch)
            CASE(      indexF, ruleIndex)
            CASE(     indexAF, ruleIndexA)
            default: error("type rules not defined.");
        }
    }
    else error("primitive not defined");
}

int findFuncIndex(char *funcName){
    return findInBuiltinSet(funcName, FUNCTIONS);
}

int getValence(pFunc k){
    /* debug: check, removable */
    if(formatF+1 != ltF)  { EP("ltF must follow formatF");  }
    if( matchF+1 != eachF){ EP("eachF must follow matchF"); }
    if(k<=formatF) return 1;
    else if(k>=ltF && k<=matchF) return 2;
    else {
        switch(k){
            case        eachF: return 0;
            case    eachItemF: return 0;
            case    eachLeftF: return 0;
            case   eachRightF: return 0;
            case        enumF: return 0;
            case        dictF: return 0;
            case       tableF: return 2;
            case      ktableF: return 0;
            case        keysF: return 0;
            case      valuesF: return 0;
            case        metaF: return 0;
            case columnValueF: return 2;
            case   loadTableF: return 1;
            case       fetchF: return 0;
            case       indexF: return 0;
            case      indexAF: return 0;
            default: EP("Func (%d) not defined yet.",k);
        }
    }
}

