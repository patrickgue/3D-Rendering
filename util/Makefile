MODEL_PROG=model
MODEL_SRCS=model.c
MODEL_OBJS=$(MODEL_SRCS:.c=.o)


all:$(MODEL_PROG)


$(MODEL_PROG):$(MODEL_OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o:%.c
	$(CC) -c $^ -o $@

clean:
	rm -f $@ *.o
