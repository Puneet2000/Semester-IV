libmyshm.so: libmyshm.o
	g++ -std=c++11 -shared -o libmyshm.so libmyshm.o -g

libmyshm.o: libmyshm.cpp
	g++ -std=c++11 -fpic -c -o libmyshm.o -I ../include -I . libmyshm.cpp -g

clean:
	rm -f *.o *.so
