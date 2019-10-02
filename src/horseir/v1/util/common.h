/*
 * Description:
 *  A centralized place for keeping utility information
 */

#ifndef __U_COMMON__
#define __U_COMMON__

#ifdef  __cplusplus
extern "C" {
#endif

#define percent(a,b) ((a)*100.0/(b))

/* profile.c */
I testFunc1        ();
I profileV         (V x);

/* trie.c */
void printTypeShape(); 

/* mics.c */
E calcInterval(struct timeval t0, struct timeval t1); 

/* nametable.c */
void saveToNameTable(char *str, V val);
V getValueFromNameTable(char *str);

/* /connect.c */
int runModeServer();
int runModeClient();

/* info.c */
const char *getTypeName  (I x);
const char *getpTypeName (pType x);
const char *getKindName  (Kind x);
const char *getpFuncName (pFunc x);
int   getpTypeSize(pType x);
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
void dotProg(Prog *root);


#ifdef  __cplusplus
}
#endif

#endif

