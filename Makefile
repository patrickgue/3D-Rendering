PROG=game
SRCS=game.c vector.c text.c
OBJS=$(SRCS:.c=.o)
MODELS=assets/square.bin assets/plane.bin assets/cube.bin

CFLAGS+=-g -I/Users/patrick/Development/third/minifb/include
LDFLAGS+=-L/Users/patrick/Development/third/minifb/build -lminifb -ObjC -lObjC -framework Cocoa  -framework Metal -framework MetalKit

all:$(PROG)

$(PROG):$(OBJS) models
	c++ $(LDFLAGS) $(OBJS) -o $@

%.o:%.c
	cc $(CFLAGS) -c $^ -o $@ 

clean:
	rm -f $(OBJS) $(PROG) assets/*.bin
	make -C util/ clean

models: model $(MODELS)

%.bin:%.mdl 
	./util/model $^ $@


test:$(PROG)
	./$(PROG) -d

model:
	make -C util/ model
