.PHONY: all test clean .FORCE

all: cmd/smallest-rational

cmd/smallest-rational: .FORCE
	cd libsr      && make
	cd libcscript && make
	cd cmd        && make

test: cmd/smallest-rational
	cd cmd && make test

clean:
	cd libsr      && make clean
	cd libcscript && make clean
	cd cmd        && make clean

.FORCE:

show-targets:
	@show-makefile-targets

show-%:
	@echo $*=$($*)
