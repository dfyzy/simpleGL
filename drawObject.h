#ifndef SIMPLE_DRAW_OBJECT_H
#define SIMPLE_DRAW_OBJECT_H

#include "color.h"
#include "texture.h"

namespace simpleGL {

constexpr Vector QUAD[] = {{-0.5f, 0.5f},
									{-0.5f, -0.5f},
									{0.5f, 0.5f},
									{0.5f, -0.5f}};

constexpr int QUAD_VERTS = 4;

struct DataType { enum E { VERTEX, TEXTURE, COLOR, COUNT }; };

class DrawObject {
private:
	unsigned id;

	~DrawObject();

public:
	DrawObject();

	unsigned getId() const { return id; }

	void bindData(DataType::E type, float data[]) const;

	void bindQuadData(DataType::E type, Matrix model) const {
		float data[QUAD_VERTS*2];
		int offset = 0;

		for (int i = 0; i < QUAD_VERTS; i++)
			(model*QUAD[i]).load(data, &offset);

		bindData(type, data);
	}

	void bindVertexData(Matrix model) const {
		bindQuadData(DataType::VERTEX, model);
	}
	void bindTextureData(Texture texture) const {
		bindQuadData(DataType::TEXTURE, texture.getMatrix());
	}
	void bindColorData(Color color) const;

	void draw() const;

	void unload() { delete this; }
};

}

#endif
