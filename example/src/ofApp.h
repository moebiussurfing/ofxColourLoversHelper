
#pragma once

#include "ofMain.h"

#include "ofxColourLoversHelper.h"

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();
	void windowResized(int w, int h);

	//-

	ofxColourLoversHelper colourLoversHelper;

	//ofEventListener listener_LoverName;

	//-

	// local palette
	void drawPalette();
	std::string myPalette_Name = "";
	ofColor myColor;
	vector<ofColor> myPalette;
};
