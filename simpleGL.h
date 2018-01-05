#ifndef SIMPLE_GL_H
#define SIMPLE_GL_H

namespace simpleGL {

class SortedFrame;

double getDeltaTime();

SortedFrame* getMainFrame();

//should probably move this somewhere
void loadGLContext();

/*
 *	Starts draw loop
 */
void draw();

}

#endif
