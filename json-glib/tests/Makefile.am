include $(top_srcdir)/build/autotools/Makefile.am.gtest
include $(top_srcdir)/build/autotools/Makefile.am.silent

NULL =

INCLUDES = \
	-I$(top_srcdir)			\
	-I$(top_srcdir)/json-glib	\
	$(NULL)

AM_CPPFLAGS = $(JSON_DEBUG_CFLAGS)
AM_CFLAGS = -g $(JSON_CFLAGS)
AM_LDFLAGS = $(JSON_LIBS)

noinst_PROGRAMS = $(TEST_PROGS)
progs_ldadd = $(top_builddir)/json-glib/libjson-glib-1.0.la

TEST_PROGS          += array-test
array_test_SOURCES   = array-test.c
array_test_LDADD     = $(progs_ldadd)

TEST_PROGS          += object-test
object_test_SOURCES  = object-test.c
object_test_LDADD    = $(progs_ldadd)

TEST_PROGS          += node-test
node_test_SOURCES    = node-test.c
node_test_LDADD      = $(progs_ldadd)

