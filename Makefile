IDIR =include
CC=gcc
CFLAGS=-I $(IDIR)

ODIR=obj
LDIR =../lib

SRCDIR=src

BUILDDIR=build

LIBS=

_DEPS =clk_utilities.h generic_queue.h process_queue.h queue_utilities.h defs.h headers.h process_struct.h highest_priority_first.h shortest_remaining_time_first.h round_robin.h

DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ =clk.o generic_queue.o process_generator.o scheduler.o files_generator.o process.o process_queue.o highest_priority_first.o shortest_remaining_time_first.o round_robin.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_CLK_SOURCE=clk.o
_FILES_GENERATOR_SOURCE=files_generator.o
_PROCESS_SOURCE=process.o
_SCHEDULER_SOURCE=scheduler.o highest_priority_first.o shortest_remaining_time_first.o round_robin.o
_PROCESS_GENERATOR_SOURCE=process_generator.o generic_queue.o process_queue.o

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

build: directories clock files_generator process_generator process scheduler
run : all
		-cd build;echo 10 | ./files_generator;./process_generator
.PHONY: clean

clean:
	rm -r -f $(ODIR) $(BUILDDIR)

clean_OS:
	-ipcrm -M 300
	-ipcrm -Q 777
	@echo "All OS Items Cleared"

all:clean clean_OS build
