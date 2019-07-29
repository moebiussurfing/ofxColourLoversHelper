#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofBackground(64);

    // set positions and panel sizes
    glm::vec2 posGui(1000, 0);
    glm::vec2 sizeGui(200, 400);
    glm::vec2 posGrid(posGui.x+sizeGui.x+2, 0);
    glm::vec2 sizeGrid(120, ofGetHeight());

    //must be called before setup() to overwrite default settings
    ColourLoversHelper.setGrid(posGrid, sizeGrid);
    ColourLoversHelper.setup(posGui, sizeGui);

    // receivers pointers
    ColourLoversHelper.setColor_BACK(myColor);
    ColourLoversHelper.setPalette_BACK(myPalette);
    ColourLoversHelper.setPalette_Name_BACK(myPalette_Name);

    // initiation values
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

    // preview receivers

    if (SHOW_gui) {
        int x, y, w, h, pad, lineH;
        x = 10;
        y = ofGetHeight() - 200;
        w = h = 40;
        pad = 3;
        lineH = 20;

        ofPushStyle();
        ofFill();

        ofDrawBitmapStringHighlight("myColor:", x, y, ofColor::black, ofColor::white);
        y += pad;

        ofSetColor(myColor);
        ofDrawRectangle(ofRectangle(x, y, w, h));
        y += (h + pad);

        y += (lineH);
        ofDrawBitmapStringHighlight("myPalette:", x, y, ofColor::black, ofColor::white);
        y += pad;

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
void ofApp::exit(){
    ColourLoversHelper.exit();
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
        SHOW_gui = !SHOW_gui;
        ColourLoversHelper.setVisible(SHOW_gui);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
