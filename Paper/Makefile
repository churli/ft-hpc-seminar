# Makefile per CV+MotivationLetter

# vars
BASEDIR=`pwd`
MANDIR=$(BASEDIR)
OUTDIR=$(MANDIR)/Out

TEXOPTS=-output-directory=$(OUTDIR)
BIBENVS=BIBINPUTS=$(MANDIR) TEXMFOUTPUT=$(OUTDIR)
AUTOPTS=-interaction nonstopmode

ITEM=ft-hpc-paper
# end vars

.PHONY: compile bibcompile oneside-compile oneside-bibcompile

default: bibcompile

init:
	mkdir -p $(OUTDIR)

compile: init
	pdflatex $(TEXOPTS) $(MANDIR)/$(ITEM).tex

bibcompile: init
	pdflatex $(TEXOPTS) $(MANDIR)/$(ITEM).tex
	$(BIBENVS) bibtex $(OUTDIR)/$(ITEM).aux
	pdflatex $(TEXOPTS) $(MANDIR)/$(ITEM).tex
	pdflatex $(TEXOPTS) $(MANDIR)/$(ITEM).tex

compileauto:
	pdflatex $(AUTOPTS) $(TEXOPTS) $(MANDIR)/$(ITEM).tex

bibcompileauto:
	pdflatex $(AUTOPTS) $(TEXOPTS) $(MANDIR)/$(ITEM).tex
	$(BIBENVS) bibtex $(OUTDIR)/$(ITEM).aux
	pdflatex $(AUTOPTS) $(TEXOPTS) $(MANDIR)/$(ITEM).tex
	pdflatex $(AUTOPTS) $(TEXOPTS) $(MANDIR)/$(ITEM).tex

autocompile: init
	./autocompile.loop.sh $(ITEM).pdf compileauto bibcompileauto

clean:
	echo "Cleaning outfiles..."
	rm $(OUTDIR)/*

#EOF
