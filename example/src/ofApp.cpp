#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
	 
	//-

	ColourLoversHelper.setup();

	//-

    // receivers pointers
    ColourLoversHelper.setColor_BACK(myColor);
    ColourLoversHelper.setPalette_BACK(myPalette);
    ColourLoversHelper.setPalette_BACK_Name(myPalette_Name);

    // some initiation values..
    myColor = ofColor::black;
    myPalette.resize(2);//pointer setter whill clear/resize. nevermind the vector size here
    myPalette[0] = ofColor::white;
    myPalette[1] = ofColor::grey;
    myPalette_Name = "Not Loaded..";
}

//--------------------------------------------------------------
void ofApp::drawPalette() { // preview
	{
		ofPushStyle();

		int x, y, w, h, pad, lineH;
		x = 10;
		y = ofGetHeight() - 230;
		w = h = 40;
		pad = 3;
		lineH = 20;

		ofFill();

		ofDrawBitmapStringHighlight("LOCAL COLORS (ofApp)", x, y, ofColor::black, ofColor::white);
		y += 3 * pad + lineH;

		ofDrawBitmapStringHighlight("myColor:", x, y, ofColor::black, ofColor::white);
		y += 3 * pad;

		ofSetColor(myColor);
		ofDrawRectangle(ofRectangle(x, y, w, h));
		y += (h + pad);

		y += (lineH);
		ofDrawBitmapStringHighlight("myPalette:", x, y, ofColor::black, ofColor::white);
		y += 3 * pad;

		for (int i = 0; i < myPalette.size(); i++) {
			ofSetColor(myPalette[i]);
			ofDrawRectangle(ofRectangle(x + i * (w + pad), y, w, h));
		}
		y += (h + pad);

		y += (lineH);
		ofDrawBitmapStringHighlight("myPalette_Name:", x, y, ofColor::black, ofColor::white);
		y += (lineH);
		ofDrawBitmapStringHighlight(myPalette_Name, x, y, ofColor::black, ofColor::white);

		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
    ColourLoversHelper.draw();

	drawPalette();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    ColourLoversHelper.windowResized(w, h);
}
