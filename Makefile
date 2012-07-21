TARGET  += shparser-test

SRC     += $(wildcard *.c)
OBJ     += $(SRC:.c=.o)

GWARN   := -Wunsafe-loop-optimizations \
	   -Wdouble-promotion \
	   -Wsync-nand \
	   -Wsuggest-attribute=const \
	   -Wsuggest-attribute=pure \
	   -Wsuggest-attribute=noreturn \
	   -Wtrampolines \
	   -Wjump-misses-init \
	   -Wlogical-op \
	   -Wvector-operation-performance \
	   -Wunsuffixed-float-constants

WARN    := -Wall -Wextra -pedantic \
	   -Werror -Wfatal-errors -pedantic-errors \
	   -Wstack-protector \
	   -Wformat=2 -Wformat-y2k -Wformat-nonliteral -Wformat-security \
	   -Winit-self \
	   -Wmissing-include-dirs \
	   -Wmissing-format-attribute \
	   -Wmissing-prototypes \
	   -Wmissing-declarations \
	   -Wswitch-default -Wswitch-enum \
	   -Wfloat-equal \
	   -Wdeclaration-after-statement \
	   -Wundef \
	   -Wshadow \
	   -Wbad-function-cast \
	   -Wcast-align \
	   -Wwrite-strings \
	   -Wconversion \
	   -Waggregate-return \
	   -Wstrict-prototypes \
	   -Wold-style-definition \
	   -Wpacked \
	   -Wredundant-decls \
	   -Wnested-externs \
	   -Winline \
	   -Wdisabled-optimization \
	   -Wstack-protector \
	   -Woverlength-strings

ifeq ($(CC),clang)
  GWARN :=
  SSP   :=
else
  SSP   := --param ssp-buffer-size=4
endif

CFLAGS  += -ggdb
CFLAGS  += -O2 -pipe -mtune=generic
CFLAGS  += -fstack-protector $(SSP)
CFLAGS  += -D_FORTIFY_SOURCE=4
CFLAGS  += -std=c11 -D_XOPEN_SOURCE=700
CFLAGS  += $(WARN) $(GWARN)
LDFLAGS += -Wl,--as-needed

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

clean:
	$(RM) $(TARGET) $(OBJ)

test: all
	-@for t in $(wildcard tests/*); do			\
		echo "[=== Running test for: '$$t' ===]";	\
		./$(TARGET) $$t;				\
	done

.PHONY: all clean test
.PRECIOUS: $(SRC)
