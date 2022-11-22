#include "pongGame.h"

void PongGame::setup() {
	startRally = true;
	playerTurn = (ofRandom(100) > 50) ? PlayerTurn::player1 : PlayerTurn::player2;

	ofAddListener(ofEvents().keyPressed, this, &PongGame::keyPressedPvP);
	ofAddListener(ofEvents().keyReleased, this, &PongGame::keyReleasedPvP);
}

void PongGame::update(double deltaTime, float canvasWidth, float canvasHeight) {
	if (startRally) {
		restartRally(canvasWidth, canvasHeight);
	}

	gamePlay(deltaTime, canvasWidth, canvasHeight);
	checkForWin(canvasWidth);
}

void PongGame::draw() const {
	ofDrawBitmapString("P1: " + std::to_string(p1Score), 200, 40);
	ofDrawBitmapString("P2: " + std::to_string(p2Score), 550, 40);

	p1Paddle.draw();
	p2Paddle.draw();

	ball.draw();
}

void PongGame::restartRally(float canvasWidth, float canvasHeight) {
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

void PongGame::gamePlay(float deltaTime, float canvasWidth, float canvasHeight) {
	// Paddle movement.
	p1Paddle.move(deltaTime);
	p2Paddle.move(deltaTime);
	p1Paddle.clampToBoundary({0, 0}, {canvasWidth, canvasHeight});
	p2Paddle.clampToBoundary({0, 0}, {canvasWidth, canvasHeight});

	// Ball movement.
	ball.move(deltaTime);
	//ball.warpTo({ofGetMouseX(), ofGetMouseY()}); // For testing uncomment this and comment-out the line above.
	ball.bounceHorizontalWithEdge(0, canvasHeight);
	ball.bounceWith(p1Paddle);
	ball.bounceWith(p2Paddle);
}

void PongGame::checkForWin(float canvasWidth) {
	const Edge offEdge = ball.beyondWhichVerticalBound(0, canvasWidth);

	if (offEdge == Edge::none) return;

	// Someone just got a point, so the rally restarts.
	startRally = true;

	// Was it player 1 who scored?
	if (offEdge == Edge::right) {
		++p1Score;
		playerTurn = PlayerTurn::player1;
	}

	// Or was it player 2?
	if (offEdge == Edge::left) {
		++p2Score;
		playerTurn = PlayerTurn::player2;
	}
}

void PongGame::keyPressedPvP(ofKeyEventArgs& e) {
	if (e.key == 'w') p1Paddle.cruiseAt({0, -gameSpeed});
	if (e.key == 's') p1Paddle.cruiseAt({0, gameSpeed});
	if (e.key == 'i') p2Paddle.cruiseAt({0, -gameSpeed});
	if (e.key == 'k') p2Paddle.cruiseAt({0, gameSpeed});
}

void PongGame::keyReleasedPvP(ofKeyEventArgs& e) {
	if (e.key == 'w' || e.key == 's') p1Paddle.cruiseAt({0, 0});
	if (e.key == 'i' || e.key == 'k') p2Paddle.cruiseAt({0, 0});
}
