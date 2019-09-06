bin_PROGRAMS = gendict

AM_CFLAGS = -Wall -g 

base = gendict.c

gendict_SOURCES = $(base)

CLEANFILES = core *.core *~ *.stackdump

DISTCLEANFILES = autoconf.h stamp-auto-h autom4te*.cache

gendict$(EXEEXT): $(gendict_OBJECTS) 
	@rm -f gendict$(EXEEXT)
	$(LINK) $(gendict_OBJECTS) $(LIBS)
	cp gendict$(EXEEXT) ../