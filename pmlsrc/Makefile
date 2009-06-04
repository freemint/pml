CROSSDIR = /dsrg/bammi/cross-minix
CROSSLIB = $(CROSSDIR)/lib
CROSSBIN = $(CROSSDIR)/bin
CROSSINC = $(CROSSDIR)/include

ALL = libpml.a libpml32.a

all : $(ALL)

libpml.a :
	make -f Makefile.16 clean
	make -f Makefile.16 libpml.a

libpml32.a :
	make -f Makefile.32 clean
	make -f Makefile.32 libpml32.a

install : $(ALL)
	mkdir -p $(CROSSLIB)
	cp libpml32.a $(CROSSLIB)/libm.a
	mkdir -p $(CROSSLIB)/mshort
	cp libpml.a $(CROSSLIB)/mshort/libm.a
	mkdir -p $(CROSSINC)
	cp math.h $(CROSSINC)

clean:
	make -f Makefile.16 clean
	make -f Makefile.32 clean
