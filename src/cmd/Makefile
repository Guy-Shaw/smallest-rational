.PHONY: all test clean .FORCE

all: smallest-rational

smallest-rational: .FORCE
	cd smallest-rational && make

clean:
	cd smallest-rational && make clean

.FORCE:

show-targets:
	@show-makefile-targets

show-%:
	@echo $*=$($*)

