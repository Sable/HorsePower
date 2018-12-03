all: debug

release:
	gcc -O3 -o testAll main.c \
	../backend/*.c ../util/*.c ../analysis/*.c ../global.c ../frontend/pretty.c \
	-I"../../../../libs/include/" ../../../../libs/lib/libpcre2-8.a \
	-DQALL -std=c99 -fopenmp -lm -lstdc++ -march=native

debug:
	gcc -g -o testAll main.c \
	../backend/*.c ../util/*.c ../analysis/*.c ../global.c ../frontend/pretty.c \
	-I"../../../../libs/include/" ../../../../libs/lib/libpcre2-8.a \
	-DQALL -std=c99 -fopenmp -lm -lstdc++ -march=native #-L/usr/local/lib/ -lprofiler 
 
