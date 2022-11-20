#pragma once

#include "ofMain.h"

enum class Edge { top, right, bottom, left, none };

class Sprite {
protected:
	float width, height;
	glm::vec2 position;
public:
	Sprite(float xPosition, float yPosition, float width, float height)
		: width{width}, height{height}, position{xPosition, yPosition} {
	}

	void warpTo(const glm::vec2& destination) {
		position = destination;
	}

	void clampToBoundary(const glm::vec2& min, const glm::vec2& max) {
		position.x = ofClamp(position.x, min.x + width / 2, max.x - width / 2);
		position.y = ofClamp(position.y, min.y + height / 2, max.y - height / 2);
	}

	void draw() const {
		ofPushStyle();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofDrawRectangle(position.x, position.y, width, height);
		ofPopStyle();
	}

	Edge beyondVerticalBounds(float min, float max) const {
		if (position.x <= min) {
			return Edge::left;
		}

		if (position.x > max) {
			return Edge::right;
		}

		return Edge::none;
	}

	std::pair<Edge, float> intersects(Sprite& other) {
		const ofRectangle myBoundingBox{boundingBox()};
		const ofRectangle otherBoundingBox{other.boundingBox()};

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

	ofRectangle boundingBox() {
		return {position.x - width / 2, position.y - height / 2, width, height};
	}
};

class MotionSprite : public Sprite {
private:
	glm::vec2 velocity;
public:
	MotionSprite(float xPosition, float yPosition, float width, float height,
	             float xSpeed, float ySpeed)
		: Sprite{xPosition, yPosition, width, height}, velocity{xSpeed, ySpeed} {
	}

	// A setter for velocity.
	void cruiseAt(const glm::vec2& newVelocity) {
		velocity = newVelocity;
	}

	// Apply the velocity to the position.
	void move(float deltaTime) {
		position += velocity * deltaTime;
	}

	// This impulse will be applied to the velocity.
	void accelerate(glm::vec2 impulse) {
		velocity += impulse;
	}

	void bounceHorizontalWithEdge(float ceilingY, float floorY) {
		// Adjust ceiling and floor positions for the ball size.
		ceilingY += height / 2.0f;
		floorY -= height / 2.0f;

		if (position.y <= ceilingY || position.y >= floorY) {
			velocity.y *= -1;
			position.y = ofClamp(position.y, ceilingY, floorY);
		}
	}

	void bounceVerticalWith(Sprite other) {
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
};


class PongGame {
public:
	enum class GameType { playerVsPlayer, playerVsAi };

	enum class PlayerTurn { player1, player2 };

	void setup(const GameType& gameType) {
		playerTurn = (ofRandom(100) > 50) ? PlayerTurn::player1 : PlayerTurn::player2;
		startRally = true;

		if (gameType == GameType::playerVsPlayer) {
			ofAddListener(ofEvents().keyPressed, this, &PongGame::keyPressedPvP);
			ofAddListener(ofEvents().keyReleased, this, &PongGame::keyReleasedPvP);
		}
	}

	void update(double deltaTime, float canvasWidth, float canvasHeight) {
		if (startRally) {
			restartRally(canvasWidth, canvasHeight);
		}

		gamePlay(deltaTime, canvasWidth, canvasHeight);
		checkForWin(canvasWidth);
	}

	void draw() {
		ofDrawBitmapString("P1: " + std::to_string(p1Score), 200, 40);
		ofDrawBitmapString("P2: " + std::to_string(p2Score), 550, 40);

		p1Paddle.draw();
		p2Paddle.draw();

		ball.draw();
	}

	void restartRally(float canvasWidth, float canvasHeight) {
		startRally = false;
		const auto horizontalMiddle{canvasWidth / 2.0f};
		const auto verticalMiddle{canvasHeight / 2.0f};

		// Reposition ball and paddles to center points.
		ball.warpTo({horizontalMiddle, verticalMiddle});
		p1Paddle.warpTo({paddleEdgeBuffer, verticalMiddle});
		p2Paddle.warpTo({canvasWidth - paddleEdgeBuffer, verticalMiddle});

		// Pick random Y speed from a predefined selection.
		float startSpeedY = ofRandom(gameSpeed / 2) - gameSpeed;
		float startSpeedX = (playerTurn == PlayerTurn::player1) ? gameSpeed : -gameSpeed;

		// Set X speed (horizontal direction) depending on who is serving.
		ball.cruiseAt({startSpeedX, startSpeedY});
	}

	void gamePlay(float deltaTime, float canvasWidth, float canvasHeight) {
		p1Paddle.move(deltaTime);
		p2Paddle.move(deltaTime);
		p1Paddle.clampToBoundary({0, 0}, {canvasWidth, canvasHeight});
		p2Paddle.clampToBoundary({0, 0}, {canvasWidth, canvasHeight});

		ball.move(deltaTime);
		ball.bounceHorizontalWithEdge(0, canvasHeight);
		ball.bounceVerticalWith(p1Paddle);
		ball.bounceVerticalWith(p2Paddle);
	}

	void checkForWin(float canvasWidth) {
		const Edge offEdge = ball.beyondVerticalBounds(0, canvasWidth);

		if (offEdge == Edge::none) return;

		// Someone got a point, so the rally restarts.
		startRally = true;

		// CHECK IF PLAYER 1 SCORES POINT 
		if (offEdge == Edge::right) {
			++p1Score;
			playerTurn = PlayerTurn::player1;
		}

		// CHECK IF PLAYER 2 SCORES POINT 
		if (offEdge == Edge::left) {
			++p2Score;
			playerTurn = PlayerTurn::player2;
		}
	}

	void keyPressedPvP(ofKeyEventArgs& e) {
		if (e.key == 'w') p1Paddle.cruiseAt({0, -gameSpeed});
		if (e.key == 's') p1Paddle.cruiseAt({0, gameSpeed});
		if (e.key == 'i') p2Paddle.cruiseAt({0, -gameSpeed});
		if (e.key == 'k') p2Paddle.cruiseAt({0, gameSpeed});
	}

	void keyReleasedPvP(ofKeyEventArgs& e) {
		if (e.key == 'w' || e.key == 's') p1Paddle.cruiseAt({0, 0});
		if (e.key == 'i' || e.key == 'k') p2Paddle.cruiseAt({0, 0});
	}

private:
	GameType gameType;
	PlayerTurn playerTurn;

	bool startRally;

	short p1Score{0}, p2Score{0};

	const float paddleWidth{20.0f};
	const float paddleHeight{100.0f};
	const float paddleEdgeBuffer{50.0f};
	const float ballWidth{20.0f};
	const float ballHeight{20.0f};
	const float gameSpeed{500.0f};

	MotionSprite p1Paddle{0, 0, paddleWidth, paddleHeight, 0, 0};
	MotionSprite p2Paddle{0, 0, paddleWidth, paddleHeight, 0, 0};
	MotionSprite ball{0, 0, ballWidth, ballHeight, 0, 0};
};

class ofApp : public ofBaseApp {

public:
	void setup() override;
	void update() override;
	void draw() override;

private:
	PongGame pongGame;
};
