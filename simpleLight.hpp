#ifndef SIMPLE_LIGHT_H
#define SIMPLE_LIGHT_H

#include <list>
#include "simpleSprite.hpp"
#include "simpleTexture.hpp"

class SimpleLight : public SimpleTexture {
public:
	class Source : public SimplerSprite {
	public:
		SimpleLight& light;

		Source(SimpleLight& light, SimpleVector position, SimpleVector bounds, float rotation, SimpleRGB rgb)
			: SimplerSprite(0, position - light.position, bounds/100, rotation, rgb, {}, bounds), light(light) {}

	public:
		static Source* load(SimpleLight& light, SimpleVector position, SimpleVector bounds, float rotation, SimpleRGB rgb);

		void setPosition(SimpleVector position) const {
			SimplerSprite::setPosition(position);
			light.toggleDraw();
		}

		void setBounds(SimpleVector bounds) const {
			SimplerSprite::setBounds(bounds);
			light.toggleDraw();
		}

		void setRotation(float rotation) const {
			SimplerSprite::setRotation(rotation);
			light.toggleDraw();
		}

		void setColor(SimpleRGB c) const {
			SimplerSprite::setColor(c);
			light.toggleDraw();
		}

		void unload() {
			SimplerSprite::unload();
			light.sources.remove(this);
		}

	};

private:
	SimpleSprite* sprite;

	SimpleVector position;
	SimpleRGB baseRGB;

	GLuint fbo;
	std::list<Source*> sources;

	bool needToDraw = true;

public:
	SimpleLight(SimpleVector position, int z, unsigned width, unsigned height, SimpleRGB baseRGB);

	SimpleLight(const SimpleLight& other) = delete;

	void toggleDraw() { needToDraw = true; }

	void draw();

	void unload();

};

#endif
