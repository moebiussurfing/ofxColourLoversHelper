#pragma once
#include "ofMain.h"

#include "ofxColourLoversHelper.h"

/*

NOTE:

to run this exampe out-of-the-box
Go to the file ofxColourLoversHelper.h
You must un/comment this lines like this:

#define USE_OFX_IM_GUI
//#define USE_OFX_IM_GUI_EXTERNAL -> required to enable the one only / local ImGui

*/


class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void windowResized(int w, int h);

	ofxColourLoversHelper colourLoversHelper;

	// local palette
	void drawPalette();
	std::string myPalette_Name = "";
	ofColor myColor;
	vector<ofColor> myPalette;

	//ofEventListener listener_LoverName;

	bool bActiveTextInputLovers;
};
