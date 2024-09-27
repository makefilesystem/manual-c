TARGET := mc

LINKER :=
CC := clang++
CFLAGS := -Wall -Wvla -Wextra -Werror -pedantic-errors
INCLUDES := -I include

BUILDDIR := build
SRCDIR := src
OBJDIR := $(BUILDDIR)

SRCFILEXT := cpp

SRC := ${wildcard $(SRCDIR)/*.$(SRCFILEXT)}
OBJ := ${patsubst $(SRCDIR)/%.$(SRCFILEXT), $(OBJDIR)/%.o, $(SRC)}


.PHONY: help all clean

help:
	@echo 'Commands: '
	@echo '    make ... LINKER="-fuse-ld=..."  -- use another linker (default: gnu ld)'
	@echo '    make ... CC="..."               -- use another compiler (default: clang)'
	@echo '    make all                        -- build program'
	@echo '    make clean                      -- remove build files'
	@echo '    make help                       -- print this help'
	@echo

clean:
	rm -rf $(BUILDDIR)

all: clean $(BUILDDIR) $(OBJ)
	$(CC) $(CFLAGS) $(LINKER) $(OBJ) -o $(BUILDDIR)/$(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.$(SRCFILEXT)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)


