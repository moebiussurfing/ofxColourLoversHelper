#pragma once

#include "ofMain.h"

#include "ofxColourLoversHelper.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void windowResized(int w, int h);

	ofxColourLoversHelper ColourLoversHelper;

	std::string myPalette_Name = "";
	ofColor myColor;
	vector<ofColor> myPalette;
};
