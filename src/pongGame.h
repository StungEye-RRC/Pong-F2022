#pragma once
#include "motionSprite.h"
#include "ofEvents.h"

class PongGame {
public:
	enum class PlayerTurn { player1, player2 };

private:
	PlayerTurn playerTurn;

	bool startRally;

	short p1Score{0}, p2Score{0};

	const float paddleWidth{20.0f};
	const float paddleHeight{100.0f};
	const float paddleEdgeBuffer{50.0f};
	const float ballWidth{20.0f};
	const float ballHeight{20.0f};
	const float gameSpeed{300.0f};

	MotionSprite p1Paddle{0, 0, paddleWidth, paddleHeight, 0, 0};
	MotionSprite p2Paddle{0, 0, paddleWidth, paddleHeight, 0, 0};
	MotionSprite ball{0, 0, ballWidth, ballHeight, 0, 0};


public:
	void setup();
	void update(double deltaTime, float canvasWidth, float canvasHeight);
	void draw() const;

private:
	void restartRally(float canvasWidth, float canvasHeight);
	void gamePlay(float deltaTime, float canvasWidth, float canvasHeight);
	void checkForWin(float canvasWidth);
	void keyPressedPvP(ofKeyEventArgs& e);
	void keyReleasedPvP(ofKeyEventArgs& e);
};
