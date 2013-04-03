CFLAGS = 
MAINFLAGS = -L. -ltriangle -lsimplexml

all: clean all_objects main

all_objects:
	g++ src/utility.cpp -c -o utility.o $(CFLAGS)
	g++ src/vector2d.cpp -c -o vector2d.o $(CFLAGS)
	g++ src/triangle.cpp -c -o triangle.o $(CFLAGS)
	g++ src/prism.cpp -c -o prism.o $(CFLAGS)
	g++ -fopenmp src/triangle_complex.cpp -c -o triangle_complex.o $(CFLAGS)
	rm -f libtriangle.a
	ar -cr libtriangle.a *.o
	rm -f *.o

main:
	g++ -fopenmp src/main.cpp -o main $(MAINFLAGS) $(CFLAGS)

clean:
	rm -f libtriangle.a main
