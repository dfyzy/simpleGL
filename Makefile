prefix ?= C:/msys64/mingw64

INCPATH = $(prefix)/include
LIBPATH = $(prefix)/lib

CPPFLAGS = -std=gnu++11 -Wall -O2 -static-libgcc -static-libstdc++

LIBS = -Wl,-Bstatic -lglew32 -lglfw3 -lopengl32 -lgdi32 -lsupc++ -lws2_32 -lfreetype -lharfbuzz -lfreetype -lgraphite2 -lglib-2.0 \
			-lpng16 -lz -lm -lbz2 -lsndfile -lvorbis -lvorbisenc -logg -lspeex -lflac -Wl,-Bdynamic -lopenal

INCS = anchoredBox.h angle.h audio.h box.h camera.h color.h cursor.h drawObject.h font.h framebuffer.h glfw.h \
			image.h lighting.h math.h matrix.h point.h shader.h shaderData.h shape.h signal.h simpleGL.h slider.h \
				speaker.h sprite.h text.h texture.h timer.h unsortedSprite.h util.h vector.h
OBJS = angle.o audio.o camera.o cursor.o drawObject.o font.o framebuffer.o \
			image.o lighting.o matrix.o shader.o shaderData.o simpleGL.o \
				speaker.o sprite.o text.o timer.o unsortedSprite.o vector.o

%.o: %.cc
	g++ -DDEBUG -DFPS_COUNTER $(CPPFLAGS) -c $<

static: $(OBJS)
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
