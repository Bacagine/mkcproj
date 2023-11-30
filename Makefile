# Makefile for mkcproj
# 
# Written by Gustavo Bacagine <gustavo.bacagine@protonmail.com>
#
# Description: Makefile of the mkcproj project
# 
# Copyright (C) 2023 Gustavo Bacagine
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
# 
# Date: 04/10/2023
#

TARGET       = mkcproj

# Directories
SRCDIR       = src
INCDIR       = include
INCLOGDIR    = include/trace
INCCUTILS    = include/cutils
OBJDIR       = obj
LIBDIR       = lib
BINDIR       = bin

# Binary
BIN        = $(BINDIR)/$(TARGET)

# .c files
SRC        = $(wildcard $(SRCDIR)/*.c)

# .o files
OBJ        = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

# .so or .a files
#LIB        = $(LIBDIR)

# Compilation flags
LDFLAGS      = -L $(LIBDIR)
LDLIBS       = -lm -pthread -ltrace -lcutils
CFLAGS       = -I $(INCDIR) -I $(INCLOGDIR) -I $(INCCUTILS) -Wall -Wextra 
DEBUGFLAGS   = -g -O0 -DDEBUG_COMPILATION
FAKEFLAGS    = -g -O0 -DFAKE

# Compiler
CC         = gcc

ifdef DEBUG_COMPILATION
	CFLAGS += $(DEBUGFLAGS) 
	LDFLAGS += $(DEBUGFLAGS)
else
	CFLAGS += -O3
endif

ifdef FAKE
	CFLAGS += $(FAKEFLAGS)
	LDFLAGS += $(FAKEFLAGS)
endif

all: distclean $(OBJDIR) $(BINDIR) $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LDLIBS) $(LDFLAGS)

$(BINDIR):
	mkdir $@
$(OBJDIR):
	mkdir $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS) $(LDLIBS) $(LDFLAGS)

clean:
	rm -rvf $(OBJDIR)

strip: all

install: all strip
	./install.sh

uninstall:
	./uninstall.sh

distclean: clean
	rm -rvf *.log
	rm -rvf $(BINDIR)
	
.PHONY: all clean install uninstall distclean

