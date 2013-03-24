CFLAGS = -O3

all: clean main

all_objects:
	g++ src/utility.cpp -c -o utility.o $(CFLAGS)
	g++ src/vector2d.cpp -c -o vector2d.o $(CFLAGS)
	g++ src/triangle.cpp -c -o triangle.o $(CFLAGS)
	g++ src/prism.cpp -c -o prism.o $(CFLAGS)
	g++ -fopenmp src/triangle_complex.cpp -c -o triangle_complex.o $(CFLAGS)

main: all_objects
	g++ -fopenmp src/main.cpp -o main *.o $(CFLAGS)
	rm -f *.o

clean:
	rm -f *.o main
