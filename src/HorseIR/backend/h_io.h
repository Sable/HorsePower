#pragma once

L readFile(S fileName, L op);
L readCSV(FILE* fp);
L getField(S line, C sp);
FILE* openFile(S s);
