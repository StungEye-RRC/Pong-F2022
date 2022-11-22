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

void MotionSprite::accelerate(const glm::vec2& impulse) {
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

// Bounces this MotionSprite off another Sprite. 
void MotionSprite::bounceWith(const Sprite& other) {
	// Early exit if we recently bounced off the top or bottom
	// edge of another sprite.
	if (bounceCoolDown > 0) {
		bounceCoolDown--; // Decrement the cooldown until it hits zero.
		return;
	}

	// Check which edge of this sprite (if any) has overlapped
	// with the other sprite.
	const std::pair<Edge, float> edgeIntersection = intersects(other);
	const Edge edge = edgeIntersection.first;
	const float overlap = edgeIntersection.second;

	if (edge == Edge::right || edge == Edge::left) {
		// Deal with a vertical bounce.
		velocity.x *= -1;
		position.x += overlap;
	} else if (edge == Edge::top || edge == Edge::bottom) {
		// Deal with a horizontal bounce.
		// Speed things up slightly when we hit on the top or bottom edge.
		velocity.x *= -1.1f;
		velocity.y *= -1.2f;
		position.y += overlap;

		// Top and bottom hits get a cool down during which
		// repeated hits are ignored. This is to prevent the
		// ball from getting stuck on moving paddles.
		bounceCoolDown = coolDownAmount;
	}
}
