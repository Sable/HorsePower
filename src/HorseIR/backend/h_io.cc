#include "h_global.h"

const L LINE_MAX_CHAR = 1024;
const C LINE_SEP      = '|';

/*
 * 1: csv
 * 2: txt (optional)
 */
L readFile(S fileName, L op){
	FILE *fp = openFile(fileName);
	L status = 1;
	switch(op){
		case 1: status = readCSV(fp); break;		
	}
	fclose(fp);
	R status;
}

L readCSV(FILE* fp){
	C line[LINE_MAX_CHAR];
	L rowSize = 0, rowID = 0, colSize = -1;
	B isError = false;
	while(fgets(line, LINE_MAX_CHAR, fp)){
		if(STRING_NONEMPTY(line)){
			L numCols = getField(line, LINE_SEP);
			if(colSize < 0) colSize = numCols;
			else if(colSize != numCols) {
				isError = true;
				break;
			}
			rowSize++;
		}
		rowID++;
	}
	if(isError){
		fprintf(stderr, "readCSV error at line %lld\n", rowID);
		exit(ERROR_CODE);
	}
	P("Successfully loaded with %lld rows and %lld cols\n", rowSize++, colSize);
	R 0;
}

L getField(S line, C sp){
	C tmp[LINE_MAX_CHAR];
	S lineT = trim(line);
	L num = 0, numCols = 0, size = strlen(lineT);
	P("line = %s\n", lineT);
	DOI(size, {\
		if(sp==lineT[i]){tmp[num]=0;P("%s\n",tmp);num=0;numCols++;}\
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

/* helper functions */

#define SKIP(x,a) ((x)==(a))
#define SKIP_SET(c) (SKIP(c,' ') && SKIP(c, '\r') && SKIP(c, '\n'))

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
		*t=0;
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

void printList(V x){
	C buff[128];
	P("{");
	DOI(xn, printListItem(x,i,buff));
	P("}\n");
}

