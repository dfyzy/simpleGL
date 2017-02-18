CPPFLAGS = -std=gnu++11
INCPATH = C:/Libs/C++/include
LIBPATH = C:/Libs/C++/lib
STLIBS = #-static-libgcc -static-libstdc++
LIBS = -lglew32 -lglfw3 -lopengl32 -lgdi32 -lsupc++ -lws2_32 -lpng16 -lz -lm -lfreetype

INCS = simpleGL.hpp texture.hpp sprite.hpp unsortedSprite.hpp color.hpp vector.hpp \
 			simpleMath.hpp font.hpp text.hpp glfw.hpp light.hpp shaderData.hpp
OBJS = simpleGL.o sprite.o texture.o shader.o shaderData.o font.o text.o light.o

%.o: %.cpp
	g++ -DFPS_COUNTER $(CPPFLAGS) -c $< -I$(INCPATH)

static: $(OBJS)
	ar rvs libsimpleGL.a $(OBJS)

$(INCPATH)/simpleGL:
	mkdir -p "$(INCPATH)/simpleGL"

install: $(INCS) $(INCPATH)/simpleGL
	cp libsimpleGL.a $(LIBPATH)
	cp $(INCS) "$(INCPATH)/simpleGL"

all: static install

%.exe: static install %/main.cpp
	g++ $(CPPFLAGS) -o $(*F) $(*F)/main.cpp -I$(INCPATH) $(STLIBS) -L$(LIBPATH) -lsimpleGL $(LIBS)
