CC=gcc
CFLAGS=-I. -m32

ODIR=build
BDIR=bin

_OBJ = generator.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

generatormake: dirs generator
	
generator: $(OBJ)
	gcc -o $(BDIR)/$@ $^ $(CFLAGS)

dirs:
	mkdir -p $(ODIR) ; mkdir -p $(BDIR)

clean:
	rm -f $(ODIR)/*.o $(BDIR)/*
