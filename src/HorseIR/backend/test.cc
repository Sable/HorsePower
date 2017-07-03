#include "h_global.h"

L testInputFile(S filePath){
	if(!filePath){
		fprintf(stderr, "Correct path must be set (-p path)\n");
		exit(99);
	}
	P("path = %s\n", filePath);
	readFile(filePath, 1);
	R 0;
}

