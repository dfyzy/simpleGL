CPPFLAGS = -std=gnu++11
INCPATH = C:/Libs/C++/include
LIBPATH = C:/Libs/C++/lib
STLIBS = #-static-libgcc -static-libstdc++
LIBS = -lglew32 -lglfw3 -lopengl32 -lgdi32 -lsupc++ -lws2_32 -lpng16 -lz -lm -lfreetype

INCS = simpleGL.h shader.h image.h texture.h sprite.h unsortedSprite.h color.h angle.h vector.h \
 			simpleMath.h glfw.h shaderData.h font.h text.h light.h button.h camera.h cursor.h matrix.h
OBJS = simpleGL.o unsortedSprite.o sprite.o image.o texture.o shader.o shaderData.o font.o text.o \
			light.o camera.o cursor.o matrix.o

%.o: %.cc
	g++ -DFPS_COUNTER $(CPPFLAGS) -c $< -I$(INCPATH)

static: $(OBJS)
	ar rvs libsimpleGL.a $(OBJS)

$(INCPATH)/simpleGL:
	mkdir -p "$(INCPATH)/simpleGL"

install: $(INCS) $(INCPATH)/simpleGL
	cp libsimpleGL.a $(LIBPATH)
	cp $(INCS) "$(INCPATH)/simpleGL"

all: static install

%.exe: static install %/main.cc
	g++ $(CPPFLAGS) -o $(*F) $(*F)/main.cc -I$(INCPATH) $(STLIBS) -L$(LIBPATH) -lsimpleGL $(LIBS)
