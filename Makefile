help:
	@echo "help: show this message"
	@echo "build: build everything"

TARGS := \
	examples/bs/      \
	examples/ftr/     \
	examples/map/     \
	examples/strm/    \
	examples/tralloc/ \
	examples/vec/     \
	src/mk/           \
	tests/            \

build: $(TARGS)

install:
	make -C include/ install

%/:
	make -C $@

.PHONY: build help install
