.PHONY: all main clean spell compare

latex=pdflatex 

all: main

main:
	$(latex) -halt-on-error $@
	bibtex $@
	$(latex) -halt-on-error $@ 
	$(latex) -halt-on-error $@ 
	$(latex) -halt-on-error $@ 

build:
	$(latex) -halt-on-error main

spell:
	# warning: ok-words will be overwritten
	detex main.tex | aspell -a | env LANG=C sort -u > ok-words

compare:
	sh /Users/wukefe/software/aspell/dicts/setup-proposal.sh
	detex main.tex | aspell -a | env LANG=C sort -u | env LANG=C comm -23 - ok-words

view:
	open main.pdf

clean:
	rm -f *.aux
	rm -f *.bbl
	rm -f *.blg
	rm -f *.log
	rm -f *.pdf
	rm -f *.lof
	rm -f *.lot
	rm -f *.toc
	rm -f *~

