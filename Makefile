prefix ?= C:/msys64/mingw64/

INCPATH = $(prefix)/include
LIBPATH = $(prefix)/lib

CPPFLAGS = -std=gnu++11 -Wall -O2 -static-libgcc -static-libstdc++

LIBS = -lglew32 -lglfw3 -lopengl32 -lgdi32 -lsupc++ -lws2_32 -lpng16 -lz -lm -lfreetype

INCS = simpleGL.h glfw.h shader.h shaderData.h image.h texture.h color.h math.h matrix.h point.h shape.h unsortedSprite.h sprite.h \
 			camera.h cursor.h timer.h light.h font.h text.h
OBJS = simpleGL.o shader.o shaderData.o image.o texture.o math.o matrix.o unsortedSprite.o sprite.o \
			camera.o cursor.o timer.o light.o font.o text.o

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

all: static install
