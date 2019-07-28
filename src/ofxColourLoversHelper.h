
#pragma once
#include "ofMain.h"

#include "ofxRectangleUtils.h"

#include "ofxUI.h"
#include "ofxColourLovers.h"

class ofxColourLoversHelper {

public:

    ofxColourLoversHelper();
    ~ofxColourLoversHelper();

    bool isVisible = true;
    bool isVisible_PRE = false;
    void setVisible(bool b);
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 gridPosition;
    glm::vec2 gridSize;

    void setup();

    //--

    // API

    void setup(glm::vec2 _position, glm::vec2 _size);
    void setGrid(glm::vec2 _position, glm::vec2 _size);

    void setColor_BACK(ofColor &c);
    void setPalette_BACK(vector<ofColor> &p);
    void setPalette_Name_BACK(string &n);
    ofColor *myColor_BACK;
    vector<ofColor> *myPalette_BACK;
    string *myPalette_Name_BACK;

    void nextPalette();
    void prevPalette();

    //--

    void update();
    void draw();
    void exit();

    //---

//    ofxUILabel *bgLabel;
    ofxUICanvas *gui;
    ofxUIScrollableCanvas *colourLab;
    ofxUICanvas *paletteView;
    void setPalette(int p);
    int palettesX;
    int currPalette;
    vector<ColourLovePalette> palettes;

    void guiEvent(ofxUIEventArgs &e);

    void colourLoveEvent(ColourLoveEvent &e);
    void colourLabEvent(ofxUIEventArgs &e);
    void colourPaletteEvent(ofxUIEventArgs &e);

    bool updateFlag;
    void updateColourLab();
    vector<ofxUIButton *>   coloursPalette;
    vector<ofxUIButton *>   colourRanges;
    string lastSearch;

    ofxUILabel * lastPaletteName_UI;
    string lastPaletteName = "";

    void loadFavourites();
    void loadHistory();

    //---

private:

    void keyPressed( ofKeyEventArgs& eventArgs);
    void keyReleased( ofKeyEventArgs& eventArgs );
    void addKeysListeners();
    void removeKeysListeners();

    void mouseDragged( ofMouseEventArgs& eventArgs );
    void mousePressed( ofMouseEventArgs& eventArgs );
    void mouseReleased( ofMouseEventArgs& eventArgs );
    void addMouseListeners();
    void removeMouseListeners();

};

//}