#ifndef __H_TRANSLATOR__
#define __H_TRANSLATOR__

int HorseInterpreter(Prog *rt);
int HorseCompiler(Prog *rt);
Node *findModule(List *modules, char *expected);
Node *findMethod(List *body, char *expected);
#endif
