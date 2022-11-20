#pragma once

#include "ofMain.h"
#include "pongGame.h"

class ofApp : public ofBaseApp {

public:
	void setup() override;
	void update() override;
	void draw() override;

private:
	float canvasWidth{800}, canvasHeight{500};
	PongGame pongGame;
};
