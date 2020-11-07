.PHONY: all clean

all: cli srv

cli: cli.c config.h
	gcc cli.c -o cli -lpthread -lm

srv: srv.c config.h
	gcc srv.c -o srv -lpthread -lhiredis

clean:
	rm -f cli srv