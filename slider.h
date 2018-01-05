/* Button draggable only in horizontal(+ angle) direction
*/

#ifndef SIMPLE_SLIDER_H
#define SIMPLE_SLIDER_H

#include "sortedSprite.h"
#include "cursor.h"

namespace simpleGL {

class Slider : public Point {
private:
	class SliderSprite : public SortedSprite, public Button {
	private:
		Slider* slider;

	protected:
		~SliderSprite() {}

	public:
		SliderSprite(Slider* parent, Vector scale, Angle rotation, Texture texture, Vector pivot, Color color, SortedFrame* frame, int z)
			: SortedSprite(parent, {}, scale, rotation, texture, pivot, color, frame, z), Button(this), slider(parent) {}

		void onDrag(int mouseButton) override {
			Vector pos = slider->getModelMatrix().inv()*Cursor::getInstance()->getRealPosition();
			float hLength = slider->getLength()/2;

			if (std::abs(pos.x) > hLength)	pos.x = std::copysign(hLength, pos.x);

			setPosition({pos.x, getPosition().y});

			slider->callback((pos.x + hLength)/slider->getLength());
		}

	};

	SliderSprite* sprite;
	float length;

	std::function<void(float)> callback;

protected:
	~Slider() {}

public:
	struct Loader {
		Point* pparent {nullptr};
		Vector pposition;
		Vector pscale {1};
		Angle protation;
		Texture ptexture;
		Vector ppivot;
		Color pcolor {1};
		SortedFrame* pframe {getMainFrame()};
		int pz {0};
		float plength {1.0f};
		Angle pslideRotation;

		Loader() {}

		Loader& parent(Point* p) { pparent = p; return *this; }
		Loader& position(Vector v) { pposition = v; return *this; }
		Loader& scale(Vector v) { pscale = v; return *this; }
		Loader& rotation(Angle a) { protation = a; return *this; }
		Loader& texture(Texture t) { ptexture = t; return *this; }
		Loader& pivot(Vector v) { ppivot = v; return *this; }
		Loader& color(Color c) { pcolor = c; return *this; }
		Loader& frame(SortedFrame* f) { pframe = f; return *this; }
		Loader& z(int i) { pz = i; return *this; }
		Loader& length(float f) { plength = f; return *this; }
		Loader& slideRotation(Angle a) { pslideRotation = a; return *this; }

		Slider* load() { return new Slider(pparent, pposition, pscale, protation, ptexture, ppivot, pcolor, pframe, pz, plength, pslideRotation); }
	};

	Slider(Point* parent, Vector position, Vector scale, Angle rotation, Texture texture, Vector pivot, Color color, SortedFrame* frame, int z,
		float length, Angle slideRotation)
			: Point(parent, position, {1.0f}, slideRotation),
			sprite(new SliderSprite(this, scale, rotation, texture, pivot, color, frame, z)),
			length(length) {}

	float getLength() const { return length; }
	void setLength(float f) { length = f; }

	void setCallback(std::function<void(float)> func) { callback = func; }

};

}


#endif
