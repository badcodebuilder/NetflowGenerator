.PHONY: all clean

all: main.tex img/sine.png img/square.png
	latexmk -pdf main.tex -quiet
	@echo "Cleaning temp files ..."
	@latexmk -c -quiet
	@echo "Done!"

clean:
	@echo "Cleaning temp and pdf files ..."
	@latexmk -c -quiet
	@rm main.pdf
	@echo "Done!"
