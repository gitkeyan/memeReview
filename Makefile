CC = g++
CFLAGS = -pg -g -O2 -Wall
CXXFLAGS = -g -O2 -fopenmp -std=gnu++11
LDFLAGS = -pg -g
LIBS = -lm

raytracer:  main.o raytracer.o util.o light_source.o scene_object.o bmp_io.o
	$(CC) $(CFLAGS) $(CXXFLAGS) -o raytracer \
	main.o raytracer.o util.o light_source.o scene_object.o bmp_io.o $(LIBS)

clean:
	-rm -f core *.o
	-rm raytracer