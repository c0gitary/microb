cflag = -Wall -Wextra -O3 -std=gnu99
src = main.c builder.c config.c utils.c files.c
obj = $(src:.c=.o)
target = microb

all: $(target)

$(target): $(obj)
	gcc $(cflag) $(obj) -o $(target)

%.o: %.c
	gcc $(cflag) -c $< -o $@

.PHONY: clean all
clean:
	rm -f $(obj) $(target)
