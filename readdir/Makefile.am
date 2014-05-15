AM_CFLAGS = -I$(top_srcdir)/lib @AM_CFLAGS@
bin_PROGRAMS = readdir
readdir_SOURCES = readdir.c
readdir_LDADD = $(top_builddir)/lib/libbenchmark.a \
		$(top_builddir)/lib/libcopy.a \ 
			$(AM_LDFLAGS)

PROG   = readdir
GRAPHS = readdir.csv readdirfull.csv

include ../lib/lib.mk
