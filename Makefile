prefix ?= C:/msys64/mingw64

INCPATH = $(prefix)/include
LIBPATH = $(prefix)/lib

CPPFLAGS = -std=gnu++11 -g -Wall -O0 -static-libgcc -static-libstdc++

LIBS = -Wl,-Bstatic -lglew32 -lglfw3 -lopengl32 -lgdi32 -lsupc++ -lws2_32 -lfreetype -lharfbuzz -lfreetype -lgraphite2 -lglib-2.0 \
			-lpng16 -lz -lm -lbz2 -lsndfile -lvorbis -lvorbisenc -logg -lspeex -lflac -Wl,-Bdynamic -lopenal

INCS = $(wildcard *.h)
SRCS = $(wildcard *.cc)
OBJS = $(SRCS:.cc=.o)

%.o: %.cc
	g++ -DDEBUG -DFPS_COUNTER $(CPPFLAGS) -c $<

static: $(OBJS)
	rm -f libsimpleGL.a
	ar rvs libsimpleGL.a $(OBJS)

$(INCPATH)/simpleGL:
	mkdir -p "$(INCPATH)/simpleGL"

install: $(INCS) $(INCPATH)/simpleGL
	cp libsimpleGL.a $(LIBPATH)
	cp $(INCS) "$(INCPATH)/simpleGL"

%.exe: static install %/main.cc
	g++ $(CPPFLAGS) -o $(*F) $(*F)/main.cc -lsimpleGL $(LIBS)

test: example.exe sprites.exe shadow.exe stencils.exe

all: static install

clean:
	rm -f *.o *.exe libsimpleGL.a
