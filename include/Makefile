DEST=/usr/local/include/utils

HEADERS=\
	utils/bs.h        \
	utils/common.h    \
	utils/ftr.h       \
	utils/ifjmp.h     \
	utils/ifnotnull.h \
	utils/map.h       \
	utils/tralloc.h   \
	utils/unused.h    \
	utils/utils.h     \
	utils/vec.h       \

all: $(HEADERS)
	@echo "run 'make install' to install"

install: $(HEADERS)
	mkdir -p $(DEST)
	install -m 0644 $(HEADERS) -t $(DEST)

uninstall:
	rm -rf $(DEST)/$(HEADERS)
	rmdir $(DEST)

.PHONY: all install uninstall
