OUTDIR    = $(CURDIR)
SRC       = main
SRCS	  = $(wildcard *.tex */.tex)

default: view
all:
	pdflatex $(SRC)
	bibtex $(SRC)
	pdflatex $(SRC)
	pdflatex $(SRC)

view: all
	evince $(OUTDIR)/main.pdf

clean:
	rm -r $(OUTDIR)
