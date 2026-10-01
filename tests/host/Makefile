CFLAGS := -std=c99 -Wall -Wextra -Werror -O1 -g -I../../src/c/logic
LOGIC_SRC := $(wildcard ../../src/c/logic/*.c)
TESTS := $(wildcard test_*.c)
BINS := $(patsubst test_%.c,bin/%,$(TESTS))

.PHONY: test clean

test: $(BINS)
	@for t in $(BINS); do \
		echo "RUN $$t"; \
		./$$t || exit $$?; \
	done

bin/%: test_%.c $(LOGIC_SRC) | bin
	$(CC) $(CFLAGS) $< $(LOGIC_SRC) -o $@

bin:
	mkdir -p bin

clean:
	rm -rf bin
