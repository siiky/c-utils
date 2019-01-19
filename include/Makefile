DEST=/usr/local/include/utils

HEADERS=\
	bs.h        \
	common.h    \
	ftr.h       \
	ifjmp.h     \
	ifnotnull.h \
	map.h       \
	unused.h    \
	utils.h     \
	vec.h       \

all: $(HEADERS)
	@echo "run 'make install' to install"

install: $(HEADERS)
	mkdir -p $(DEST)
	install -m 0644 $(HEADERS) -t $(DEST)

uninstall:
	rm -rf $(DEST)/$(HEADERS)
	rmdir $(DEST)
