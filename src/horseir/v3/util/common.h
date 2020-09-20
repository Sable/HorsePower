/*
 * Description:
 *  A centralized place for keeping utility information
 */

#ifndef __U_COMMON__
#define __U_COMMON__

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum JsonKind{
    JSON_VALUE, JSON_VALUES, JSON_JSON, JSON_JSONS
}JsonKind;

typedef struct horse_json{
    JsonKind kind;
    int size;
    char *key;
    union{
        char *field;
        char **field_list;
        struct horse_json *child;
        struct horse_json **child_list;
    };
    struct horse_json *next;
}JSON;

typedef struct simple_hash_node{
    L key, val;
    struct simple_hash_node *next;
}sHashNode;

typedef struct simple_hash_table{
    sHashNode **table;
    L size;
}sHashTable;

typedef enum OptCode{
    OPT_NA, OPT_FE, OPT_FP, OPT_FA, OPT_BASIC, OPT_ALL, OPT_FP1, OPT_FP2,
    OPT_FP_DLS18, OPT_IL, OPT_SL
}OC;

#define percent(a,b) ((a)*100.0/(b))

///* profile.c */
//I testFunc1        ();
//I profileV         (V x);
//
///* trie.c */
//void printTypeShape(); 
//
///* mics.c */
//E calcInterval(struct timeval t0, struct timeval t1); 
//
///* nametable.c */
//void saveToNameTable(char *str, V val);
//V getValueFromNameTable(char *str);
//
///* /connect.c */
//int runModeServer();
//int runModeClient();

/* info.c */
const char *getTypeName(I x);
HorseType getTypeFromV(V x);
HorseType getType(Node *x);  /* used in udchain */
const char *getExtraKind(GenKind x);
//const char *getpTypeName (pType x);
const char *getKindName(Kind x);
//const char *getpFuncName (pFunc x);
//int   getpTypeSize(pType x);
void  getInfoVar2 (V x, const char *name);
#define getInfoVar(x) getInfoVar2(x, #x)

HA newHorseArray();
HA appendToHorseArray(HA x, L v);
L  countHorseArray(HA x);
O  copyFromHorseArray(G g, HA x);
O  indexWithHorseArray(V z, V y, HA x); // z=y[x]
O  freeHorseArray(HA x);

/* dot.c */
O  dotProg();
O  dotProgMermaid();

/* options.c */
typedef enum OptionMode{
    UnknownM, InterpNaiveM, CompilerM, VersionM, HelperM, UtilityM
}OptionMode;

typedef enum {
    UnknownU, StatsU, PrinterU
}OptionUtility;

int  getLongOption(int argc, char *argv[]);
void usage(int e);

const char *obtainOptStr(OC x);

extern OptionMode optMode;    // program mode
extern OptionUtility optUtl;  // utility option
extern I  qRun;      // number of runs
extern S  qPath;     // file path
extern TC qTarg;     // one target
extern OC qOpts[99]; // list of optimizations
extern I  numOpts;   // number of optimizations
extern B  qIsTpch;   // is tpch
extern I  qTpchId;   // if(qIsTpch) id
extern S  qCmd;      // dump or load

#define GOOD_TRY() usage(0)
#define BAD_TRY()  usage(1)

/* stats.c */
O loadStats();
O dumpStats();

/* json.c */
#define newJSONKeyValue(k,v)    newJSON0(k,v)
#define newJSONKeyVector(k,n,v) newJSON1(k,n,v)
#define newJSONKeyJSON(k,v)     newJSON2(k,v)
#define newJSONKeyList(k,n,v)   newJSON3(k,n,v)

O demoJSON();
O printJSON(JSON *x);

JSON *newJSON0(const char *key, const char *val);
JSON *newJSON1(const char *key, int size, const char **val);
JSON *newJSON2(const char *key, JSON *val);
JSON *newJSON3(const char *key, int size, JSON **val);
JSON *addJSONField(JSON *x, JSON *n);
JSON *initJSON();

/* simplehash */
sHashTable* initSimpleHash(L size);
O addToSimpleHash   (sHashTable *st, L key, L val);
L lookupSimpleHash  (sHashTable *st, L key);
O profileSimpleHash (sHashTable *st);
O freeSimpleHash    (sHashTable *st);

O probeBooleanVector(V x);

#ifdef  __cplusplus
}
#endif

#endif

