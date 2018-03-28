CC = gcc
WARNINGS = -Wall -Wformat -Wno-deprecated-declarations
CFLAGS = -I./include
LDLIBS = -lm
version = 0.2.0


UNAME_S := $(shell uname -s)

objs = $(srcs:.c=.o)
exe = dmp

SRCDIR = src
OBJDIR = build

STRUCTURE := $(shell find $(SRCDIR) -type d)
CODEFILES := $(addsuffix /*,$(STRUCTURE))
CODEFILES := $(wildcard $(CODEFILES))

SRCFILES := $(filter %.c,$(CODEFILES))
OBJFILES := $(subst $(SRCDIR),$(OBJDIR),$(SRCFILES:%.c=%.o))

.PHONY: all clean

all: $(OBJDIR) $(exe)

$(OBJDIR):
	@mkdir -p $@

$(OBJDIR)/%.o: $(addprefix $(SRCDIR)/,%.c)
	@printf "Compiling    $@ <- $<\n"
	@mkdir -p $(dir $@)
	@$(CC) $(WARNINGS) $(CFLAGS) -c $< -o $@

$(exe):  $(OBJFILES)
	@printf "Linking      $@ <- $(OBJDIR)/*\n"
	@$(CC) $(WARNINGS) -o $@ $(foreach i,$^,$(i) ) $(LDLIBS) 
	@printf "Compiled     v$(version) for $(UNAME_S)\\n"

clean:
	@rm -rf $(exe)
	@rm -rf $(OBJDIR)/*.o
	@rm -rf $(OBJDIR)/render/*.o

install: $(exe)
	install dmp ~/.bin
# luajit/src/libluajit.a:
# 	$(MAKE) -C luajit
