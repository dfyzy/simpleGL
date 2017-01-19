#ifndef SIMPLE_LIGHT_H
#define SIMPLE_LIGHT_H

#include <list>
#include "simpleSprite.hpp"
#include "simpleTexture.hpp"

class SimpleLight : public SimpleTexture, public SimpleSprite {
public:
	class Source : public SimplerSprite {
	public:
		SimpleLight* light;

	public:
		Source(SimpleLight* light, SimpleVector position, SimpleVector bounds, float rotation, SimpleRGB rgb);

		Source(const Source& other) =delete;
		Source(Source&& other) =delete;

		~Source() {
			light->sources.remove(this);
		}

		void setPosition(SimpleVector position) const {
			SimplerSprite::setPosition(position - light->position);
			light->toggleDraw();
		}

		void setBounds(SimpleVector bounds) const {
			SimplerSprite::setBounds(bounds);
			light->toggleDraw();
		}

		void setRotation(float rotation) const {
			SimplerSprite::setRotation(rotation);
			light->toggleDraw();
		}

		void setColor(SimpleRGB c) const {
			SimplerSprite::setColor(c);
			light->toggleDraw();
		}

	};

private:
	SimpleVector position;
	SimpleRGB baseRGB;

	GLuint fbo;
	std::list<Source*> sources;

	bool needToDraw = true;

	~SimpleLight();

public:
	SimpleLight(SimpleVector position, int z, unsigned width, unsigned height, SimpleRGB baseRGB);

	void toggleDraw() { needToDraw = true; }

	void draw();

	void unload() {
		SimpleTexture::unload();
		SimpleSprite::unload();
	}

};

#endif
