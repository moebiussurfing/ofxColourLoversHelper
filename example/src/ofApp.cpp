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
    ColourLoversHelper.setPalette_Name_BACK(myPalette_Name);

    // some initiation values..
    myColor = ofColor::white;
    myPalette.resize(2);//pointer setter whill clear/resize. nevermind the vector size here
    myPalette[0] = ofColor::white;
    myPalette[0] = ofColor::white;
    myPalette_Name = "NOT LOADED";
}

//--------------------------------------------------------------
void ofApp::update(){
    ColourLoversHelper.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ColourLoversHelper.draw();

	//-

    // preview
    {
        int x, y, w, h, pad, lineH;
        x = 10;
        y = ofGetHeight() - 230;
        w = h = 40;
        pad = 3;
        lineH = 20;

        ofPushStyle();
        ofFill();

        ofDrawBitmapStringHighlight("ofApp LOCAL COLORS", x, y, ofColor::black, ofColor::white);
        y += 3*pad+lineH;

        ofDrawBitmapStringHighlight("myColor:", x, y, ofColor::black, ofColor::white);
        y += 3*pad;

        ofSetColor(myColor);
        ofDrawRectangle(ofRectangle(x, y, w, h));
        y += (h+pad);

        y += (lineH);
        ofDrawBitmapStringHighlight("myPalette:", x, y, ofColor::black, ofColor::white);
        y += 3*pad;

        for (int i = 0; i < myPalette.size(); i++) {
            ofSetColor(myPalette[i]);
            ofDrawRectangle(ofRectangle(x + i * (w + pad), y, w, h));
        }
        y += (h+pad);

        y += (lineH);
        ofDrawBitmapStringHighlight("myPalette_Name:", x, y, ofColor::black, ofColor::white);
        y += (lineH);
        ofDrawBitmapStringHighlight(myPalette_Name, x, y, ofColor::black, ofColor::white);

        ofPopStyle();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == OF_KEY_DOWN)
    {
        ColourLoversHelper.nextPalette();
    }
    if (key == OF_KEY_UP)
    {
        ColourLoversHelper.prevPalette();
    }

    if (key == 'g')
    {
        ColourLoversHelper.setToggleVisible();
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    ColourLoversHelper.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::exit() {
	ColourLoversHelper.exit();
}