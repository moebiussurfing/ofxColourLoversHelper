#pragma once
#include "ofMain.h"

#include "ofxColourLoversHelper.h"

/*

NOTE:

to run this exampe out-of-the-box
Go to the file ofxColourLoversHelper.h
And oou must un/comment this lines like this:

#define USE_OFX_IM_GUI
//#define USE_OFX_IM_GUI_EXTERNAL

*/


class ofApp : public ofBaseApp {
public:
	void setup();
	void draw();
	void windowResized(int w, int h);

	ofxColourLoversHelper ColourLoversHelper;

	// local palette
	void drawPalette();
	std::string myPalette_Name = "";
	ofColor myColor;
	vector<ofColor> myPalette;
};
