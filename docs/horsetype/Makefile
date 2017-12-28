paper=inference
.PHONY: all paper clean

all: paper

paper:
	pdflatex -halt-on-error $(paper)
	#bibtex $(paper)
	#pdflatex -halt-on-error $(paper)
	#pdflatex -halt-on-error $(paper)
	#pdflatex -halt-on-error $(paper)

build:
	pdflatex -halt-on-error $(paper)

view:
	open $(paper).pdf

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
