-include config.mk

# Default settings
GCC      ?= gcc
CFLAGS   ?= -g -Wall

# Update settings
CFLAGS    += --std=gnu99
LDFLAGS   += -lpthread 

# Add Plan 9 Flags
P9PFLAGS  += -I$(PLAN9)/include
LDFLAGS   += -L$(PLAN9)/lib -lplumb -l9pclient -lmux -lthread -l9
srv.o: CPPFLAGS += $(P9PFLAGS)

# Add GTK Flags
PACKAGES  += gtk+-3.0
GTKFLAGS  += $(shell pkg-config --cflags $(PACKAGES))
LDFLAGS   += $(shell pkg-config --libs   $(PACKAGES))
gui.o: CPPFLAGS += $(GTKFLAGS)

# Targets
all: notify

clean:
	rm -f notify *.o

# Rules
notify: main.o gui.o srv.o
	$(GCC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

%.o: %.c $(wildcard *.h) makefile
	$(GCC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

