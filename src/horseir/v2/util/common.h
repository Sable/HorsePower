/*
 * Description:
 *  A centralized place for keeping utility information
 */

#ifndef __U_COMMON__
#define __U_COMMON__

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct horse_json{
    int kind, size;
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
    OPT_NA, OPT_SR, OPT_FE, OPT_FP, OPT_FA, OPT_ALL
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
Type getTypeFromV(V x);
Type getType(Node *x);  /* used in udchain */
const char *getExtraKind(GenKind x);
//const char *getpTypeName (pType x);
const char *getKindName(Kind x);
//const char *getpFuncName (pFunc x);
//int   getpTypeSize(pType x);
void  getInfoVar2 (V x, S name);
#define getInfoVar(x) getInfoVar2(x, #x)
#define showTime(s) P("[%-10s]> The elapsed time (ms): %g\n",s,calcInterval(tv0, tv1))

  HA newHorseArray();
  HA appendToHorseArray(HA x, L v);
   L countHorseArray(HA x);
void copyFromHorseArray(G g, HA x);
void indexWithHorseArray(V z, V y, HA x); // z=y[x]
void freeHorseArray(HA x);

/* dot.c */
//void dotProg(Prog *root);

/* options.c */
typedef enum OptionMode{
    UnknownM, InterpNaiveM, CompilerM, InterpJITM,
    VersionM, HelperM, ExperimentM, UtilityM
}OptionMode;

typedef enum {
    UnknownU, PrettyPrintU, DotU, StatsU
}OptionUtility;

int  getLongOption(int argc, char *argv[]);
void usage(int e);

const S obtainOptStr(OC x);

extern OptionMode optMode;    // program mode
extern OptionUtility optUtl;  // utility option
extern int   qRun;            // number of runs
extern char *qPath;           // file path
extern TC    qTarg;           // one target
extern OC    qOpts[99];       // list of optimizations
extern int   numOpts;         // number of optimizations
extern bool  qIsTpch;         // is tpch
extern int   qTpchId;         // if(qIsTpch) id
extern char *qStats;          // dump or load

#define GOOD_TRY() usage(0)
#define BAD_TRY()  usage(1)

/* stats.c */
void handleStats(S cmd);

/* json.c */
#define newJSONKeyValue(k,v)    newJSON0(k,v)
#define newJSONKeyVector(k,n,v) newJSON1(k,n,v)
#define newJSONKeyJSON(k,v)     newJSON2(k,v)
#define newJSONKeyList(k,n,v)   newJSON3(k,n,v)

void demoJSON();
void printJSON(JSON *x);
JSON *newJSON0(char *key, char *val);
JSON *newJSON1(char *key, int size, char **val);
JSON *newJSON2(char *key, JSON *val);
JSON *newJSON3(char *key, int size, JSON **val);
JSON *addJSONField(JSON *x, JSON *n);
JSON *initJSON();

/* simplehash */
sHashTable* initSimpleHash(L size);
O addToSimpleHash   (sHashTable *st, L key, L val);
L lookupSimpleHash  (sHashTable *st, L key);
O profileSimpleHash (sHashTable *st);

#ifdef  __cplusplus
}
#endif

#endif

