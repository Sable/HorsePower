#include "h_global.h"

const L LINE_MAX_CHAR = 1024;
const C LINE_SEP      = '|';

/*
 * 1: csv
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
	while(fgets(line, LINE_MAX_CHAR, fp)){
		getField(line, LINE_SEP);
	}
	R 0;
}

L getField(S line, C sp){
	C tmp[LINE_MAX_CHAR];
	L num = 0, tot = 0, size = strlen(line);
	DOI(size, {\
		if(sp==line[num]){tmp[num]=0;P(tmp);num=0;tot++}\
		else{tmp[num++]=line[i]}})
	P("total = %d\n",tot);
	R 0;
}

FILE* openFile(S s){
	FILE* fp = fopen(s, "r");
	if(NULL == fp){
		fprintf(stderr, "%s\n", strerror(errno));
		exit(ERROR_CODE);
	}
	R fp;
}
