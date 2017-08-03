figures=Fig_Intersection.eps Fig_Type1.eps Fig_Type2.eps Fig_TypeJoin.eps logo-dragon.eps
project = HorseIRTypeSlides


all : $(figures) $(project).tex
	@echo "latex->dvips->ps2pdf"
	@latex "$(project).tex" > /dev/null 2>&1
	@dvips "$(project).dvi" > /dev/null 2>&1
	@ps2pdf "$(project).ps" > /dev/null 2>&1

.PHONY:
clean_temp :
	rm -f $(project).aux
	rm -f $(project).dvi
	rm -f $(project).log
	rm -f $(project).nav
	rm -f $(project).out
	rm -f $(project).ps
	rm -f $(project).snm
	rm -f $(project).toc
	rm -f $(project).vrb

.PHONY:
clean: clean_temp
	rm -f $(project).pdf
