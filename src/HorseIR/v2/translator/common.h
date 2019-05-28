#ifndef __H_TRANSLATOR__
#define __H_TRANSLATOR__

/* declaration */

/* Interpreter */
I HorseInterpreter(Prog *rt);
O runInterpreter();

/* Compiler */
I HorseCompiler(Prog *rt);

/* Interpreter v2 */
I HorseInterpreter2(Prog *rt);


/* helper functions */
#define totalVar totalElement
I getHType(Type t);
O loadConst(Node *n, V x, L k, I t);

#endif
