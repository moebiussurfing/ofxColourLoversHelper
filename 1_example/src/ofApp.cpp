#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofBackground(16);

    ColourLoversHelper.setup(glm::vec2(1000,10), glm::vec2(200, 200));

    ColourLoversHelper.setColor_BACK(myColor);
    ColourLoversHelper.setPalette_BACK(myPalette);
    ColourLoversHelper.setPalette_Name_BACK(myPalette_Name);

    myColor = ofColor::white;
    myPalette.resize(2);
    myPalette[0] = ofColor::white;
    myPalette[0] = ofColor::black;
    myPalette_Name = "NOT LOADED";
}

//--------------------------------------------------------------
void ofApp::update(){
    ColourLoversHelper.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ColourLoversHelper.draw();

    int x, y, w, h, pad;
    x = 10;
    y = 10;
    w = h = 20;
    pad = 2;

    ofPushStyle();
    ofFill();

    ofSetColor(myColor);
    ofDrawRectangle(ofRectangle(x,y,w,h));

    y += (h+pad);
    for (int i=0; i<myPalette.size(); i++)
    {
        ofSetColor(myPalette[i]);
        ofDrawRectangle(ofRectangle(x+i*w,y,w,h));
    }

    y += 2*(h+pad);
    ofDrawBitmapStringHighlight(myPalette_Name, x, y, ofColor::black, ofColor::white);

    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::exit(){
    ColourLoversHelper.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    ColourLoversHelper.nextPalette();
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
