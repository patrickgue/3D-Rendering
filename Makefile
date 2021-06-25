PROG=game
SRCS=game.c vector.c text.c
OBJS=$(SRCS:.c=.o)

CFLAGS+=-g -I/Users/patrick/Development/third/minifb/include
LDFLAGS+=-L/Users/patrick/Development/third/minifb/build -lminifb -ObjC -lObjC -framework Cocoa  -framework Metal -framework MetalKit

all:$(PROG)

$(PROG):$(OBJS)
	c++ $(LDFLAGS) $^ -o $@

%.o:%.c
	cc $(CFLAGS) -c $^ -o $@ 

clean:
	rm -f $(OBJS) $(PROG)


test:$(PROG)
	./$(PROG)
