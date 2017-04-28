prefix ?= C:/msys64/mingw64/

INCPATH = $(prefix)/include
LIBPATH = $(prefix)/lib

CPPFLAGS = -std=gnu++11 -Wall -O2

LIBS = -lglew32 -lglfw3 -lopengl32 -lgdi32 -lsupc++ -lws2_32 -lpng16 -lz -lm -lfreetype #-static-libgcc -static-libstdc++

INCS = simpleGL.h shader.h image.h texture.h sprite.h unsortedSprite.h point.h color.h angle.h vector.h \
 			math.h glfw.h shaderData.h font.h text.h light.h camera.h cursor.h matrix.h timer.h
OBJS = simpleGL.o unsortedSprite.o sprite.o image.o texture.o shader.o shaderData.o font.o text.o \
			light.o camera.o cursor.o matrix.o timer.o

%.o: %.cc
	g++ -DFPS_COUNTER $(CPPFLAGS) -c $<

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
