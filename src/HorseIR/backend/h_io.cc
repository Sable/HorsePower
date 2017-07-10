#include "h_global.h"

const L LINE_MAX_CHAR = 1024;
const C LINE_SEP      = '|';

/*
 * 1: csv
 * 2: txt (optional)
 */
// L readFile(S fileName, L op){
// 	FILE *fp = openFile(fileName);
// 	L status = 1;
// 	switch(op){
// 		case 1: status = readCSV(fp); break;		
// 	}
// 	fclose(fp);
// 	R status;
// }

V readCSV(S fileName, L numCols, L *types, L *symList){
	FILE *fp = openFile(fileName);
	L numRow = loadCSV(fp, false, NULL, numCols, NULL);
	V x = allocTable(numCols);
	DOI(numCols, {V newDict=xV(i); \
		initDict(newDict); \
		initSymbol(VDexl(newDict, 0), symList[i]); \
		initValue (VDexl(newDict, 1), types[i], numRow);});

	P("** Done with initialization **\n");
	printTable(x); P("\n");
	rewind(fp);
	loadCSV(fp, true, x, numCols, types);
	fclose(fp);
	R x;
}

L loadCSV(FILE *fp, B isLoading, V table, L numCols, L *types){
	C line[LINE_MAX_CHAR];
	L rowSize = 0, rowID = 0;
	L errCode = 0;
	while(fgets(line, LINE_MAX_CHAR, fp)){
		if(STRING_NONEMPTY(line)){
			if(isLoading){
				getField(line, LINE_SEP, table, rowSize, types, &errCode);
			}
			rowSize++;
		}
		rowID++;
	}
	if(errCode != 0){
		fprintf(stderr, "readCSV error at line %lld\n", rowID);
		exit(ERROR_CODE);
	}
	if(!isLoading)
		P("CSV info: %lld rows and %lld cols\n", rowSize, numCols);
	R rowSize;
}

L getField(S line, C sp, V x, L rowID, L *types, L *errCode){
	C tmp[LINE_MAX_CHAR];
	S lineT = trim(line);
	L num = 0, numCols = 0, strLen = strlen(lineT);
	P("line[%lld] = %s", rowID,lineT); printType(xp); P("\n");
	DOI(strLen, {\
		if(sp==lineT[i]){\
			tmp[num]=0;P("%s [col %lld]\n",tmp, numCols);\
			P(" entering\n");\
			loadItem(getDictVal(getTableDict(x,numCols)),rowID,types[numCols],tmp);\
			num=0;numCols++;}\
		else{tmp[num++]=lineT[i];}})
	R numCols;
}

FILE* openFile(S s){
	FILE* fp = fopen(s, "r");
	if(NULL == fp){
		fprintf(stderr, "%s\n", strerror(errno));
		exit(ERROR_CODE);
	}
	R fp;
}

/* x[k] = (typ) s */
void loadItem(V x, L k, L typ, S s){
	P("loadItem, ");
	printType(xp);
	P(" typ = %lld, k = %lld, xg = 0\n",typ,k);
	// switch(typ){
	// 	caseB xB(k) = atoi(s); break;
	// 	caseI xI(k) = atoi(s); break;
	// 	caseL xL(k) = atol(s); break;
	// 	caseS xS(k) = insertSym(createSymbol(s)); break;
	// }
	P("xS(%lld) = %lld\n",k,xS(k));
	getchar();
	getchar();
	getchar();
}

/* helper functions */

#define SKIP(x,a) ((x)==(a))
#define SKIP_SET(c) (SKIP(c,' ') || SKIP(c, '\r') || SKIP(c, '\n'))

S trim(S s){
	R trimRight(trimLeft(s));
}

S trimLeft(S s){
	L k=0; while(s[k]==' ')k++; R s+k;
}

S trimRight(S s){
	S t = s;
	if(STRING_NONEMPTY(s)){
		t = s + strlen(s) - 1;
		while(t!=s && SKIP_SET(*t)) t--;
		*(t+(t!=s))=0;
	}
	R s;
}

void errorMsg(S msg){
	fprintf(stderr, "%s\n", msg);
	exit(ERROR_CODE);
}

/* output */

void printItem(V x, S strBuff){
	switch(xp){
		caseB SP(strBuff, "%d"  , xb); break;
		caseI SP(strBuff, "%d"  , xi); break;
		caseL SP(strBuff, "%lld", xl); break;
		caseE SP(strBuff, "%lf" , xe); break;
		caseS SP(strBuff, "%lld", xs); break;
	}
}

void printListItem(V x, L k, S strBuff){
	switch(xp){
		caseB SP(strBuff, "%d"  , xB(k)); break;
		caseI SP(strBuff, "%d"  , xI(k)); break;
		caseL SP(strBuff, "%lld", xL(k)); break;
		caseE SP(strBuff, "%lf" , xF(k)); break;
		caseS SP(strBuff, "%lld", xS(k)); break;
		caseA DOI(xn, {printListItem(xG(i),i,strBuff);}) return;
	}
	P("%s ", strBuff);
}

void printHead(V x){
	P("{["); printType(xp); P("]");
}

void printList(V x){
	C buff[128];
	printHead(x);
	// DOI(xn, printListItem(x,i,buff));
	P("}");
}

void printDict(V x){
	if(isN()){
		printHead(x);
		printList(xV(0));
		P(",");
		printList(xV(1));
		P("}");
	}
	else{
		P("<Not a dictionary>");
	}
}

void printTable(V x){
	if(isA()){
		printHead(x);
		DOI(xn, {if(i>0)P(","); printDict(xV(i));})
		P("}");
	}
	else {
		P("<Not a table>");
	}
}

