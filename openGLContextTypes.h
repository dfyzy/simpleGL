#ifndef SIMPLE_OPENGL_CONTEXT_TYPES_H
#define SIMPLE_OPENGL_CONTEXT_TYPES_H

#include "glfw.h"

namespace simpleGL {

constexpr int QUAD_VERTS = 4;

enum class EBufferDataType { Vertex, Texture, Color, Count };
constexpr int bufferDataSize[(int)EBufferDataType::Count] { 2, 2, 4 };

}

#endif
