CFLAGS=-O3 -Wall -Wextra -s


.PHONY: fib fibThread

fib: fib.c
	$(CC) $(CFLAGS) $< -o $@

fibthread: fibthread.c
	$(CC) $(CFLAGS) $< -o $@ -lpthread


clean:
	$(RM) fib fibthread *.o *.s