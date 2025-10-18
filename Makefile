# Master Makefile for Pintos
# Builds each subdirectory like threads/, devices/, etc.

SUBDIRS = threads devices lib tests

.PHONY: all clean $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

