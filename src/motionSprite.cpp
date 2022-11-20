#include "motionSprite.h"

MotionSprite::MotionSprite(float xPosition, float yPosition, float width, float height, float xSpeed,
                           float ySpeed): Sprite{xPosition, yPosition, width, height}, velocity{xSpeed, ySpeed} {
}

void MotionSprite::cruiseAt(const glm::vec2& newVelocity) {
	velocity = newVelocity;
}

void MotionSprite::move(float deltaTime) {
	position += velocity * deltaTime;
}

void MotionSprite::accelerate(glm::vec2 impulse) {
	velocity += impulse;
}

void MotionSprite::bounceHorizontalWithEdge(float ceilingY, float floorY) {
	// Adjust ceiling and floor positions for the ball size.
	ceilingY += height / 2.0f;
	floorY -= height / 2.0f;

	if (position.y <= ceilingY || position.y >= floorY) {
		velocity.y *= -1;
		position.y = ofClamp(position.y, ceilingY, floorY);
	}
}

void MotionSprite::bounceVerticalWith(Sprite other) {
	const std::pair<Edge, float> edgeIntersection = intersects(other);
	const Edge edge = edgeIntersection.first;
	const float overlap = edgeIntersection.second;

	if (edge == Edge::right || edge == Edge::left) {
		velocity.x *= -1;
		position.x += overlap;
	} else if (edge == Edge::top || edge == Edge::bottom) {
		// Speed things up slightly.
		velocity.x *= -1.05f;
		velocity.y *= -1.05f;
		position.y += overlap;
	}
}
