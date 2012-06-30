CFLAGS=`pkg-config --cflags gtk+-3.0`
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

libcamo.so: $(OBJS)
	gcc -shared -fPIC -DPIC -o libcamo.so $(OBJS) `pkg-config --libs gtk+-3.0`
	strip libcamo.so

clean:
	rm -f libcamo.so $(OBJS)
