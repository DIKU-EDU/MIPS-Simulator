default: all

all: synopsis.tex
	pdflatex synopsis.tex & pdflatex synopsis.tex

view:
	xpdf synopsis.pdf

clean:
	-rm -vf *.aux *.out *.log *.synctex.gz
