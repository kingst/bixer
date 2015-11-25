all: dvr.app

CC = g++
CFLAGS = -ggdb -W
CPPFLAGS = $(CFLAGS) -std=c++11

OBJS = HTTP.o MyServerSocket.o MySocket.o http_parser.o main.o HTTPRequest.o \
	Process.o HttpStreamingService.o HTTPResponse.o IptvService.o
LDFLAGS = -lpthread

-include $(OBJS:.o=.d)

dvr.app: $(OBJS)
	$(CC) -o $@ $(CPPFLAGS) $(OBJS) $(LDFLAGS)

%.d: %.cpp
	set -e; $(CC) -MM $(CPPFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

%.d: %.c
	set -e; $(CC) -MM $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@


%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f dvr.app *.o *~ core.* *.d
