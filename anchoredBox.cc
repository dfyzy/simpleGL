#include "anchoredBox.h"
#include "box.h"

namespace simpleGL {

AnchoredBox::AnchoredBox(Point* parent, Vector position, Vector scale, Angle rotation, Vector bounds, EAnchor anchor)
	: Point(parent, position, scale, rotation), box(new Box(this, bounds)), anchor(anchor) {
	updateAnchor();
}

void AnchoredBox::updateAnchor() {
	box->setPosition(anchorToFactor(anchor)*box->getBounds()*0.5f);
	setChanges();
}

Vector AnchoredBox::getBounds() const {
	return box->getBounds();
}

void AnchoredBox::setBounds(Vector bounds) {
	if (bounds == box->getBounds())	return;

	box->setBounds(bounds);
	updateAnchor();
}

}
