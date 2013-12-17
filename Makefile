CFLAGS=`pkg-config --cflags gtk+-3.0` -fPIC
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
BIN=libcamo.so

$(BIN): $(OBJS) blur.h
	gcc -shared -o $(BIN) $(OBJS) `pkg-config --libs gtk+-3.0`
	strip $(BIN)

clean:
	rm -f $(BIN) $(OBJS)
