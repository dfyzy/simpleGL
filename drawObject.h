/* The base visual class
 * Draws quads
*/

#ifndef SIMPLE_DRAW_OBJECT_H
#define SIMPLE_DRAW_OBJECT_H

#include "openGLContextTypes.h"

#include "color.h"
#include "matrix.h"
#include "texture.h"

namespace simpleGL {

constexpr Vector QUAD[] = {{-0.5f, 0.5f},
									{-0.5f, -0.5f},
									{0.5f, 0.5f},
									{0.5f, -0.5f}};

class DrawObject {
private:
	GLint qid;

public:
	DrawObject();
	~DrawObject();

	GLint getId() const { return qid; }

	void bindData(EBufferDataType type, float data[]) const;

	void bindQuadData(EBufferDataType type, Matrix model) const {
		float data[QUAD_VERTS*2];
		int offset = 0;

		for (int i = 0; i < QUAD_VERTS; i++)
			(model*QUAD[i]).load(data, &offset);

		bindData(type, data);
	}

	void bindVertexData(Matrix model) const {
		bindQuadData(EBufferDataType::Vertex, model);
	}
	void bindTextureData(Texture texture) const {
		bindQuadData(EBufferDataType::Texture, texture.getMatrix());
	}
	void bindColorData(Color color) const;

	void draw() const;
};

}

#endif
