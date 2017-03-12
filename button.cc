#include <map>

#include "button.h"
#include "cursor.h"
#include "util.h"

namespace {

std::map<simpleGL::Sprite*, simpleGL::Button*, simpleGL::Sprite::Comparer> buttons;

simpleGL::Button* clicked[simpleGL::Cursor::BUTTONS_MAX];
simpleGL::Button* hovered = nullptr;

}

namespace simpleGL {

bool inBounds(Sprite* sprite) {
	Vector dist = (sprite->getRealPosition() - Cursor::getInstance()->getPosition()).rotate(sprite->getRealRotation()).abs();
	Vector bounds = sprite->getTexture().getBounds() * sprite->getScale()/2;

	return (dist.x < bounds.x) && (dist.y < bounds.y);
}

void util::clickButtons(int button) {
	for (auto it = buttons.rbegin(); it != buttons.rend(); it++) {
		if (inBounds(it->first)) {
			//TODO:alpha stuff

			if (Cursor::getInstance()->getButton(button)) {
				it->second->onPress(button);
				clicked[button] = it->second;
			} else {
				it->second->onRelease(button);

				if (clicked[button] == it->second)
					it->second->onClick(button);

				clicked[button] = nullptr;
			}

			return;
		}
	}
}

void util::moveButtons() {
	for (int i = 0; i < Cursor::BUTTONS_MAX; i++)
		if (clicked[i])	clicked[i]->onDrag(i);

	for (auto it = buttons.rbegin(); it != buttons.rend(); it++) {
		bool wasOn = it->second == hovered;

		if (inBounds(it->first)) {
			if (!wasOn)	{
				it->second->onEnter();
				if (hovered)	hovered->onExit();
				hovered = it->second;
			}

			return;
		} else {
			if (wasOn) {
				it->second->onExit();
				hovered = nullptr;
			}
		}
	}
}

void addButton(Sprite* sprite, Button* button) {
	buttons.insert({sprite, button});
}

void removeButton(Sprite* sprite) {
	buttons.erase(sprite);
}

}
