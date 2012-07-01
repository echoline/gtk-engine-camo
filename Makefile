CFLAGS=`pkg-config --cflags gtk+-3.0`
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
BIN=libcamo.so

$(BIN): $(OBJS)
	gcc -shared -fPIC -DPIC -o $(BIN) $(OBJS) `pkg-config --libs gtk+-3.0`
	strip $(BIN)

clean:
	rm -f $(BIN) $(OBJS)
