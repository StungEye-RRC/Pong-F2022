#pragma once
#include "ofGraphics.h"
#include "ofMath.h"

enum class Edge { top, right, bottom, left, none };

class Sprite {
protected:
	float width, height;
	glm::vec2 position;
public:
	Sprite(float xPosition, float yPosition, float width, float height);
	void warpTo(const glm::vec2& destination);
	void clampToBoundary(const glm::vec2& min, const glm::vec2& max);
	void draw() const;
	Edge beyondVerticalBounds(float min, float max) const;
protected:
	std::pair<Edge, float> intersects(Sprite& other);
	ofRectangle boundingBox();
};
