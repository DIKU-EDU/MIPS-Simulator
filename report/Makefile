OUTDIR    = $(CURDIR)
SRC       = main
SRCS	  = $(wildcard *.tex */.tex)
CFLAGS	  = -file-line-error

default: view
all: clean
	pdflatex $(SRC) $(CFLAGS)
	bibtex $(SRC)
	pdflatex $(SRC) $(CFLAGS)
	pdflatex $(SRC) $(CFLAGS)


view: all
	evince $(OUTDIR)/main.pdf

clean:
	-rm -rvf *.pdf *.log *.toc *.bbl *.aux *.blg *.out
