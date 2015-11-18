all: dvr

CC = g++
CFLAGS = -ggdb -W

OBJS = HTTP.o MyServerSocket.o MySocket.o http_parser.o main.o HTTPRequest.o

-include $(OBJS:.o=.d)

dvr: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LDFLAGS)

%.d: %.cpp
	set -e; $(CC) -MM $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	set -e; $(CC) -MM $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@


%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean: 
	rm -f dvr *.o *~ core.* *.d
