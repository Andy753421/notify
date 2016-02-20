-include config.mk

# Default settings
GCC      ?= gcc
CFLAGS   ?= -g -Wall

# Update settings
CFLAGS    += --std=gnu99
LDFLAGS   += -lpthread 

# Add Plan 9 Flags
P9PFLAGS  += -I$(PLAN9)/include
LDFLAGS   += -L$(PLAN9)/lib -lplumb -l9pclient -lmux -lthread -lbio -l9
client.o: CPPFLAGS += $(P9PFLAGS)
server.o: CPPFLAGS += $(P9PFLAGS)

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
notify: main.o client.o server.o gui.o text.o
	@echo LD $@
	@$(GCC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

%.o: %.c $(wildcard *.h) makefile
	@echo CC $@
	@$(GCC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
