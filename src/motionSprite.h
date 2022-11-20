#pragma once
#include "sprite.h";

class MotionSprite : public Sprite {
private:
	glm::vec2 velocity;

public:
	MotionSprite(float xPosition, float yPosition, float width, float height,
	             float xSpeed, float ySpeed);

	void cruiseAt(const glm::vec2& newVelocity);
	void move(float deltaTime);
	void accelerate(glm::vec2 impulse);
	void bounceHorizontalWithEdge(float ceilingY, float floorY);
	void bounceVerticalWith(Sprite other);
};
