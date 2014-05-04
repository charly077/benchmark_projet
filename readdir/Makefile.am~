AM_CFLAGS = -I$(top_srcdir)/lib -BM_USE_CLOCK_GETTIME @AM_CFLAGS@
bin_PROGRAMS = readdir
readdir_SOURCES = readdir.c
readdir_LDADD = $(top_builddir)/lib/libbenchmark.a $(AM_LDFLAGS)

PROG   = readdir
GRAPHS = readdir.csv readdirtot.csv

include ../lib/lib.mk
