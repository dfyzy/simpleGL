/* The base visual class
 * Draws quads
*/

#ifndef SIMPLE_DRAW_OBJECT_H
#define SIMPLE_DRAW_OBJECT_H

#include "color.h"
#include "matrix.h"
#include "texture.h"

namespace simpleGL {

constexpr Vector QUAD[] = {{-0.5f, 0.5f},
									{-0.5f, -0.5f},
									{0.5f, 0.5f},
									{0.5f, -0.5f}};

constexpr int QUAD_VERTS = 4;

enum class EDataType { Vertex, Texture, Color, Count };

class DrawObject {
private:
	unsigned id;

	~DrawObject();

public:
	DrawObject();

	unsigned getId() const { return id; }

	void bindData(EDataType type, float data[]) const;

	void bindQuadData(EDataType type, Matrix model) const {
		float data[QUAD_VERTS*2];
		int offset = 0;

		for (int i = 0; i < QUAD_VERTS; i++)
			(model*QUAD[i]).load(data, &offset);

		bindData(type, data);
	}

	void bindVertexData(Matrix model) const {
		bindQuadData(EDataType::Vertex, model);
	}
	void bindTextureData(Texture texture) const {
		bindQuadData(EDataType::Texture, texture.getMatrix());
	}
	void bindColorData(Color color) const;

	void draw() const {
		glDrawArrays(GL_TRIANGLE_STRIP, id*4, 4);
	}

	void unload() { delete this; }
};

}

#endif
