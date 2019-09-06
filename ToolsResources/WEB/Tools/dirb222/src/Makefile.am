bin_PROGRAMS = dirb

INCLUDES = @NETWORK_CFLAGS@ 

AM_CFLAGS = -Wall 

base = crea_wordlist.c\
	dirb.c\
	get_url.c\
	lanza_ataque.c\
	calculanec.c\
	utils.c\
	options.c\
	resume.c

dirb_SOURCES = $(base)

dirb_LDFLAGS = @NETWORK_LIBS@

CLEANFILES = core *.core *~ *.stackdump

DISTCLEANFILES = autoconf.h stamp-auto-h autom4te*.cache

dirb$(EXEEXT): $(dirb_OBJECTS) $(dirb_DEPENDENCIES) 
	@rm -f dirb$(EXEEXT)
	$(LINK) $(dirb_LDFLAGS) $(dirb_OBJECTS) $(dirb_LDADD) $(LIBS)
	cp dirb$(EXEEXT) ../