## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = std-options
AUTOMAKE_OPTIONS += subdir-objects

AM_CFLAGS = -DUNIX -D_LINUX -DLinux -D_MACHINE=i386 -DMOS_BYTE_ORDER=MOS_LITTLE_ENDIAN -D_MOS_PTHREADS

AM_CFLAGS+= -Werror
AM_CFLAGS+= -Wreturn-type -Wcast-qual -Wwrite-strings
AM_CFLAGS+= -Wpointer-arith
AM_CFLAGS+= -Wall -Wno-format-y2k
AM_CFLAGS+= -Wno-switch
AM_CFLAGS+= -Wmissing-prototypes -Wnested-externs -Wreturn-type
AM_CFLAGS+= -Wshadow -Wchar-subscripts
AM_CFLAGS+= -Wredundant-decls -Wsign-compare

bin_PROGRAMS = phidget22admin

phidget22admin_SOURCES = phidgetadmin.h \
	main.c

if DEBUG
AM_CFLAGS += -D_DEBUG -DDEBUG
endif
