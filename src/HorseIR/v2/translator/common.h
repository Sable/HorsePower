#ifndef __H_TRANSLATOR__
#define __H_TRANSLATOR__

/* declaration */

/* Interpreter */
I HorseInterpreter();
O runInterpreter();

/* Compiler */
I HorseCompiler();

/* Interpreter v2 */
I HorseInterpreter2();


/* helper functions */
#define totalVar totalElement
I getHType(Type t);
O loadConst(Node *n, V x, L k, I t);

#endif
