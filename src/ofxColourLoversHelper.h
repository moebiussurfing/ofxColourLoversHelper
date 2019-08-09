
#pragma once
#include "ofMain.h"

#include "ofxUI.h"
#include "ofxColourLovers.h"

class ofxColourLoversHelper {

public:

    ofxColourLoversHelper();
    ~ofxColourLoversHelper();

    void update();
    void draw();
    void exit();
    void windowResized(int w, int h);

    //--

    // API

    void setup();

    void setup(glm::vec2 _position, glm::vec2 _size);
    void setGrid(glm::vec2 _position, glm::vec2 _size);
    void setPosition(glm::vec2 _position, glm::vec2 _size);

    void setVisible(bool b);

    // pointers back to 'communicate externally'
    void setColor_BACK(ofColor &c);
    void setPalette_BACK(vector<ofColor> &p);
    void setPalette_Name_BACK(string &n);
    void setPalette_bUpdated_Palette_BACK(bool &b);
    void setPalette_bUpdated_Color_BACK(bool &b);

    void nextPalette();
    void prevPalette();
    void randomPalette();

    //--

    // pointers back to 'communicate externally'

    ofColor *myColor_BACK;
    vector<ofColor> *myPalette_BACK;
    string *myPalette_Name_BACK;
    bool *bUpdated_Palette_BACK;
    bool *bUpdated_Color_BACK;

    //----

    bool MODE_PickColor_BACK = true;//should store in xml..
    bool MODE_PickPalette_BACK = true;

    bool ENABLER_Keys = true;

private:

    void refreshPalette();

    bool MODE_fixedSize = true;

    ofColor lastColor_clicked;

    //-

    ofxUICanvas *gui;
    ofxUIScrollableCanvas *colourLab;
    ofxUICanvas *paletteView;
    void setPalette(int p);
    int palettesX;

    int currPalette; //last picked palette/color
    vector<ColourLovePalette> palettes;

    void guiEvent(ofxUIEventArgs &e);
    void colourLoveEvent(ColourLoveEvent &e);
    void colourLabEvent(ofxUIEventArgs &e);
    void colourPaletteEvent(ofxUIEventArgs &e);

    bool updateFlag;
    void updateColourLab();
    vector<ofxUIButton *> coloursPalette;
    vector<ofxUIButton *> colourRanges;
    string lastSearch;
    ofxUILabel * lastPaletteName_UI;
    string lastPaletteName = "";

    void loadFavourites();
    void loadHistory();
    void clearFavourites();
    void clearHistory();

    string path = "colourLovers/";
    ofColor colorMarked;

    //---

    bool isVisible = true;
    bool isKeysEnabled = true;

    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 gridPosition;
    glm::vec2 gridSize;

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