# Filename: src/libsr/Makefile
# Project: libsr
# Brief: Build the library, 'libsr'.
#
# Copyright (C) 2018 Guy Shaw
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
# Description
# -----------
# The library, 'libsr', provides the definitions and functions needed
# to support computation of the "smallest" rational number approximation
# of a given floating-point number.

LIBRARY := libsr
SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c, %.o, $(SOURCES))

CC := gcc
CPPFLAGS := -I../inc
CFLAGS := -std=c99 -Wall -Wextra -g

.PHONY: all install clean show-targets

all: $(LIBRARY).a

$(LIBRARY).a: $(OBJECTS)
	ar crv $(LIBRARY).a $(OBJECTS)

clean:
	rm -f $(LIBRARY).a $(OBJECTS) *.o

show-targets:
	@show-makefile-targets

__show-%:
	@echo $*=$($*)
