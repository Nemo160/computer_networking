src = $(wildcard *.c)
obj = $(src:.c=.o)

CFLAGS = --std=c11 -g

web_server: $(obj)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f $(obj) web_server

