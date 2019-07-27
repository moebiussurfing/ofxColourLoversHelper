#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofBackground(16);

    ColourLoversHelper.setup(glm::vec2(0,0), glm::vec2(200, 700));
//    ColourLoversHelper.setup();

    ColourLoversHelper.setColor_BACK(myColor);

}

//--------------------------------------------------------------
void ofApp::update(){
    ColourLoversHelper.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ColourLoversHelper.draw();

    ofPushStyle();
    ofFill();
    ofSetColor(myColor);

    ofDrawRectangle(ofRectangle(10,750,100,100));

    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::exit(){
    ColourLoversHelper.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
