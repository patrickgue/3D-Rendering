MODEL_PROG=model
MODEL_SRCS=model.c
MODEL_OBJS=$(MODEL_SRCS:.c=.o)

EDITOR_PROG=editor
EDITOR_SRCS=editor.c ../model.c ../text.c ../vector.c
EDITOR_OBJS=$(EDITOR_SRCS:.c=.o)

CFLAGS+=-I../vendor/minifb/include -g


UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
EDITOR_LDFLAGS=-L../vendor/minifb/build -lminifb -lX11 -lGL
endif
ifeq ($(UNAME), Darwin)
EDITOR_LDFLAGS=-L../vendor/minifb/build -lminifb -ObjC -lObjC -framework Cocoa  -framework Metal -framework MetalKit
endif

# EDITOR_LDFLAGS=-L../vendor/minifb/build/ -lminifb -ObjC -lObjC -framework Cocoa  -framework Metal -framework MetalKit


all:$(MODEL_PROG)


$(MODEL_PROG):$(MODEL_OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(EDITOR_PROG):$(EDITOR_OBJS)
	c++ $^ $(EDITOR_LDFLAGS) $(LDFLAGS) -o $@

%.o:%.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm -f $(MODEL_PROG) $(EDITOR_PROG) *.o
