#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(100);
	ofSetWindowShape(canvasWidth, canvasHeight);
	ofSetRectMode(OF_RECTMODE_CENTER);
}

//--------------------------------------------------------------
void ofApp::update() {
	// RALLY RESTART (Conditional on bool)
	// MOVE PADDLES (Conditional on user input)
	// MOVE BALL
	// BALL BOUNCE ON EDGE (Conditional y position of ball)
	// BALL BOUNCE ON PADDLE 1 (Conditional y pos & x pos of ball in relation to P1)
	// BALL BOUNCE ON PADDLE 2 (Conditional y pos & x pos of ball in relation to P2)
	// CHECK FOR POINT SCORED (Conditional x pos of ball)
}

//--------------------------------------------------------------
void ofApp::draw() {
	// DRAW SCORES
	// DRAW PADDLES
	// DRAW BALL
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// SET USER INPUT BOOLS 
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	// UNSET USER INPUT BOOLS
}
