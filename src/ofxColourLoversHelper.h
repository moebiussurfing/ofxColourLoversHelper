
#pragma once
#include "ofMain.h"


//--

//	OPTIONAL
//
//	You can choice a gui
#define USE_OFX_IM_GUI
//#define USE_OFX_UI

//--


#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ImGui.h"

#ifdef USE_OFX_UI
#include "ofxUI.h"
#endif

#ifdef USE_OFX_IM_GUI
#include "ofxImGui.h"
#endif

#include "ofxColourLovers.h"

class ofxColourLoversHelper {

public:

    ofxColourLoversHelper();
    ~ofxColourLoversHelper();

	void setup(); 
	void update();
    void draw();
    void exit();
    void windowResized(int w, int h);

    //--

private:
#ifdef USE_OFX_IM_GUI
	ofxImGui::Gui gui_ImGui;
	void drawImGui();
#endif

	std::string textInput_temp1 = "";
	std::string textInput_temp1_PRE = "-1";

	//--

#ifdef USE_OFX_UI
	ofxUICanvas *gui;
	ofxUIScrollableCanvas *gui_Lab;
	ofxUICanvas *gui_Palette;
	void Changed_Gui(ofxUIEventArgs &e);
	void Changed_Gui_Lab(ofxUIEventArgs &e);
	void Changed_ColourPalette(ofxUIEventArgs &e);
	vector<ofxUIButton *> buttonColoursPalette;
	vector<ofxUIButton *> colourRanges;
	ofxUILabel * lastPaletteName_UI;
#endif

private:
	void Changed_ColourLovers(ColourLoveEvent &e);

	//--

    // API

public:
    void setup(glm::vec2 _position, glm::vec2 _size);
    void setGrid(glm::vec2 _position, glm::vec2 _size);
    void setPosition(glm::vec2 _position, glm::vec2 _size);

    void setToggleVisible();
	bool isVisible() {
		return bIsVisible;
	}
    void setVisible(bool b);
    void setVisibleSearcher(bool b){
        bSearcherVisible = b;
#ifdef USE_OFX_UI
        gui->setVisible(bSearcherVisible);
#endif
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

private:
    bool bSearcherVisible = true;

    // colour lovers browsing
public:
	void nextPalette();
    void prevPalette();
    void randomPalette();

    //--

    // pointers back to 'communicate externally'
public:
    void setColor_BACK(ofColor &c);
    void setPalette_BACK(vector<ofColor> &p);
    void setPalette_Name_BACK(std::string &n);
    void setPalette_bUpdated_Palette_BACK(bool &b);
    void setPalette_bUpdated_Color_BACK(bool &b);

    // pointers back to 'communicate externally'
private:
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

	int amountResults = 40;

	//-

    void setPalette(int p);
    int palettesX;

    int currPalette; //last picked palette/color
    vector<ColourLovePalette> palettes;

    bool updateFlag;
    void build_Gui_Lab();
    
	std::string lastSearch;
    std::string lastSearch_PRE = "-1";

    std::string lastPaletteName = "";

    void loadFavourites();
    void loadHistory();
    void clearFavourites();
    void clearHistory();

    std::string path = "colourLovers/";
    ofColor colorMarked;

    //---

    bool bIsVisible = true;
    bool isKeysEnabled = true;

    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 gridPosition;
    glm::vec2 gridSize;

    void addKeysListeners();
    void removeKeysListeners();

    void mouseDragged( ofMouseEventArgs& eventArgs );
    void mousePressed( ofMouseEventArgs& eventArgs );
    void mouseReleased( ofMouseEventArgs& eventArgs );

	void addMouseListeners();
    void removeMouseListeners();
	
public:
    void keyPressed( ofKeyEventArgs& eventArgs);
    void keyReleased( ofKeyEventArgs& eventArgs );
};
