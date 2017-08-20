PROJECT_NAME=UCORE-2017

pdf: $(PROJECT_NAME).tex $(PROJECT_NAME).bib
	pdflatex $(PROJECT_NAME).tex > /dev/null
	bibtex $(PROJECT_NAME) > /dev/null
	pdflatex $(PROJECT_NAME).tex > /dev/null
	pdflatex $(PROJECT_NAME).tex > /dev/null

.PHONY:
clean:
	rm -f *.aux *.bbl *.blg *.log *.out *.pdf
