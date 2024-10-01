# Flags to pass to compiler
CFLAGS := -O3 -Wall -Wextra -std=c99 -I.

all:
	@$(CC) $(CFLAGS) re.c tests/test1.c         -o tests/test1
	@$(CC) $(CFLAGS) re.c tests/test2.c         -o tests/test2
	@$(CC) $(CFLAGS) re.c tests/test_rand.c     -o tests/test_rand
	@$(CC) $(CFLAGS) re.c tests/test_rand_neg.c -o tests/test_rand_neg
	@$(CC) $(CFLAGS) re.c tests/test_compile.c  -o tests/test_compile

clean:
	@rm -f tests/test1 tests/test2 tests/test_rand tests/test_compile
	@#@$(foreach test_bin,$(TEST_BINS), rm -f $(test_bin) ; )
	@rm -f a.out
	@rm -f *.o

