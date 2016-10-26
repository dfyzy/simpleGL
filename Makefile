INCPATH = C:/Libs/C++/include
CPPFLAGS = -std=gnu++11
LIBPATH = C:/Libs/C++/lib
STLIBS = -static-libgcc -static-libstdc++
LIBS = -lglew32 -lglfw3 -lopengl32 -lgdi32 -lsupc++ -lws2_32 \
			-lboost_thread-mgw49-mt-s-1_62 -lboost_system-mgw49-mt-s-1_62 -lpng16 -lz -lm

INCS = simpleGL.hpp simpleTexture.hpp simpleSprite.hpp color.hpp
OBJS = simpleGL.o spriteUtil.o textureUtil.o
EXM = example/main.cpp

.cpp.o:
	g++ $(CPPFLAGS) -c $< -I$(INCPATH)

static: $(OBJS)
	ar rvs libsimpleGL.a $(OBJS)

install:
	cp libsimpleGL.a $(LIBPATH)
	test -d "$(INCPATH)/simpleGL" || mkdir -p "$(INCPATH)/simpleGL" && cp $(INCS) "$(INCPATH)/simpleGL"

example: $(EXM)
	g++ $(CPPFLAGS) -o example $(EXM) -I$(INCPATH) $(STLIBS) -L$(LIBPATH) -lsimpleGL $(LIBS)
