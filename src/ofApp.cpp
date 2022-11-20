#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(100);
	ofSetWindowShape(800, 500);
	ofSetWindowPosition(900, 200);
	ofSetRectMode(OF_RECTMODE_CENTER);
	pongGame.setup(PongGame::GameType::playerVsPlayer);
}

//--------------------------------------------------------------
void ofApp::update() {
	pongGame.update(ofGetLastFrameTime(), ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::draw() {
	pongGame.draw();
}
