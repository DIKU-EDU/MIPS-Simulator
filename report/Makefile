OUTDIR    = $(CURDIR)/output
TARGET    = $(OUTDIR)/report.pdf
SRC       = $(wildcard *.tex)
COMPILE   = pdflatex -output-directory=$(OUTDIR) -file-line-error $(SRC)

default: all
all:
	-mkdir -p $(OUTDIR)
	$(COMPILE) && $(COMPILE)

view: all
	xpdf $(OUTDIR)/main.pdf

clean:
	rm -r $(OUTDIR)
