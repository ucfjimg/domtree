all: dom.o

test: domtest
	./domtest

domtest: domtest.cpp dom.h dom.o
	g++ -g -o domtest domtest.cpp dom.o

dom.o: dom.cpp dom.h
	g++ -g -c -o dom.o dom.cpp

clean:
	rm dom.o domtest


