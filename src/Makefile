# Filename: src/Makefile
# Project: smallest-rational
# Brief: "smallest" rational that approximates a given floating-point number
#
# Copyright (C) 2016 Guy Shaw
# Written by Guy Shaw <gshaw@acm.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Top-level Makefile.  Make all libraries and commands.
# Make targets: all test clean

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
