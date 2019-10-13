CROSSDIR = /dsrg/bammi/cross-minix
CROSSLIB = $(CROSSDIR)/lib
CROSSBIN = $(CROSSDIR)/bin
CROSSINC = $(CROSSDIR)/include
WITH_SHORT_LIBS = 0

ALL = libpml32.a

ifeq ($(WITH_SHORT_LIBS), 1)
ALL += libpml.a
endif

all : $(ALL)

libpml32.a :
	make -f Makefile.32 clean
	make -f Makefile.32 libpml32.a

ifeq ($(WITH_SHORT_LIBS), 1)
libpml.a :
	make -f Makefile.16 clean
	make -f Makefile.16 libpml.a
endif

install : $(ALL)
	mkdir -p $(CROSSLIB)
	cp libpml32.a $(CROSSLIB)/libm.a
ifeq ($(WITH_SHORT_LIBS), 1)
	mkdir -p $(CROSSLIB)/mshort
	cp libpml.a $(CROSSLIB)/mshort/libm.a
endif
	mkdir -p $(CROSSINC)
	cp math.h $(CROSSINC)
	mkdir -p $(CROSSINC)/bits
	cp bits/nan.h bits/huge_val.h bits/mathdef.h $(CROSSINC)/bits

clean:
	make -f Makefile.32 clean
ifeq ($(WITH_SHORT_LIBS), 1)
	make -f Makefile.16 clean
endif
