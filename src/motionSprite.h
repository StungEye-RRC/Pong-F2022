#pragma once
#include <glm/common.hpp>
#include <glm/common.hpp>

#include "sprite.h";

class MotionSprite : public Sprite {
private:
	glm::vec2 velocity;
	const int coolDownAmount{50};
	int bounceCoolDown{0};

public:
	MotionSprite(float xPosition, float yPosition, float width, float height,
	             float xSpeed, float ySpeed);

	void cruiseAt(const glm::vec2& newVelocity);
	void move(float deltaTime);
	void accelerate(const glm::vec2& impulse);
	void bounceHorizontalWithEdge(float ceilingY, float floorY);
	void bounceWith(const Sprite& other);
};
