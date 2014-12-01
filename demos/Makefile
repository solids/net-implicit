LIBSDL = -L/usr/local/lib -lSDL
LIBGL  = -framework GLUT -framework OpenGL
LIBS = -L../threed -lthreed -lstdc++ $(LIBSDL) $(LIBGL)

OBJS_COMMON = demoapp.o demoio.o demoappbaseimp.o sphere.o box.o
OBJS_DEMO1 = $(OBJS_COMMON) demo1.o
OBJS_DEMO2 = $(OBJS_COMMON) demo2.o
OBJS_DEMO3 = $(OBJS_COMMON) demo3.o

all:	demo1 demo2 demo3

clean:
	rm -rf *.o demo1 demo2 demo3

demo1: $(OBJS_DEMO1) ../threed/libthreed.a
	gcc -o demo1 $(OBJS_DEMO1) $(LIBS)

demo2: $(OBJS_DEMO2) ../threed/libthreed.a
	gcc -o demo2 $(OBJS_DEMO2) $(LIBS)

demo3: $(OBJS_DEMO3) ../threed/libthreed.a
	gcc -o demo3 $(OBJS_DEMO3) $(LIBS)

.cpp.o:
	g++ -o $*.o -c $*.cpp -I.. 
