#include "sprite.h"

Sprite::Sprite(float xPosition, float yPosition, float width, float height)
	: width{width}, height{height}, position{xPosition, yPosition} {
}

void Sprite::warpTo(const glm::vec2& destination) {
	position = destination;
}

void Sprite::clampToBoundary(const glm::vec2& min, const glm::vec2& max) {
	position.x = ofClamp(position.x, min.x + width / 2, max.x - width / 2);
	position.y = ofClamp(position.y, min.y + height / 2, max.y - height / 2);
}

void Sprite::draw() const {
	ofPushStyle();
	ofSetRectMode(OF_RECTMODE_CENTER);
	ofDrawRectangle(position.x, position.y, width, height);
	ofPopStyle();
}

Edge Sprite::beyondWhichVerticalBound(float min, float max) const {
	if (position.x <= min) {
		return Edge::left;
	}

	if (position.x > max) {
		return Edge::right;
	}

	return Edge::none;
}

// Tests to see if this sprite has overlapped another sprite.
// Returns the edge of this sprite (top, right, bottom, left)
// has overlapped, and by how much, as a std::pair.
// If no overlap is present it returns Edge::none.
std::pair<Edge, float> Sprite::intersects(const Sprite& other) const {
	const ofRectangle myBoundingBox{boundingBox()};
	const ofRectangle otherBoundingBox{other.boundingBox()};

	// Early return is there is no intersection.
	if (!myBoundingBox.intersects(otherBoundingBox)) {
		return {Edge::none, 0.0f};
	}

	const ofRectangle intersection = myBoundingBox.getIntersection(otherBoundingBox);

	if (intersection.getHeight() >= intersection.getWidth()) {
		if (intersection.getX() == otherBoundingBox.getX()) {
			return {Edge::right, -intersection.getWidth()};
		}

		return {Edge::left, intersection.getWidth()};
	}

	if (intersection.getY() == otherBoundingBox.getY()) {
		return {Edge::bottom, -intersection.getHeight()};
	}
	return {Edge::top, intersection.getHeight()};
}

ofRectangle Sprite::boundingBox() const {
	return {position.x - width / 2, position.y - height / 2, width, height};
}
