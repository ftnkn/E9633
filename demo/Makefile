
SUBDIRS := can uart
#PWD := $(shell pwd)

.PHONY: subdirs clean

sudbdirs:
	mkdir -p out
	@for dir in $(SUBDIRS);do\
		make -C $$dir;\
	done

clean:
	@for dir in $(SUBDIRS);do\
		make -C $$dir clean;\
	done

