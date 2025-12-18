CC ?= gcc
CFLAGS ?= -Wall -Wextra -O3 -std=c99
override CFLAGS += $(CDEFS)
LDFLAGS ?= -s

SRCDIR = src
BUILDDIR = build
BINDIR = bin
OBJDIR = $(BUILDDIR)/obj
DEPDIR = $(BUILDDIR)/dep

SOURCES = \
	config.c \
	fileman.c \
	logger.c \
	utils.c \
	generator_makefile.c \
	mcu_avr.c \
	main.c 

SRCPATHS = $(appprefix $(SRCDIR)/, $(SOURCES))
OBJPATHS = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))
DEPPATHS = $(addprefix $(DEPDIR)/, $(SOURCES:.c=.d))
TARGET = $(BINDIR)/microb

.PHONY: all dirs
all: dirs $(TARGET) move

.PHONY: move
move: $(TARGET)
	mv $(TARGET) ./test

dirs:
	@mkdir -p $(BINDIR) $(BUILDDIR) $(OBJDIR) $(DEPDIR)


.PHONY: $(TARGET)
$(TARGET): $(OBJPATHS)
	$(CC) -o $@ $^ $(LDFLAGS)


$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -MMD -MF $(DEPDIR)/$*.d -c $< -o $@

-include $(DEPPATHS)

.PHONY: clean
clean:
	rm -rf $(OBJPATHS) $(DEPPATHS) $(TARGET)

