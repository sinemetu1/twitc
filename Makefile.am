AUTOMAKE_OPTIONS = subdir-objects
SUBDIRS = src
dist_doc_DATA = README.md

ACLOCAL_AMFLAGS = -I m4

README: README.md

integration:
	cd src; make integration;

check-valgrind:
	cd src; make check-valgrind;
