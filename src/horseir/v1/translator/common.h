#ifndef __H_TRANSLATOR__
#define __H_TRANSLATOR__

I HorseInterpreter(Prog *rt);
I HorseCompiler(ChainList *rt);
Node *findModule(List *modules, char *expected);
Node *findMethod(List *body, char *expected);
void getRealFuncName(S func, S x);

#endif
