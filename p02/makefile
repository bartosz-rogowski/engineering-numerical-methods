CC=gcc
CFLAGS= -O2 -lm -lgsl -lgslcblas
DEPS=
OBJS=main.o
OUT=main
GNUPLOT=gnuplot_2.sh

$(OUT): $(OBJS)
	$(CC)  -o $@  $^ $(CFLAGS)
	
.c.o:
	$(CC) -c -o $@ $< $(CFLAGS) 
	
.PHONY: clean	
clean:
	rm -f *o
	rm -f *~
	rm -f $(OUT)
	
run:
	./main

mem:
	valgrind ./main

graph:
	gnuplot $(GNUPLOT)

erase:
	rm -f *o
	rm -f *~
	rm -f $(OUT)
	rm -f *dat
	rm -f *txt