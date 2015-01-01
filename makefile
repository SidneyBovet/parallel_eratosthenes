CC=gcc
MPIC=mpicc
CFLAGS=-std=c99 -lm -Wall -I.


all:
	make seq && make par && make stats
seq:
	cd sequential && \
	$(CC) -o ../bin/seq seq.c $(CFLAGS)
par:
	cd parallel && \
	$(MPIC) -o ../bin/par par.c $(CFLAGS)
stats:
	cd netstats && \
	$(MPIC) -o ../bin/netstats netstats.c $(CFLAGS)
