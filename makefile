IDIR =includes
CC=gcc
CFLAGS=-I $(IDIR)

ODIR=obj
LDIR =../lib

SRCDIR=src

BUILDDIR=build

LIBS=

_DEPS =clkUtilities.h generic_queue.h process_queue.h queueUtilities.h defs.h headers.h process_struct.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ =clk.o generic_queue.o processGenerator.o scheduler.o FilesGenerator.o process.o process_queue.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_CLK_SOURCE = clk.o
_FILES_GENERATOR_SOURCE=FilesGenerator.o
_PROCESS_SOURCE=process.o
_SCHEDULER_SOURCE=scheduler.o
_PROCESS_GENERATOR_SOURCE=processGenerator.o generic_queue.o process_queue.o

CLK_SOURCE= $(patsubst %,$(ODIR)/%,$(_CLK_SOURCE))

FILES_GENERATOR_SOURCE= $(patsubst %,$(ODIR)/%,$(_FILES_GENERATOR_SOURCE))

PROCESS_SOURCE= $(patsubst %,$(ODIR)/%,$(_PROCESS_SOURCE))

SCHEDULER_SOURCE= $(patsubst %,$(ODIR)/%,$(_SCHEDULER_SOURCE))

PROCCESS_GENERATOR_SOURCE= $(patsubst %,$(ODIR)/%,$(_PROCESS_GENERATOR_SOURCE))

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clock: $(CLK_SOURCE)
	$(CC) -o $(BUILDDIR)/$@ $^ $(CFLAGS) $(LIBS)
files_generator: $(FILES_GENERATOR_SOURCE)
	$(CC) -o $(BUILDDIR)/$@ $^ $(CFLAGS) $(LIBS)
process: $(PROCESS_SOURCE)
	$(CC) -o $(BUILDDIR)/$@ $^ $(CFLAGS) $(LIBS)
scheduler: $(SCHEDULER_SOURCE)
	$(CC) -o $(BUILDDIR)/$@ $^ $(CFLAGS) $(LIBS)
process_generator: $(PROCCESS_GENERATOR_SOURCE)
	$(CC) -o $(BUILDDIR)/$@ $^ $(CFLAGS) $(LIBS)
directories:
	mkdir $(ODIR)
	mkdir $(BUILDDIR)
	
build:directories clock files_generator process_generator process scheduler

.PHONY: clean

clean:
	rm -r -f $(ODIR) $(BUILDDIR)

clean_OS:
	ipcrm -M 300
	ipcrm -Q 777
	@echo "All OS Items Cleared"

all:clean build
