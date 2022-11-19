#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(100);
	ofSetWindowShape(canvasWidth, canvasHeight);
	ofSetWindowPosition(900, 200);
	ofSetRectMode(OF_RECTMODE_CENTER);
}

//--------------------------------------------------------------
void ofApp::update() {
	// RALLY RESTART (Conditional on bool)
	if (startRally) {
		startRally = false;
		ballXPosition = ofGetWidth() / 2.0f;
		ballYPosition = p2YPosition = p1YPosition = ofGetHeight() / 2.0f;

		ballXSpeed = p1Serves ? 300 : -300;
		ballYSpeed = ofRandom(0, 200) - 100; // Random from -100 to 100
	}

	// MOVE BALL (Delta Time Aware)
	ballXPosition += ballXSpeed * ofGetLastFrameTime();
	ballYPosition += ballYSpeed * ofGetLastFrameTime();

	// MOVE PADDLES (Conditional on user input. Delta time aware.)

	const double speedChange{300 * ofGetLastFrameTime()};

	if (p1UpPressed) p1YPosition -= speedChange;
	if (p1DownPressed) p1YPosition += speedChange;
	if (p2UpPressed) p2YPosition -= speedChange;
	if (p2DownPressed) p2YPosition += speedChange;

	p1YPosition = ofClamp(p1YPosition, 50, 450);
	p2YPosition = ofClamp(p2YPosition, 50, 450);

	// BALL BOUNCE ON EDGE (Conditional y position of ball)

	if (ballYPosition <= 10 || ballYPosition >= 490) {
		ballYSpeed *= -1;
		// Maybe instead of clamping the ball should be push back beyond the 
		// edge by the overlap distance?
		ballYPosition = ofClamp(ballYPosition, 10, 490);
	}

	// BALL BOUNCE ON PADDLE 1 (Conditional y pos & x pos of ball in relation to P1)
	if ((ballXPosition < 70 && ballXPosition > 50)
		&& (ballYPosition < p1YPosition + 60)
		&& (ballYPosition > p1YPosition - 60)) {
		ballXSpeed *= -1;
		ballYSpeed += 2 * (ballYPosition - p1YPosition);
		ballXPosition = 70;
	}

	// BALL BOUNCE ON PADDLE 2 (Conditional y pos & x pos of ball in relation to P2)
	if ((ballXPosition > 730 && ballXPosition < 750)
		&& (ballYPosition < p2YPosition + 60)
		&& (ballYPosition > p2YPosition - 60)) {
		ballXSpeed *= -1;
		ballYSpeed += 2 * (ballYPosition - p2YPosition);
		ballXPosition = 730;
	}

	// CHECK FOR POINT SCORED (Conditional x pos of ball)
	if (ballXPosition < 0) {
		++p2Score;
		startRally = true;
		p1Serves = false;
	} else if (ballXPosition > canvasWidth) {
		++p1Score;
		startRally = true;
		p1Serves = true;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	// DRAW SCORES
	ofSetColor(ofColor::white);
	ofDrawBitmapString("P1: " + std::to_string(p1Score), 200, 40);
	ofDrawBitmapString("P2: " + std::to_string(p2Score), 550, 40);

	// DRAW PADDLES
	ofDrawRectangle(50, p1YPosition, 20, 100);
	ofDrawRectangle(750, p2YPosition, 20, 100);

	// DRAW BALL
	ofSetColor(ofColor::cornflowerBlue);
	ofDrawRectangle(ballXPosition, ballYPosition, 20, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// SET USER INPUT BOOLS
	if (key == 'w') p1UpPressed = true;
	if (key == 's') p1DownPressed = true;
	if (key == OF_KEY_UP) p2UpPressed = true;
	if (key == OF_KEY_DOWN) p2DownPressed = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	// UNSET USER INPUT BOOLS
	if (key == 'w') p1UpPressed = false;
	if (key == 's') p1DownPressed = false;
	if (key == OF_KEY_UP) p2UpPressed = false;
	if (key == OF_KEY_DOWN) p2DownPressed = false;
}
