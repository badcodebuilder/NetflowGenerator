.PHONY: all prog clean

all: prog move

prog:
	$(MAKE) -C src

move:
	mkdir -p bin/
	mv -f src/cli bin/
	mv -f src/srv bin/

clean:
	rm -rf bin/*