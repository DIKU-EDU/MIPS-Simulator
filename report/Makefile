OUTDIR    = $(CURDIR)/output
TARGET    = $(OUTDIR)/report.pdf
SRC       = main
COMPILE   = pdflatex -output-directory=$(OUTDIR) -file-line-error $(SRC)

default: view
all:
	-mkdir -p $(OUTDIR)
	$(COMPILE)
	bibtex $(SRC)
	$(COMPILE)
	$(COMPILE)

view: all
	evince $(OUTDIR)/main.pdf

clean:
	rm -r $(OUTDIR)
