CC      ?= gcc
CFLAGS  ?= -O2 -Wall
LDFLAGS ?=

TARGET  = tapedecay.so
DESTDIR = $(HOME)/.ladspa
CONFDIR = $(HOME)/.config/pipewire/filter-chain.conf.d

.PHONY: all install uninstall load unload reload clean

all: $(TARGET)

$(TARGET): tapedecay.c ladspa.h
	$(CC) $(CFLAGS) -shared -fPIC -o $@ tapedecay.c $(LDFLAGS)

install: $(TARGET)
	mkdir -p $(DESTDIR) $(CONFDIR)
	cp $(TARGET) $(DESTDIR)/
	cp tapedecay.conf $(CONFDIR)/

uninstall:
	rm -f $(DESTDIR)/$(TARGET) $(CONFDIR)/tapedecay.conf

load: install
	systemctl --user restart pipewire

unload: uninstall
	systemctl --user restart pipewire

reload: install
	systemctl --user restart pipewire

clean:
	rm -f $(TARGET)
