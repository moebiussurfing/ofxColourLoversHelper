
#pragma once
#include "ofMain.h"

#define USE_OFX_UI
#ifdef USE_OFX_UI
#include "ofxUI.h"
#endif

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
    void setVisibleSearcher(bool b){
        bSearcherVisible = b;
        gui->setVisible(bSearcherVisible);
    }

    //bool bIsEnabled = true;
    void setKeysEnabled(bool b){
        //isKeysEnabled = b;
        //
        //if (isKeysEnabled)
        //{
        //    addKeysListeners();
        //}
        //else
        //{
        //    removeKeysListeners();
        //}
    }

    bool bSearcherVisible = true;

    // colour lovers browsing
    void nextPalette();
    void prevPalette();
    void randomPalette();

    //--

    // pointers back to 'communicate externally'
    void setColor_BACK(ofColor &c);
    void setPalette_BACK(vector<ofColor> &p);
    void setPalette_Name_BACK(std::string &n);
    void setPalette_bUpdated_Palette_BACK(bool &b);
    void setPalette_bUpdated_Color_BACK(bool &b);

    // pointers back to 'communicate externally'
    ofColor *myColor_BACK;
    vector<ofColor> *myPalette_BACK;
    std::string *myPalette_Name_BACK;
    bool *bUpdated_Palette_BACK;
    bool *bUpdated_Color_BACK;

    //----

    bool MODE_PickColor_BACK = true;//should store in xml..
    bool MODE_PickPalette_BACK = true;

    ofParameter<bool> ENABLER_Keys {"keys enabler", true};
	//bool ENABLER_Keys = true;

private:

    void refreshPalette();

    bool MODE_fixedSize = true;

    ofColor lastColor_clicked;

    //-

#ifdef USE_OFX_UI
    ofxUICanvas *gui;
    ofxUIScrollableCanvas *colourLab;
    ofxUICanvas *paletteView;

    void guiEvent(ofxUIEventArgs &e);
    void colourLoveEvent(ColourLoveEvent &e);
    void colourLabEvent(ofxUIEventArgs &e);
    void colourPaletteEvent(ofxUIEventArgs &e);
    vector<ofxUIButton *> coloursPalette;
    vector<ofxUIButton *> colourRanges;
    ofxUILabel * lastPaletteName_UI;
#endif

	//-

    void setPalette(int p);
    int palettesX;

    int currPalette; //last picked palette/color
    vector<ColourLovePalette> palettes;

    bool updateFlag;
    void updateColourLab();
    std::string lastSearch;
    std::string lastPaletteName = "";

    void loadFavourites();
    void loadHistory();
    void clearFavourites();
    void clearHistory();

    std::string path = "colourLovers/";
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
