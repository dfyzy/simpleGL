INCPATH = C:/Libs/C++/include
CPPFLAGS = -std=gnu++11
LIBPATH = C:/Libs/C++/lib
STLIBS = #-static-libgcc -static-libstdc++
LIBS = -lglew32 -lglfw3 -lopengl32 -lgdi32 -lsupc++ -lws2_32 -lpng16 -lz -lm #-lfreetype #-lsupc++

INCS = simpleGL.hpp simpleTexture.hpp simpleSprite.hpp simpleShader.hpp simpleColor.hpp simpleVector.hpp simpleMath.hpp glfw.hpp shaderData.hpp
OBJS = simpleGL.o spriteUtil.o textureUtil.o shaderUtil.o shaderData.o

%.o: %.cpp
	g++ -DFPS_COUNTER $(CPPFLAGS) -c $< -I$(INCPATH)

static: $(OBJS)
	ar rvs libsimpleGL.a $(OBJS)

install: $(INCS)
	cp libsimpleGL.a $(LIBPATH)
	test -d "$(INCPATH)/simpleGL" || mkdir -p "$(INCPATH)/simpleGL" && cp $(INCS) "$(INCPATH)/simpleGL"

all: static install

%.test: static install %/main.cpp
	g++ $(CPPFLAGS) -o $(*F) $(*F)/main.cpp -I$(INCPATH) $(STLIBS) -L$(LIBPATH) -lsimpleGL $(LIBS)
