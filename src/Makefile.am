AUTOMAKE_OPTIONS = subdir-objects

TWITC_H = \
	twitc_reply.h \
	twitc.h
TWITC_C = \
	twitc_reply.c \
	twitc.c

bin_PROGRAMS      = twitc
include_HEADERS   = $(TWITC_H)
twitc_SOURCES     = main.c $(TWITC_H) $(TWITC_C)

lib_LIBRARIES = libtwitc.a
libtwitc_a_SOURCES = $(TWITC_H) $(TWITC_C)

SUBDIRS = test
check_PROGRAMS = \
	test/test_reply \
	test/test_twitc
TESTS = $(check_PROGRAMS)

test_test_reply_CFLAGS = $(AM_CFLAGS)
test_test_reply_SOURCES = test/test_reply.c
test_test_reply_LDADD = $(lib_LIBRARIES)

test_test_twitc_CFLAGS = $(AM_CFLAGS)
test_test_twitc_SOURCES = test/test_twitc.c
test_test_twitc_LDADD = $(lib_LIBRARIES)
