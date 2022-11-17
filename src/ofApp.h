#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
	void setup() override;
	void update() override;
	void draw() override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void windowResized(int w, int h) override;

private:
	const int canvasWidth{800}, canvasHeight{500};

	bool startRally{true};
	bool p1Serves{ofRandom(0, 100) < 50};

	bool p1UpPressed{false}, p1DownPressed{false}, p2UpPressed{false}, p2DownPressed{false};

	float ballXPosition{0.0f}, ballYPosition{0.0f};
	float ballXSpeed{0.0f}, ballYSpeed{0.0f};

	float p1YPosition{0.0f}, p2YPosition{0.0f};

	short p1Score{0}, p2Score{0};
};
