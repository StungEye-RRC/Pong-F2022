#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(100);
	ofSetWindowShape(canvasWidth, canvasHeight);
	ofSetWindowPosition(900, 200);
	ofSetRectMode(OF_RECTMODE_CENTER);
	pongGame.setup(PongGame::GameType::playerVsPlayer);
}

//--------------------------------------------------------------
void ofApp::update() {
	pongGame.update(ofGetLastFrameTime(), canvasWidth, canvasHeight);
}

//--------------------------------------------------------------
void ofApp::draw() {
	pongGame.draw();
}
