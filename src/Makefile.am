AUTOMAKE_OPTIONS = subdir-objects

TWITC_H = \
	twitc_reply.h \
	twitc.h

TWITC_C = \
	twitc_reply.c \
	twitc.c

TEST_TWITC_H = \
	test/test.h

include_HEADERS    = $(TWITC_H)
lib_LIBRARIES      = libtwitc.a
libtwitc_a_SOURCES = $(TWITC_H) $(TWITC_C)

SUBDIRS = test

check_PROGRAMS = \
	test/test_twitc \
	test/test_reply

TESTS = $(check_PROGRAMS)

test_test_reply_CFLAGS = -DTEST_TWITC $(CFLAGS) $(AM_CFLAGS)
test_test_reply_SOURCES = $(TEST_TWITC_H) $(TWITC_H) $(TWITC_C) test/test_reply.c

test_test_twitc_CFLAGS = -DTEST_TWITC $(CFLAGS) $(AM_CFLAGS)
test_test_twitc_SOURCES = $(TEST_TWITC_H) $(TWITC_H) $(TWITC_C) test/test_twitc.c

EXTRA_PROGRAMS = test/integration
test_integration_CFLAGS = $(CFLAGS) $(AM_CFLAGS)
test_integration_SOURCES = $(TWITC_H) $(TWITC_C) test/test_twitc_integration.c

check-valgrind:
	valgrind --leak-check=full --track-origins=yes test/test_twitc
	valgrind --leak-check=full --track-origins=yes test/test_reply

integration: test/integration
	test/integration
