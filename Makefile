help:
	@echo "help: show this message"
	@echo "build: build everything"

TARGS := \
	examples/bs/      \
	examples/ftr/     \
	examples/map/     \
	examples/rc/      \
	examples/strm/    \
	examples/tralloc/ \
	examples/vec/     \
	src/mk/           \
	tests/            \

build: $(TARGS)

install:
	$(MAKE) -C include/ install

version:
	date +"v%Y.%m.%d-"

docs:
	doxygen

%/:
	$(MAKE) -C $@

.PHONY: build docs help install version
