OBJS = boundingbox.o camera.o csgisosurface.o isomesher.o isomesher_dc.o isomesher_mc.o \
	isosurface.o lightsource.o matrix.o mesh.o meshface.o plane.o qef.o transform.o \
	world.o

all:	libthreed.a

clean:
	rm -rf *.o *.a

libthreed.a: $(OBJS)
	ar r libthreed.a $(OBJS)

.cpp.o:
	g++ -o $*.o -c $*.cpp -I.. 
